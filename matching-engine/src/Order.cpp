#include "Order.hpp"

Order::Order(const std::string &order_id, const std::string &sender_id,
             bool is_buy, Quantity quantity, Price price, Price stop_price,
             const std::string &symbol, const std::string &gateway,
             const std::string &data_service,
             DataWriterContainerPtr data_writer_container_ptr,
             const std::string &security_exchange, OrderConditions conditions)
    : m_order_id(order_id), m_sender_id(sender_id), m_is_buy(is_buy),
      m_quantity(quantity), m_price(price), m_stop_price(stop_price),
      m_symbol(symbol), m_gateway(gateway), m_data_service(data_service),
      m_data_writer_container_ptr(data_writer_container_ptr),
      m_security_exchange(security_exchange), m_order_conditions(conditions),
      m_quantity_filled(0), m_quantity_in_market(0), m_fill_cost(0) {}

bool Order::is_limit() const {
  return Order::get_price() != MARKET_ORDER_PRICE;
}

bool Order::is_buy() const { return m_is_buy; }

Price Order::get_price() const { return m_price; }

Price Order::get_stop_price() const { return m_stop_price; }

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

void Order::on_accepted() { m_quantity_in_market = m_quantity; }

void Order::on_filled(Quantity filled_quantity, Cost fill_cost) {
  m_quantity_in_market -= filled_quantity;
  m_quantity_filled += filled_quantity;
  m_fill_cost += fill_cost;
}

void Order::on_cancelled() { m_quantity_in_market = 0; }

void Order::on_cancel_rejected(const std::string &reason) {
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