#pragma once

#include "Order.hpp"
#include "OrderCallback.hpp"
#include "OrderEventHandler.hpp"
#include "OrderPrice.hpp"
#include "TradeEventHandler.hpp"
#include <map>
#include <string>

// The reason we use a multimap instead of a priority queue to manage orders
// within the order book is because the multimap allows us the ability to both
// iterate through all the orders (in sorted order) and quickly find all the
// orders for a certain price within the order book. With the priority queue, we
// only have access to the topmost element, which can make some functionality of
// the order book difficult to implement (i.e cancelling orders, finding/sorting
// orders in terms of time priority, scaning multiple price levels).
// Additionally, a map is super helpful when dealing with deferred orders
// (orders that cannot be matched currently, but later), since we need to find
// the order from the map using the information from the list, this is not
// possible with the priority queue.
using OrderMap = std::multimap<OrderPrice, OrderPtr>;

using OrderVec = std::vector<OrderPtr>;
using OrderCallbackVec = std::vector<OrderCallback>;

// This list keeps track of the orders that we skipped during this match run for
// any reason.
using DeferredMatchList = std::list<typename OrderMap::iterator>;

// This class represents one stock in the matching engine.
// Orders come in and get matched with other orders in this order book.
// Every matching engine will have multiple order books.
// Each order book stores the current market price and the orders that are
// currently being executed.
class OrderBook {
public:
  // The symbol is just the stock ticker symbol.
  OrderBook(const std::string &symbol = "unknown");

  // Getter const functions.

  const std::string &get_symbol() const;

  Price get_market_price() const;

  const OrderMap &get_buy_orders() const;

  const OrderMap &get_sell_orders() const;

  const OrderMap &get_buy_stop_orders() const;

  const OrderMap &get_sell_stop_orders() const;

  // Setter functions.

  void set_symbol(const std::string &symbol);

  void set_market_price(Price market_price);

  void
  set_order_event_handler_ptr(OrderEventHandlerPtr order_event_handler_ptr);

  void
  set_trade_event_handler_ptr(TradeEventHandlerPtr trade_event_handler_ptr);

  // Action functions.

  // Returns true if the inbound order is filled within the execution of this
  // function.
  bool add(const OrderPtr &order);

  void cancel(const OrderPtr &order);

protected:
  // This is an internal method to process the pending callbacks all at once,
  // and repeat that logic until the callbacks list is empty. We use
  // m_callbacks_running to guarantee this function is not called recursively.
  void callback_now();

  // This is an internal method to execute the logic of one callback of any
  // type.
  void execute_callback(const OrderCallback &callback);

  // Returns true if the inbound order is filled within the execution of this
  // function.
  bool match_order(OrderPtr &inbound_order, Price inbound_price,
                   OrderMap &current_orders, DeferredMatchList &deferred_aons);

  // Returns true if the inbound order is filled within the execution of this
  // function.
  bool match_regular_order(OrderPtr &inbound_order, Price inbound_price,
                           OrderMap &current_orders,
                           DeferredMatchList &deferred_aons);

  // Returns true if the inbound order is filled within the execution of this
  // function.
  bool match_aon_order(OrderPtr &inbound_order, Price inbound_price,
                       OrderMap &current_orders,
                       DeferredMatchList &deferred_aons);

  // This helper function tries to create matches with the inbound order with
  // any of the deferred orders. We have a ceiling and floor on this to both not
  // overfill the inbound order, and avoid small fragmented fills. This function
  // returns the quantity actually filled.
  Quantity try_create_deferred_trades(OrderPtr &inbound_order,
                                      DeferredMatchList &deferred_matches,
                                      Quantity max_quantity,
                                      Quantity min_quantity,
                                      OrderMap &current_orders);

  // This helper function tried to create matches with any of the deferred AON
  // orders. This function returns true if there are any orders that were
  // matched.
  bool check_deferred_aon_orders(DeferredMatchList &aon_orders,
                                 OrderMap &same_side_order_map,
                                 OrderMap &other_side_order_map);

  // This helper function creates a trade between two orders, limited by the max
  // quantity. This function returns the quantity actually filled.
  Quantity create_trade(OrderPtr &inbound_order, OrderPtr &other_order,
                        Quantity max_quantity = QUANTITY_MAX);

  // This function finds an order in the order map and returns the result in the
  // result pointer. This function returns true if the order is found.
  bool find_on_market(const OrderPtr &order,
                      typename OrderMap::iterator &result);

  // This function finds an order in the stop order map and returns the result
  // in the result pointer. This function returns true if the order is found.
  bool find_in_stop_orders(const OrderPtr &order,
                           typename OrderMap::iterator &result);

  // Add incoming stop order to stop collection unless it's already in the
  // market. Returns if successful.
  bool add_stop_order(const OrderPtr &order);

  /// See if any stop orders should go on the market.
  void check_stop_orders(bool is_buy, Price price, OrderMap &stop_order_map);

  /// Accept pending (formerly stop) orders and submit them to the market.
  void submit_pending_orders();

private:
  // Order book metadata.
  std::string m_symbol;
  Price m_market_price;

  // Event handlers.
  OrderEventHandlerPtr m_order_event_handler_ptr;
  TradeEventHandlerPtr m_trade_event_handler_ptr;

  // Order state management.
  OrderMap m_buy_orders;
  OrderMap m_sell_orders;
  OrderMap m_stop_buy_orders;
  OrderMap m_stop_sell_orders;
  OrderVec m_pending_orders;

  // OrderCallback state management.
  OrderCallbackVec m_callbacks;
  OrderCallbackVec m_working_callbacks;
  bool m_callbacks_running;
};