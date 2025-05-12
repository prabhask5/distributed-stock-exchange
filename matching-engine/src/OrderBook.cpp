#define NOMINMAX

#include "OrderBook.hpp"
#include "Order.hpp"
#include "OrderBookConstants.hpp"
#include "OrderEventHandler.hpp"
#include "OrderPrice.hpp"
#include "TradeEventHandler.hpp"
#include <map>
#include <string>

OrderBook::OrderBook(const std::string &symbol, Price initial_market_price)
    : m_symbol(symbol), m_order_event_handler_ptr(nullptr),
      m_trade_event_handler_ptr(nullptr), m_market_price(initial_market_price) {
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
  Price old_market_price = m_market_price;
  m_market_price = market_price;

  // The reason we use an "else if" here is to maintain the invariant that only
  // ONE side of stop orders can be activated at one time. Since both have the
  // same condition in one case, there is a change this is not true if both are
  // "if".
  if ((m_market_price > old_market_price) ||
      (old_market_price == MARKET_ORDER_PRICE)) {
    // The price has gone up: check if any stop buy orders activated.
    check_stop_orders(true, m_market_price, m_buy_stop_orders);
  } else if ((m_market_price < old_market_price) ||
             (old_market_price == MARKET_ORDER_PRICE)) {
    // The price has gone down: check if any stop sell orders activated.
    check_stop_orders(false, m_market_price, m_sell_stop_orders);
  }
}

void OrderBook::set_order_event_handler_ptr(
    const OrderEventHandlerPtr &handler_ptr) {
  m_order_event_handler_ptr = handler_ptr;
}

void OrderBook::set_trade_event_handler_ptr(
    const TradeEventHandlerPtr &handler_ptr) {
  m_trade_event_handler_ptr = handler_ptr;
}

bool OrderBook::add(const OrderPtr &order) {
  // If the order is invalid, we reject.
  if (order->get_quantity() == 0) {
    on_reject(order, "Order is invalid, quantity must be positive.");
    return false;
  }

  bool matched = false;

  // If the order is presented as a stop order and we're allowed to add it to
  // the market, we do so.
  if ((order->get_stop_price() != 0) && add_stop_order(order)) {
    on_accept_stop(order);
  } else {
    on_accept(order);
    matched = add_order(order);

    // Cancel any immediate or cancel order if it is not completely filled.
    if (order->is_immediate_or_cancel() && (!order->is_filled())) {
      on_cancel(order);
    }
  }

  // If adding this order triggered any stops, they are in pending orders. We'll
  // handle those now.
  while (!m_pending_orders.empty()) {
    add_pending_orders();
  }
  on_order_book_change();

  return matched;
}

void OrderBook::cancel(const OrderPtr &order) {
  bool found = false;
  typename OrderMap::iterator iter;

  // If order is not currently stop, search in regular maps.
  if (!order->is_stop()) {
    OrderMap &relevant_order_map =
        order->is_buy() ? m_buy_orders : m_sell_orders;
    found = find_in_market(order, iter);
    if (found) {
      relevant_order_map.erase(iter);
    }
  } else {
    OrderMap &relevant_stop_order_map =
        order->is_buy() ? m_buy_stop_orders : m_sell_stop_orders;
    found = find_in_stop_orders(order, iter);
    if (found) {
      relevant_stop_order_map.erase(iter);
    }
  }

  if (found) {
    on_cancel(order);
    on_order_book_change();
  } else {
    on_cancel_reject(order, "Order not found.");
  }
}

bool OrderBook::add_order(const OrderPtr &order) {
  DeferredMatchList deferred_aon_orders;
  OrderMap &same_side_order_map =
      order->is_buy() ? m_buy_orders : m_sell_orders;
  OrderMap &other_side_order_map =
      order->is_buy() ? m_sell_orders : m_buy_orders;

  // Make sure we're always matching an order with the OTHER SIDE orders!
  bool matched = match_order(order, other_side_order_map, deferred_aon_orders);

  // If the order is not completely filled after initial matching and is NOT an
  // immediate or cancel order, we can insert into the relevant order map and
  // rerun matching for deferred all or nothing orders.
  if ((!order->is_filled()) && (!order->is_immediate_or_cancel())) {
    same_side_order_map.emplace(order->get_order_price(), order);

    // See if adding this order satisfies deferred AON orders from the OTHER
    // side.
    matched |= check_deferred_aon_orders(
        deferred_aon_orders, other_side_order_map, same_side_order_map);
  }

  return matched;
}

