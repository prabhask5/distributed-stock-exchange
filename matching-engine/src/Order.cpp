#include "Order.hpp"
#include "MatchingEngineDataWriterContainer.hpp"
#include "OrderConstants.hpp"
#include "OrderPrice.hpp"
#include <DefaultDomainParticipantConstants.hpp>
#include <ExecutionReport.hpp>
#include <LoggerHelper.hpp>
#include <OrderCancelReject.hpp>
#include <OrderCancelRejectLogger.hpp>
#include <cstdint>
#include <quickfix/FixValues.h>
#include <string>

Order::Order(const std::string &order_id, const std::string &sender_id,
             bool is_buy, Quantity quantity, Price price, Price stop_price,
             const std::string &symbol, const std::string &gateway,
             const std::string &data_service,
             const DataWriterContainerPtr &data_writer_container_ptr,
             const std::string &security_exchange, OrderConditions conditions)
    : m_order_id(order_id), m_sender_id(sender_id), m_is_buy(is_buy),
      m_quantity(quantity), m_order_price(OrderPrice(price, is_buy)),
      m_stop_order_price(OrderPrice(stop_price, is_buy)), m_symbol(symbol),
      m_gateway(gateway), m_data_service(data_service),
      m_data_writer_container_ptr(data_writer_container_ptr),
      m_security_exchange(security_exchange), m_order_conditions(conditions),
      m_quantity_filled(0), m_quantity_in_market(0), m_fill_cost(0) {}

bool Order::is_limit() const {
  return Order::get_price() != MARKET_ORDER_PRICE;
}

bool Order::is_buy() const { return m_is_buy; }

const OrderPrice &Order::get_order_price() const { return m_order_price; }

const OrderPrice &Order::get_stop_order_price() const {
  return m_stop_order_price;
}

Price Order::get_price() const { return m_order_price.get_price(); }

Price Order::get_stop_price() const { return m_stop_order_price.get_price(); }

Quantity Order::get_quantity() const { return m_quantity; }

bool Order::is_all_or_none() const {
  return (m_order_conditions & OrderCondition::ALL_OR_NONE) != 0;
}

bool Order::is_immediate_or_cancel() const {
  return (m_order_conditions & OrderCondition::IMM_OR_CANCEL) != 0;
}

OrderConditions Order::get_order_conditions() const {
  return m_order_conditions;
}

std::string Order::get_symbol() const { return m_symbol; }

std::string Order::get_order_id() const { return m_order_id; }

std::string Order::get_sender_id() const { return m_sender_id; }

Quantity Order::get_quantity_filled() const { return m_quantity_filled; }

Quantity Order::get_quantity_in_market() const { return m_quantity_in_market; }

Cost Order::get_fill_cost() const { return m_fill_cost; }

bool Order::is_filled() const { return m_quantity_filled == m_quantity; }

bool Order::is_stop() const {
  return ((m_order_conditions & OrderCondition::STOP) != 0) &&
         m_stop_order_price.get_price() > 0;
}

void Order::on_accepted() { m_quantity_in_market = m_quantity; }

void Order::on_rejected(const char *reason) {}

void Order::on_filled(Quantity filled_quantity, Cost fill_cost) {
  m_quantity_in_market -= filled_quantity;
  m_quantity_filled += filled_quantity;
  m_fill_cost += fill_cost;
}

void Order::on_cancelled() { m_quantity_in_market = 0; }

void Order::on_cancel_rejected(const char *reason) {
  DistributedStockExchange_OrderCancelReject::OrderCancelReject
      order_cancel_reject;

  order_cancel_reject.SourceUser(m_data_service);
  order_cancel_reject.Destination(m_gateway);
  order_cancel_reject.DestinationUser(m_sender_id);

  order_cancel_reject.fix_header().MsgType("9");
  order_cancel_reject.Text(reason);
  order_cancel_reject.ClOrdID(m_order_id);

  log_debug<std::stringstream, OrderCancelRejectLogger,
            DistributedStockExchange_OrderCancelReject::OrderCancelReject>(
      logger, order_cancel_reject, "OrderCancelReject");

  int ret = m_data_writer_container_ptr->orderCancelRejectDW->write(
      &order_cancel_reject);

  if (ret != eprosima::fastdds::dds::RETCODE_OK) {
    LOG4CXX_ERROR(logger, "OrderCancelReject write returned :" << ret);
  }
}

