#pragma once

#include "OrderBookTypes.hpp"
#include "OrderConstants.hpp"
#include "OrderEventHandlerTypes.hpp"
#include "TradeEventHandlerTypes.hpp"

// This class represents one stock in the matching engine.
// Orders come in and get matched with other orders in this order book.
// Every matching engine will have multiple order books.
// Each order book stores the current market price and the orders that are
// currently being executed.
// This class is always executed on one thread.
class OrderBook {
public:
  // The symbol is just the stock ticker symbol.
  OrderBook(Price initial_market_price, const std::string &symbol = "unknown");

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

  void set_order_event_handler_ptr(
      const OrderEventHandlerPtr &order_event_handler_ptr);

  void set_trade_event_handler_ptr(
      const TradeEventHandlerPtr &trade_event_handler_ptr);

  // Action functions.

  // Returns true if the inbound order is filled within the execution of this
  // function.
  bool add(const OrderPtr &order);

  void cancel(const OrderPtr &order);

protected:
  // This helper method actually adds the order to the order book by first
  // trying to match it with other orders, then adding it to the order map if
  // its not an immediate or cancel order. Returns true if the inbound order is
  // matched within the execution of this function.
  bool add_order(const OrderPtr &order);

  // Returns true if the inbound order is filled within the execution of this
  // function.
  bool match_order(const OrderPtr &inbound_order,
                   OrderMap &other_side_order_map,
                   DeferredMatchList &deferred_aon_orders);

  // Returns true if the inbound order is filled within the execution of this
  // function.
  bool match_regular_order(const OrderPtr &inbound_order,
                           OrderMap &other_side_order_map,
                           DeferredMatchList &deferred_aon_orders);

  // Returns true if the inbound order is filled within the execution of this
  // function.
  bool match_aon_order(const OrderPtr &inbound_order,
                       OrderMap &other_side_order_map,
                       DeferredMatchList &deferred_aon_orders);

  // This helper function tries to create matches with the inbound order with
  // any of the deferred orders. We have a ceiling and floor on this to both not
  // overfill the inbound order, and avoid small fragmented fills. This function
  // returns the quantity actually filled.
  Quantity try_create_deferred_trades(const OrderPtr &inbound_order,
                                      DeferredMatchList &deferred_matches,
                                      Quantity max_quantity,
                                      Quantity min_quantity,
                                      OrderMap &current_orders);

  // This helper function tried to create matches with any of the deferred AON
  // orders. This function returns true if there are any orders that were
  // matched.
  bool check_deferred_aon_orders(DeferredMatchList &aon_orders,
                                 OrderMap &deferred_order_map,
                                 OrderMap &other_side_order_map);

  // This helper function creates a trade between two orders, limited by the max
  // quantity. This function returns the quantity actually filled.
  Quantity create_trade(const OrderPtr &inbound_order,
                        const OrderPtr &other_order,
                        Quantity max_quantity = QUANTITY_MAX);

  // This function finds an order in the order map and returns the result in the
  // result pointer. This function returns true if the order is found.
  bool find_in_market(const OrderPtr &order,
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
  void add_pending_orders();

  // Virtual functions for DepthOrderBook.hpp to implement additional
  // functionality onto.

  virtual void on_accept(const OrderPtr &order);

  virtual void on_accept_stop(const OrderPtr &order);

  virtual void on_trigger_stop(const OrderPtr &order);

  virtual void on_reject(const OrderPtr &order, const char *reason);

  virtual void on_fill(const OrderPtr &order, const OrderPtr &matched_order,
                       Quantity quantity_filled, Price fill_price);

  virtual void on_cancel(const OrderPtr &order);

  virtual void on_cancel_stop(const OrderPtr &order);

  virtual void on_cancel_reject(const OrderPtr &order, const char *reason);

  virtual void on_trade(const OrderBook *book, Quantity quantity, Price price);

  virtual void on_order_book_change();

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
  OrderMap m_buy_stop_orders;
  OrderMap m_sell_stop_orders;
  OrderVec m_pending_orders;
};