bool OrderBook::match_order(const OrderPtr &inbound_order,
                            OrderMap &other_side_order_map,
                            DeferredMatchList &deferred_aon_orders) {
  if (inbound_order->is_all_or_none())
    return match_aon_order(inbound_order, other_side_order_map,
                           deferred_aon_orders);
  else
    return match_regular_order(inbound_order, other_side_order_map,
                               deferred_aon_orders);
}

bool OrderBook::match_regular_order(const OrderPtr &inbound_order,
                                    OrderMap &other_side_order_map,
                                    DeferredMatchList &deferred_aon_orders) {
  // while (there are incoming orders) && (inbound order not filled).
  //   current = next incoming order.
  //   if current does not match with inbound: break.
  //   if current is AON:.
  //      if incoming satisfies AON: trade.
  //      else: add AON to deferred.
  //   if current is reg: trade.
  // loop.

  bool matched = false;
  auto iter = other_side_order_map.begin();

  while ((iter != other_side_order_map.end()) &&
         (!inbound_order->is_filled())) {
    auto entry = iter++;

    const OrderPrice &current_order_price = entry->first;
    const OrderPtr &current_order = entry->second;

    // If this current entry does not match with the inbound order, nothing
    // after it will; we can break early.
    if (!current_order_price.matches(inbound_order->get_order_price()))
      break;

    // Case 1: Matching current order is an AON order.
    if (current_order->is_all_or_none()) {
      // Case 1a: We can completely fill the current AON order with the inbound
      // order -> trade!
      if (current_order->get_quantity_in_market() <=
          inbound_order->get_quantity_in_market()) {
        Quantity traded_quantity = create_trade(inbound_order, current_order);

        // NOTE: Here are are assuming that the traded quantity MUST be ==
        // current order quantity AND that create_trade handles updating order
        // states.
        if (traded_quantity > 0) {
          matched = true;
          other_side_order_map.erase(entry);
        }
      } else {
        // Case 1b: We cannot completely fill the current AON order, so we add
        // it to the deferred AON list and move on.
        deferred_aon_orders.push_back(entry);
      }
    } else {
      // Case 2: Matching current order is a regular order: both are non-AON, so
      // we can just trade freely.

      // NOTE: Here are are assuming that create_trade handles updating order
      // states.
      Quantity traded_quantity = create_trade(inbound_order, current_order);

      if (traded_quantity > 0) {
        matched = true;
        if (current_order->is_filled())
          other_side_order_map.erase(entry);
      }
    }
  }

  return matched;
}

