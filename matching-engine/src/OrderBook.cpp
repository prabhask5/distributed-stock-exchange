#include "OrderBook.hpp"

OrderBook::OrderBook(const std::string &symbol) {
  // TODO: Initialize order book
}

const std::string &OrderBook::get_symbol() const {
  // TODO: Return stock symbol
  static const std::string empty = "";
  return empty;
}

Price OrderBook::get_market_price() const {
  // TODO: Return current market price
  return 0;
}

const OrderMap &OrderBook::get_buy_orders() const {
  // TODO: Return buy orders
  static const OrderMap dummy;
  return dummy;
}

const OrderMap &OrderBook::get_sell_orders() const {
  // TODO: Return sell orders
  static const OrderMap dummy;
  return dummy;
}

const OrderMap &OrderBook::get_buy_stop_orders() const {
  // TODO: Return stop buy orders
  static const OrderMap dummy;
  return dummy;
}

const OrderMap &OrderBook::get_sell_stop_orders() const {
  // TODO: Return stop sell orders
  static const OrderMap dummy;
  return dummy;
}

void OrderBook::set_symbol(const std::string &symbol) {
  // TODO: Set symbol
}

void OrderBook::set_market_price(Price market_price) {
  // TODO: Set market price
}

void OrderBook::set_order_event_handler_ptr(OrderEventHandlerPtr handler_ptr) {
  // TODO: Set order event handler
}

void OrderBook::set_trade_event_handler_ptr(TradeEventHandlerPtr handler_ptr) {
  // TODO: Set trade event handler
}

bool OrderBook::add(const OrderPtr &order) {
  // TODO: Add an order
  return false;
}

void OrderBook::cancel(const OrderPtr &order) {
  // TODO: Cancel an order
}

void OrderBook::callback_now() {
  // TODO: Execute pending callbacks
}

void OrderBook::execute_callback(const OrderCallback &callback) {
  // TODO: Handle one callback
}

bool OrderBook::match_order(OrderPtr &inbound_order, Price inbound_price,
                            OrderMap &current_orders,
                            DeferredMatchList &deferred_aons) {
  // TODO: Match order with current orders
  return false;
}

bool OrderBook::match_regular_order(OrderPtr &inbound_order,
                                    Price inbound_price,
                                    OrderMap &current_orders,
                                    DeferredMatchList &deferred_aons) {
  // TODO: Match regular order
  return false;
}

bool OrderBook::match_aon_order(OrderPtr &inbound_order, Price inbound_price,
                                OrderMap &current_orders,
                                DeferredMatchList &deferred_aons) {
  // TODO: Match AON order
  return false;
}

Quantity OrderBook::try_create_deferred_trades(
    OrderPtr &inbound_order, DeferredMatchList &deferred_matches,
    Quantity max_quantity, Quantity min_quantity, OrderMap &current_orders) {
  // TODO: Create trades from deferred matches
  return 0;
}

bool OrderBook::check_deferred_aon_orders(DeferredMatchList &aon_orders,
                                          OrderMap &same_side_order_map,
                                          OrderMap &other_side_order_map) {
  // TODO: Match AON orders that were deferred
  return false;
}

Quantity OrderBook::create_trade(OrderPtr &inbound_order, OrderPtr &other_order,
                                 Quantity max_quantity) {
  // TODO: Create a trade
  return 0;
}

bool OrderBook::find_on_market(const OrderPtr &order,
                               typename OrderMap::iterator &result) {
  // TODO: Find order in active market
  return false;
}

bool OrderBook::find_in_stop_orders(const OrderPtr &order,
                                    typename OrderMap::iterator &result) {
  // TODO: Find order in stop list
  return false;
}

bool OrderBook::add_stop_order(const OrderPtr &order) {
  // TODO: Add a stop order
  return false;
}

void OrderBook::check_stop_orders(bool is_buy, Price price,
                                  OrderMap &stop_order_map) {
  // TODO: Trigger stop orders
}

void OrderBook::submit_pending_orders() {
  // TODO: Move pending stop orders to active market
}
