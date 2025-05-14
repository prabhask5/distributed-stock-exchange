#pragma once

#include "MatchingEngineDataWriterContainerTypes.hpp"
#include "OrderPrice.hpp"
#include "OrderTypes.hpp"
#include <string>

namespace DistributedStockExchange_ExecutionReport {
class ExecutionReport; // From ExecutionReport.hpp.
}

class Order {
public:
  // This constructor takes in:
  // - order_id: id of order.
  // - sender_id: id of user this order came from.
  // - is_buy: is this a buy order?
  // - quantity: order quantity.
  // - price: order price.
  // - stop_price: if this order is a stop order, the stop price.
  // - conditions: any order conditions- based on the OrderCondition enum.
  // - symbol: stock ticker symbol of the order book this order is sent to.
  // - gateway: name of FIX gateway to send responses to.
  // - data_service: name of data service connected to FIX gateway, in DDS
  //   communication this is the source_user.
  // - data_writer_container_ptr: pointer to the container that contains
  //   all the data writers that this matching engine uses.
  // - security_exchange: name of the exchange this matching engine is in.
  Order(const std::string &order_id, const std::string &sender_id, bool is_buy,
        Quantity quantity, Price price, Price stop_price,
        const std::string &symbol, const std::string &gateway,
        const std::string &data_service,
        const DataWriterContainerPtr &data_writer_container_ptr,
        const std::string &security_exchange,
        OrderConditions conditions = OrderCondition::NONE);

  // Getter const functions.

  // Determines if an order is a limit order, or an order that is restricted by
  // the price. If the order is a buy order, the price is the maximum price to
  // be paid. If the order is a sell order, the price is the minimum price to be
  // paid.
  bool is_limit() const;

  bool is_buy() const;

  const OrderPrice &get_order_price() const;

  const OrderPrice &get_stop_order_price() const;

  Price get_price() const;

  Price get_stop_price() const;

  Quantity get_quantity() const;

  bool is_all_or_none() const;

  bool is_immediate_or_cancel() const;

  OrderConditions get_order_conditions() const;

  std::string get_symbol() const;

  std::string get_order_id() const;

  std::string get_sender_id() const;

  Quantity get_quantity_filled() const;

  Quantity get_quantity_in_market() const;

  Cost get_fill_cost() const;

  bool is_filled() const;

  bool is_stop() const;

  // Callback functions for order execution lifecycle.

  void on_accepted();

  void on_rejected(const char *reason);

  void on_filled(Quantity filled_quantity, Cost fill_cost);

  void on_cancelled();

  void on_cancel_rejected(const char *reason);

  void on_trigger_stop();

  // Populate execution report for the order currently being executed to pass to
  // the user via the FIX gateway.

  void populate_execution_report(
      DistributedStockExchange_ExecutionReport::ExecutionReport
          &execution_report,
      const char exec_type);

private:
  // Default order properties.

  std::string m_order_id;
  std::string m_sender_id;
  bool m_is_buy;
  Quantity m_quantity;
  OrderPrice m_order_price;
  OrderPrice m_stop_order_price;
  std::string m_symbol;
  OrderConditions m_order_conditions;

  // Context of order in distributed system properties.

  std::string m_gateway;
  std::string m_data_service;
  DataWriterContainerPtr m_data_writer_container_ptr;
  std::string m_security_exchange;

  // Active execution order properties (state of order through matching engine).

  // m_quantity_filled + m_quantity_in_market = m_quantity
  Quantity m_quantity_filled;
  Quantity m_quantity_in_market;
  Cost m_fill_cost;
};

std::ostream &operator<<(std::ostream &out, const Order &order);