bool OrderBook::match_aon_order(const OrderPtr &inbound_order,
                                OrderMap &other_side_order_map,
                                DeferredMatchList &deferred_aon_orders) {
  // while (there are incoming orders) && (inbound order not filled).
  //   store total quantity from deferred matches.
  //   current = next incoming order.
  //   if current does not match with inbound: break.
  //   if current is AON:.
  //      if inbound satisfies AON AND making trades between inbound, current,
  //      and some deferred matches statisfies inbound: try to trade.
  //      else if inbound satisfies AON: add AON to deferred, add to total
  //      deferred quantity.
  //      else: add AON to deferred.
  //   if current is reg:.
  //      if making trades between inbound, current, and some deferred matches
  //      statisfies inbound: try to trade.
  //      else: add current to deferred, add to total deferred quantity.
  // loop.

  bool matched = false;
  auto iter = other_side_order_map.begin();
  Quantity quantity_deferred = 0;
  DeferredMatchList deferred_matched_orders;
  Quantity inbound_quantity_in_market = inbound_order->get_quantity_in_market();

  while ((iter != other_side_order_map.end()) &&
         (!inbound_order->is_filled())) {
    auto entry = iter++;

    const OrderPrice &current_order_price = entry->first;
    const OrderPtr &current_order = entry->second;
    Quantity current_quantity_in_market =
        current_order->get_quantity_in_market();

    // If this current entry does not match with the inbound order, nothing
    // after it will; we can break early.
    if (!current_order_price.matches(inbound_order->get_order_price()))
      break;

    // Case 1: Matching current order is an AON order.
    if (current_order->is_all_or_none()) {
      // Case 1a: We can completely fill the current AON order with the inbound
      // AON order AND making trades between inbound, current, and some deferred
      // matches statisfies inbound AON order -> try to trade!
      if ((current_quantity_in_market <= inbound_quantity_in_market) &&
          (inbound_quantity_in_market <=
           (current_quantity_in_market + quantity_deferred))) {
        // Try to make deferred trades (if any) before making the treade with
        // the current AON order. We need the exact quantity needed to fill the
        // inbound AON order to actually make the trade with the current AON
        // order.
        Quantity exact_quantity_needed =
            inbound_quantity_in_market - current_quantity_in_market;
        if (exact_quantity_needed ==
            try_create_deferred_trades(
                inbound_order, deferred_matched_orders, exact_quantity_needed,
                exact_quantity_needed, other_side_order_map)) {
          // We now know for sure that we can satisfy the inbound AON order with
          // a combination of the current AON order and some deferred matched
          // orders, we can finally execute the trade with the current AON
          // order. NOTE: Here are are assuming that create_trade handles
          // updating order states.
          Quantity traded_quantity = create_trade(inbound_order, current_order);

          if (traded_quantity > 0) {
            matched = true;
            other_side_order_map.erase(entry);
          }
        }
      } else if (current_quantity_in_market <= inbound_quantity_in_market) {
        // Case 1b: We can only completely fill the current AON order with the
        // inbound AON order, so we add the current AON order to the deferred
        // matched orders and add the quantity to the total deferred quantity,
        // and move on.
        quantity_deferred += current_quantity_in_market;
        deferred_matched_orders.push_back(entry);
      } else {
        // Case 1c: We cannot completely fill the current AON order, so we add
        // it to the deferred matched orders and move on.
        deferred_matched_orders.push_back(entry);
      }
    } else {
      // Case 2: Matching current order is a regular order, so we just need to
      // satisfy the inbound AON order.

      // Case 2a: We can completely fill the inbound AON order by making trades
      // with the current order, and some deferred matches -> try to trade!
      if (inbound_quantity_in_market <=
          current_quantity_in_market + quantity_deferred) {
        // We want to trade as much as possible, so the max will just be the
        // inbound open quantity, the min can either be the difference needed or
        // 0 if current order handles that.
        Quantity min_quantity =
            (inbound_quantity_in_market > current_quantity_in_market)
                ? (inbound_quantity_in_market - current_quantity_in_market)
                : 0;
        Quantity traded_quantity_from_deferred = try_create_deferred_trades(
            inbound_order, deferred_matched_orders, inbound_quantity_in_market,
            min_quantity, other_side_order_map);

        if (inbound_quantity_in_market <=
            current_quantity_in_market + traded_quantity_from_deferred) {
          // We now know for sure that we can satisfy the inbound AON order with
          // a combination of the current AON order and some deferred matched
          // orders, we can finally execute the trade with the current AON
          // order. NOTE: Here are are assuming that create_trade handles
          // updating order states.
          Quantity traded_quantity = create_trade(inbound_order, current_order);

          if (traded_quantity > 0) {
            matched = true;
            if (current_order->is_filled())
              other_side_order_map.erase(entry);
          }
        }
      } else {
        // Case 2b: We don't have enough to satisfy the inbound AON order yet,
        // so we add current to deferred matches and add the quantity to the
        // total deferred quantity, and move on.
        quantity_deferred += current_quantity_in_market;
        deferred_matched_orders.push_back(entry);
      }
    }
  }

  return matched;
}

