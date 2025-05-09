#include "OrderBook.hpp"

OrderBook::OrderBook(const std::string &symbol)
    : m_symbol(symbol), m_callbacks_running(false),
      m_order_event_handler_ptr(nullptr), m_trade_event_handler_ptr(nullptr),
      m_market_price(MARKET_ORDER_PRICE) {
  m_callbacks.reserve(CALLBACK_VEC_STARTING_SIZE);
  m_working_callbacks.reserve(CALLBACK_VEC_STARTING_SIZE);
}

const std::string &OrderBook::get_symbol() const { return m_symbol; }

Price OrderBook::get_market_price() const { return m_market_price; }

const OrderMap &OrderBook::get_buy_orders() const { return m_buy_orders; }

const OrderMap &OrderBook::get_sell_orders() const { return m_sell_orders; }

const OrderMap &OrderBook::get_buy_stop_orders() const {
  return m_buy_stop_orders;
}

const OrderMap &OrderBook::get_sell_stop_orders() const {
  return m_sell_stop_orders;
}

void OrderBook::set_symbol(const std::string &symbol) { m_symbol = symbol; }

void OrderBook::set_market_price(Price market_price) {
  m_market_price = market_price;
}

void OrderBook::set_order_event_handler_ptr(OrderEventHandlerPtr handler_ptr) {
  m_order_event_handler_ptr = handler_ptr;
}

void OrderBook::set_trade_event_handler_ptr(TradeEventHandlerPtr handler_ptr) {
  m_trade_event_handler_ptr = handler_ptr;
}

bool OrderBook::add(const OrderPtr &order) {
  bool matched = false;

  // If the order is invalid, we reject.
  if (order->get_quantity() == 0) {
    m_callbacks.push_back(
        OrderCallback::reject(order, "quantity must be positive"));
  } else {
    // If the order is presented as a stop order and we're allowed to add it to
    // the market, we do so.
    if (order->get_stop_price() != 0 && add_stop_order(order)) {
      m_callbacks.push_back(OrderCallback::accept_stop(order));
    } else {
      // TODO: finish
    }
  }

  callback_now();
  return matched;
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