void Order::on_trigger_stop() { m_order_conditions = OrderCondition::NONE; }

void Order::populate_execution_report(
    DistributedStockExchange_ExecutionReport::ExecutionReport &execution_report,
    const char exec_type) {
  execution_report.Source("MATCHING_ENGINE");
  execution_report.SourceUser(m_data_service);
  execution_report.Destination(m_gateway);
  execution_report.DestinationUser(m_sender_id);
  execution_report.fix_header().MsgType("8");
  execution_report.OrderID(m_order_id);
  execution_report.Side(is_buy() ? '1' : '2');
  execution_report.Symbol(get_symbol());
  execution_report.SecurityExchange(m_security_exchange);
  execution_report.ExecType(exec_type);
  execution_report.CumQty(get_quantity_filled());
  execution_report.LeavesQty(get_quantity_in_market());
  execution_report.Price(get_price());
  execution_report.StopPx(get_stop_price());
  execution_report.TransactTime(
      std::chrono::duration_cast<std::chrono::microseconds>(
          std::chrono::system_clock::now().time_since_epoch())
          .count());
  execution_report.LastPx(0);
  execution_report.LastQty(0);
  execution_report.OrderQty(get_quantity());
  execution_report.OrdStatus(exec_type);
  execution_report.OrdRejReason(0);
  execution_report.Text("OK");

  if (get_stop_price() != 0) {
    execution_report.OrdType(FIX::OrdType_STOP);
  } else if (get_price() != 0) {
    execution_report.OrdType(FIX::OrdType_LIMIT);
  } else {
    execution_report.OrdType(FIX::OrdType_MARKET);
  }

  if (is_all_or_none()) {
    execution_report.ExecInst("G");
  }

  if (m_order_conditions == OrderCondition::IMM_OR_CANCEL) {
    execution_report.TimeInForce(FIX::TimeInForce_IMMEDIATE_OR_CANCEL);
  } else if (m_order_conditions == OrderCondition::FILL_OR_KILL) {
    execution_report.TimeInForce(FIX::TimeInForce_FILL_OR_KILL);
  } else {
    execution_report.TimeInForce(FIX::TimeInForce_DAY);
  }

  if (get_quantity_filled() > 0) {
    execution_report.AvgPx(std::nearbyint(
        get_fill_cost() / get_quantity_filled())); // round to the nearest tick
  } else {
    execution_report.AvgPx(0); // avoid scientific numbers
  }
}

std::ostream &operator<<(std::ostream &out, const Order &order) {
  out << "[#" << order.get_order_id();
  out << ' ' << (order.is_buy() ? "BUY" : "SELL");
  out << ' ' << order.get_quantity();
  out << ' ' << order.get_symbol();

  if (order.get_price() == 0) {
    out << " MKT";
  } else {
    out << " $" << order.get_price();
  }

  if (order.get_stop_price() != 0) {
    out << " STOP " << order.get_stop_price();
  }

  out << (order.is_all_or_none() ? " AON" : "")
      << (order.is_immediate_or_cancel() ? " IOC" : "");

  auto onMarket = order.get_quantity_in_market();
  if (onMarket != 0) {
    out << " Open: " << onMarket;
  }

  auto filled = order.get_quantity_filled();
  if (filled != 0) {
    out << " Filled: " << filled;
  }

  auto cost = order.get_fill_cost();
  if (cost != 0) {
    out << " Cost: " << cost;
  }

  out << ']';

  return out;
}