Quantity OrderBook::try_create_deferred_trades(
    const OrderPtr &inbound_order, DeferredMatchList &deferred_matches,
    Quantity max_quantity, Quantity min_quantity, OrderMap &current_orders) {
  // Since we don't know which trades need to go through to get the quantity we
  // want, we'll make two passes of the deferred matches list. Pass one will
  // dial in how much quantity will be filled from each order (stored in the
  // vector below). Pass two will actually execute the trades themselves.
  std::vector<int> order_found_quantity(deferred_matches.size(), 0);
  Quantity total_found_quantity = 0;
  size_t ofq_index = 0;

  // Pass one.
  for (auto entry : deferred_matches) {
    // Break whenever we're done with greedy.
    if (total_found_quantity >= max_quantity)
      break;

    const OrderPtr &current_order = entry->second;
    Quantity quantity_used = current_order->get_quantity_in_market();

    // Check whether using the current used quantity will put us over max
    // quantity.
    if (total_found_quantity + quantity_used > max_quantity) {
      // If the order is an AON order, we have to use 0 (can't partially fill).
      if (current_order->is_all_or_none())
        quantity_used = 0;
      else {
        // Else, we can partially fill, so we do so.
        // NOTE: This is guarenteed to be <= order->get_quantity_in_market()
        // since total_found_quantity + quantity_used > max_quantity implies
        // quantity_used > max_quantity - total_found_quantity.
        quantity_used = max_quantity - total_found_quantity;
      }
    }

    total_found_quantity += quantity_used;
    order_found_quantity[ofq_index++] = quantity_used;
  }

  Quantity traded_quantity = 0;
  ofq_index = 0;

  // Check whether the total_found_quantity exists within the max and min
  // quantity range; we can only execute the trades if so.
  if ((total_found_quantity >= min_quantity) &&
      (total_found_quantity <= max_quantity)) {
    // We now know for sure this works, so let's execute those trades.
    // Pass two.
    for (auto entry : deferred_matches) {
      if (traded_quantity >= total_found_quantity)
        break;

      const OrderPtr &current_order = entry->second;

      // NOTE: Here are are assuming that create_trade handles updating order
      // states.
      traded_quantity += create_trade(inbound_order, current_order,
                                      order_found_quantity[ofq_index++]);
      if (current_order->is_filled())
        current_orders.erase(entry);
    }
  }

  return traded_quantity;
}

bool OrderBook::check_deferred_aon_orders(DeferredMatchList &aon_orders,
                                          OrderMap &deferred_order_map,
                                          OrderMap &other_side_order_map) {
  bool result = false;
  DeferredMatchList ignored_aon_orders;

  for (auto entry : aon_orders) {
    const OrderPrice &current_order_price = entry->first;
    const OrderPtr &order = entry->second;

    // We have a previously deferred AON order; we'll try to match it with the
    // orders on the other side.
    result |= match_order(order, other_side_order_map, ignored_aon_orders);

    // If matching the other filled it entirely, we don't need it in the order
    // map anymore.
    if (order->is_filled())
      deferred_order_map.erase(entry);
  }

  return result;
}

Quantity OrderBook::create_trade(const OrderPtr &inbound_order,
                                 const OrderPtr &other_order,
                                 Quantity max_quantity) {
  // The cross price (the price of the trade) is initially determined by the
  // price of the passive order, or the order that is currently sitting in the
  // order book. This is "other order" so we take that as the cross price
  // initially.
  Price cross_price = other_order->get_price();

  // In the case that the passive order price is 0 (it's a market order), we
  // then take the price of the inbound order.
  if (cross_price == MARKET_ORDER_PRICE)
    cross_price = inbound_order->get_price();

  // In the case that BOTH orders are market orders, we just take the price of
  // the stock currently.
  if (cross_price == MARKET_ORDER_PRICE)
    cross_price = m_market_price;

  // If it's still zero, it's over just abort.
  if (cross_price == MARKET_ORDER_PRICE)
    return 0;

  Quantity quantity_filled =
      std::min(max_quantity, std::min(inbound_order->get_quantity_in_market(),
                                      other_order->get_quantity_in_market()));

  if (quantity_filled > 0) {
    set_market_price(cross_price);
    on_fill(inbound_order, other_order, quantity_filled, cross_price);
  }

  return quantity_filled;
}

bool OrderBook::find_in_market(const OrderPtr &order,
                               typename OrderMap::iterator &result) {
  OrderMap &relevant_order_map = order->is_buy() ? m_buy_orders : m_sell_orders;

  for (result = relevant_order_map.find(order->get_order_price());
       result != relevant_order_map.end(); ++result) {
    if (result->second == order)
      return true;

    // This represents the first order that is over the key order price; since
    // the map is sorted all the others will be too.
    if (order->get_order_price() < result->first) {
      result = relevant_order_map.end();
      return false;
    }
  }

  return false;
}

