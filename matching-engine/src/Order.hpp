#pragma once

#include <ExecutionReport.hpp>
#include <MatchingEngineDataWriterContainer.hpp>
#include <cstdint>
#include <string>

using Price = uint64_t;
using Quantity = uint64_t;
using Cost = uint64_t;
using FillId = uint32_t;
using ChangeId = uint32_t;
using OrderConditions = uint32_t;

enum OrderCondition {
  // No special conditions for the order.
  NONE = 0,

  // This order must be filled in its entirity or not at all.
  // This order cannot be partially filled.
  ALL_OR_NONE = 1,

  // This order is matched immediately against the market.
  // If this order is not matched immediately, it is cancelled.
  IMM_OR_CANCEL = ALL_OR_NONE << 1,

  // This order is both filled entirely and immediately.
  // If this order is either partially filled or not filled immediately, it is
  // cancelled.
  FILL_OR_KILL = ALL_OR_NONE | IMM_OR_CANCEL,

  // A stop order; this order only activates once a specified stop price is
  // reached.
  // When buying, this happens when the price increases up to and above the stop
  // price.
  // When selling, this happens when the price decreases down to and below the
  // stop price.
  // But once the stop price is reached, this order permanently becomes an
  // active order and follows the other rules that define it.
  STOP = IMM_OR_CANCEL << 1
};

// These are constants only used within Order.cpp.
namespace {
// The price of a market order is 0, since it's determined by the price of the
// matching order.
const Price MARKET_ORDER_PRICE(0);

// Defines the delta in price of an price-unchanged order, 0.
const Price PRICE_UNCHANGED(0);

// Max quantity that can order can contain.
const Quantity QUANTITY_MAX(UINT64_MAX);

// Defined the delta in quantity of an quantity-unchanged order, 0.
const int64_t SIZE_UNCHANGED(0);
} // namespace

class Order {
public:
  // This constructor takes in:
  // order_id: id of order
  // sender_id: id of user this order came from
  // is_buy: is this a buy order?
  // quantity: order quantity
  // price: order price
  // stop_price: if this order is a stop order, the stop price
  // conditions: any order conditions- based on the OrderCondition enum
  // symbol: stock ticker symbol of the order book this order is sent to
  // gateway: name of FIX gateway to send responses to
  // data_service: name of data service connected to FIX gateway, in DDS
  // communication this is the source_user data_writer_container_ptr: pointer to
  // the container that contains all the data writers that this matching engine
  // uses security_exchange: name of the exchange this matching engine is in
  Order(const std::string &order_id, const std::string &sender_id, bool is_buy,
        Quantity quantity, Price price, Price stop_price,
        const std::string &symbol, const std::string &gateway,
        const std::string &data_service,
        DataWriterContainerPtr data_writer_container_ptr,
        const std::string &security_exchange,
        OrderConditions conditions = OrderCondition::NONE);

  // Getter const functions
  bool is_limit() const;

  bool is_buy() const;

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

  // Callback functions for order execution lifecycle
  void on_accepted();

  void on_rejected(const std::string &reason);

  void on_filled(Quantity filled_quantity, Cost fill_cost);

  void on_cancelled();

  void on_cancel_rejected(const std::string &reason);

  // Populate execution report for the order currently being executed to pass to
  // the user via the FIX gateway
  void populate_execution_report(
      DistributedStockExchange_ExecutionReport::ExecutionReport
          &execution_report,
      const char exec_type);

private:
  // Default order properties
  std::string m_order_id;
  std::string m_sender_id;
  bool m_is_buy;
  Quantity m_quantity;
  Price m_price;
  Price m_stop_price;
  std::string m_symbol;
  OrderConditions m_order_conditions;

  // Context of order in distributed system properties
  std::string m_gateway;
  std::string m_data_service;
  DataWriterContainerPtr m_data_writer_container_ptr;
  std::string m_security_exchange;

  // Active execution order properties (state of order through matching engine)
  Quantity m_quantity_filled;
  Quantity m_quantity_in_market;
  Cost m_fill_cost;
};

using OrderPtr = std::shared_ptr<Order>;

std::ostream &operator<<(std::ostream &out, const Order &order);