bool OrderBook::find_in_stop_orders(const OrderPtr &order,
                                    typename OrderMap::iterator &result) {
  OrderMap &relevant_stop_order_map =
      order->is_buy() ? m_buy_stop_orders : m_sell_stop_orders;

  for (result = relevant_stop_order_map.find(order->get_stop_order_price());
       result != relevant_stop_order_map.end(); ++result) {
    if (result->second == order)
      return true;

    // This represents the first stop order that is over the key order price;
    // since the map is sorted all the others will be too.
    if (order->get_order_price() < result->first) {
      result = relevant_stop_order_map.end();
      return false;
    }
  }

  return false;
}

bool OrderBook::add_stop_order(const OrderPtr &order) {
  // Check if the order's actually a stop order.
  if (!order->is_stop())
    return false;

  // Check if this stop order is immediately triggered with the current market
  // price, if so it's a normal order now.
  bool is_triggered = order->get_stop_order_price() > m_market_price;
  if (is_triggered)
    return false;

  // We now know for sure this is a stop order that is not triggered yet, add it
  // to the right stop order map.
  OrderMap &relevant_stop_order_map =
      order->is_buy() ? m_buy_stop_orders : m_sell_stop_orders;
  relevant_stop_order_map.emplace(order->get_stop_order_price(), order);

  return true;
}

void OrderBook::check_stop_orders(bool is_buy, Price price,
                                  OrderMap &stop_order_map) {
  OrderPrice bound(is_buy, price);

  for (auto iter = stop_order_map.begin(); iter != stop_order_map.end();) {
    // This represents the first stop order that is under the bound; this marks
    // the first stop order not triggered, and since the map is sorted all the
    // others will be too.
    if (bound > iter->first)
      break;

    // Move from stop order map to pending orders to wait to be executed.
    m_pending_orders.push_back(std::move(iter->second));
    iter = stop_order_map.erase(iter);
  }
}

void OrderBook::add_pending_orders() {
  OrderVec pending;
  pending.swap(m_pending_orders);

  for (OrderPtr order : pending) {
    add_order(order);
    on_trigger_stop(order);
  }
}

void OrderBook::on_accept(const OrderPtr &order) {
  if (m_order_event_handler_ptr) {
    m_order_event_handler_ptr->on_order_accept(order);
  }
}

void OrderBook::on_accept_stop(const OrderPtr &order) {
  if (m_order_event_handler_ptr) {
    m_order_event_handler_ptr->on_order_accept(order);
  }
}

void OrderBook::on_trigger_stop(const OrderPtr &order) {
  if (m_order_event_handler_ptr) {
    m_order_event_handler_ptr->on_stop_order_triggered(order);
  }
}

void OrderBook::on_reject(const OrderPtr &order, const char *reason) {
  if (m_order_event_handler_ptr) {
    m_order_event_handler_ptr->on_order_reject(order, reason);
  }
}

void OrderBook::on_fill(const OrderPtr &order, const OrderPtr &matched_order,
                        Quantity quantity_filled, Price fill_price) {
  if (m_order_event_handler_ptr) {
    m_order_event_handler_ptr->on_order_fill(order, matched_order,
                                             quantity_filled, fill_price);
  }
  on_trade(this, quantity_filled, fill_price);
}

void OrderBook::on_cancel(const OrderPtr &order) {
  if (m_order_event_handler_ptr) {
    m_order_event_handler_ptr->on_order_cancelled(order);
  }
}

void OrderBook::on_cancel_stop(const OrderPtr &order) {
  if (m_order_event_handler_ptr) {
    m_order_event_handler_ptr->on_order_cancelled(order);
  }
}

void OrderBook::on_cancel_reject(const OrderPtr &order, const char *reason) {
  if (m_order_event_handler_ptr) {
    m_order_event_handler_ptr->on_order_cancel_reject(order, reason);
  }
}

void OrderBook::on_trade(const OrderBook *book, Quantity quantity,
                         Price price) {
  if (m_trade_event_handler_ptr) {
    m_trade_event_handler_ptr->on_trade(book, quantity, price);
  }
}

void OrderBook::on_order_book_change() {}