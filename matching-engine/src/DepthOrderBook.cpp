#include "DepthOrderBook.hpp"
#include "DepthEventHandler.hpp"
#include "DepthLevel.hpp"
#include "DepthLevelConstants.hpp"
#include "Order.hpp"
#include "OrderBook.hpp"

DepthOrderBook::DepthOrderBook(Price initial_market_price,
                               const std::string &symbol)
    : OrderBook(initial_market_price, symbol), m_last_change(0),
      m_last_published_change(0), m_ignore_buy_fill_quantity(0),
      m_ignore_sell_fill_quantity(0) {}

void DepthOrderBook::on_accept(const OrderPtr &order) {
  OrderBook::on_accept(order);

  // If the order is a limit order.
  if (order->is_limit()) {
    // If the order is completely filled on acceptance, do not modify depth
    // unnecessarily.
    if (order->is_filled()) {
      // Don't tell depth about this order - it's going away immediately.
      // Instead tell depth about future fills to ignore.
      depth_ignore_fill_quantity(order->get_quantity(), order->is_buy());
    } else {
      // Add to bid or ask depth.
      depth_add_order(order->get_price(), order->get_quantity(),
                      order->is_buy());
    }
  }
}

void DepthOrderBook::on_accept_stop(const OrderPtr &order) {
  OrderBook::on_accept_stop(order);
}

void DepthOrderBook::on_trigger_stop(const OrderPtr &order) {
  OrderBook::on_trigger_stop(order);

  // Add to depth.
  depth_add_order(order->get_price(), order->get_quantity(), order->is_buy());
}

void DepthOrderBook::on_reject(const OrderPtr &order, const char *reason) {
  OrderBook::on_reject(order, reason);
}

void DepthOrderBook::on_fill(const OrderPtr &order,
                             const OrderPtr &matched_order,
                             Quantity quantity_filled, Price fill_price) {
  OrderBook::on_fill(order, matched_order, quantity_filled, fill_price);

  // If the matched order is a limit order.
  if (matched_order->is_limit()) {
    // Inform the depth.
    depth_fill_order(matched_order->get_price(), quantity_filled,
                     matched_order->is_filled(), matched_order->is_buy());
  }

  // If the inbound order is a limit order.
  if (order->is_limit()) {
    // Inform the depth.
    depth_fill_order(order->get_price(), quantity_filled, order->is_filled(),
                     order->is_buy());
  }
}

void DepthOrderBook::on_cancel(const OrderPtr &order) {
  OrderBook::on_cancel(order);

  // If the order is a limit order.
  if (order->is_limit()) {
    depth_close_order(order->get_price(), order->get_quantity(),
                      order->is_buy());
  }
}

void DepthOrderBook::on_cancel_stop(const OrderPtr &order) {
  OrderBook::on_cancel_stop(order);
}

void DepthOrderBook::on_cancel_reject(const OrderPtr &order,
                                      const char *reason) {
  OrderBook::on_cancel_reject(order, reason);
}

void DepthOrderBook::on_trade(const OrderBook *book, Quantity quantity,
                              Price price) {
  OrderBook::on_trade(book, quantity, price);
}

void DepthOrderBook::on_order_book_change() {
  OrderBook::on_order_book_change();

  // Book was updated, see if the depth we track was effected.
  if (changed_since_last_publish()) {
    if (m_depth_event_handler_ptr) {
      m_depth_event_handler_ptr->on_depth_change(this);
    }

    // Start tracking changes again...
    publish_last_change();
  }
}

const DepthLevels &DepthOrderBook::get_depth_levels() const { return m_levels; }

const DepthLevelsSection &DepthOrderBook::get_buy_depth_levels() const {
  return *reinterpret_cast<const DepthLevelsSection *>(&m_levels[0]);
}

const DepthLevelsSection &DepthOrderBook::get_sell_depth_levels() const {
  return *reinterpret_cast<const DepthLevelsSection *>(
      &m_levels[MARKET_DATA_PRICE_DEPTH]);
}

const DepthLevel &DepthOrderBook::get_first_buy_depth_level() const {
  return m_levels[0];
}

const DepthLevel &DepthOrderBook::get_last_buy_depth_level() const {
  return m_levels[MARKET_DATA_PRICE_DEPTH - 1];
}

const DepthLevel &DepthOrderBook::get_first_sell_depth_level() const {
  return m_levels[MARKET_DATA_PRICE_DEPTH];
}

const DepthLevel &DepthOrderBook::get_last_sell_depth_level() const {
  return m_levels[2 * MARKET_DATA_PRICE_DEPTH - 1];
}

ChangeId DepthOrderBook::get_last_change() const { return m_last_change; }

ChangeId DepthOrderBook::get_last_published_change() const {
  return m_last_published_change;
}

bool DepthOrderBook::changed_since_last_publish() const {
  return m_last_change > m_last_published_change;
}

DepthLevels &DepthOrderBook::get_depth_levels() { return m_levels; }

DepthLevelsSection &DepthOrderBook::get_buy_depth_levels() {
  return *reinterpret_cast<DepthLevelsSection *>(&m_levels[0]);
}

DepthLevelsSection &DepthOrderBook::get_sell_depth_levels() {
  return *reinterpret_cast<DepthLevelsSection *>(
      &m_levels[MARKET_DATA_PRICE_DEPTH]);
}

DepthLevel &DepthOrderBook::get_first_buy_depth_level() { return m_levels[0]; }

DepthLevel &DepthOrderBook::get_last_buy_depth_level() {
  return m_levels[MARKET_DATA_PRICE_DEPTH - 1];
}

DepthLevel &DepthOrderBook::get_first_sell_depth_level() {
  return m_levels[MARKET_DATA_PRICE_DEPTH];
}

DepthLevel &DepthOrderBook::get_last_sell_depth_level() {
  return m_levels[2 * MARKET_DATA_PRICE_DEPTH - 1];
}

void DepthOrderBook::depth_add_order(Price price, Quantity quantity,
                                     bool is_buy) {
  // We store a copy here to make sure this level's last change is only
  // determined by this external facing function.
  ChangeId last_change_copy = m_last_change;

  DepthLevel *level = find_depth_level(price, is_buy);

  // If level is nullptr, there's nothing else to do.
  if (!level)
    return;

  level->add_order(quantity);

  // If level is not visible, there's nothing else to do (levels that are not
  // visible are not marked as changed).
  if (level->get_is_excess())
    return;

  m_last_change = last_change_copy + 1;
  level->set_last_change(m_last_change);
}

void DepthOrderBook::depth_ignore_fill_quantity(Quantity quantity,
                                                bool is_buy) {
  if (is_buy) {
    if (m_ignore_buy_fill_quantity)
      throw std::runtime_error("DepthOrderBook::depth_ignore_fill_quantity - "
                               "Unexpected ignore_bid_fill_qty_");
    m_ignore_buy_fill_quantity = quantity;
  } else {
    if (m_ignore_sell_fill_quantity)
      throw std::runtime_error("DepthOrderBook::depth_ignore_fill_quantity - "
                               "Unexpected ignore_ask_fill_qty_");
    m_ignore_sell_fill_quantity = quantity;
  }
}

void DepthOrderBook::depth_fill_order(Price price, Quantity fill_quantity,
                                      bool filled, bool is_buy) {
  if (is_buy && m_ignore_buy_fill_quantity)
    m_ignore_buy_fill_quantity -= fill_quantity;
  else if ((!is_buy) && m_ignore_sell_fill_quantity)
    m_ignore_sell_fill_quantity -= fill_quantity;
  else if (filled)
    depth_close_order(price, fill_quantity, is_buy);
  else
    depth_change_quantity_order(price, fill_quantity, is_buy, false);
}

bool DepthOrderBook::depth_close_order(Price price, Quantity quantity_in_market,
                                       bool is_buy) {
  DepthLevel *level = find_depth_level(price, is_buy, false);

  // If level is nullptr, there's nothing else to do.
  if (!level)
    return false;

  bool level_empty = level->close_order(quantity_in_market);

  // If this is the last order on the level, erase the level, else just set the
  // last change.
  if (level_empty)
    erase_depth_level(*level, is_buy);
  else
    level->set_last_change(++m_last_change);

  return level_empty;
}

void DepthOrderBook::depth_change_quantity_order(Price price, Quantity delta,
                                                 bool is_buy,
                                                 bool is_increase) {
  DepthLevel *level = find_depth_level(price, is_buy, false);

  // If level is nullptr or the delta is 0, there's nothing else to do.
  if ((!level) || (!delta))
    return;

  if (is_increase)
    level->increase_quantity(delta);
  else
    level->decrease_quantity(delta);

  level->set_last_change(++m_last_change);
}

void DepthOrderBook::publish_last_change() {
  m_last_published_change = m_last_change;
}

DepthLevel *DepthOrderBook::find_depth_level(Price price, bool is_buy,
                                             bool should_create) {
  DepthLevelsSection relevant_section =
      is_buy ? get_buy_depth_levels() : get_sell_depth_levels();
  for (DepthLevel &level : relevant_section) {
    // Normal case, if we found our level, return.
    if (level.get_price() == price)
      return &level;

    // If the level is blank (0 price) and we can create the level, init level
    // and just use this.
    if (should_create && (level.get_price() == INVALID_LEVEL_PRICE)) {
      level.init(price, false);
      return &level;
    }

    // If is_buy == true and the current level price is below the price we're
    // searching for, we can guarantee that we cannot find the price within the
    // current buy levels. So, we add a new level right before this one (the
    // first level that executes this block is guaranteed to be the very next
    // level here).
    if (is_buy && should_create && (level.get_price() < price)) {
      insert_depth_level_before(level, price, is_buy);
      return &level;
    }

    // If is_buy == false and the current level price is above the price we're
    // searching for, we can guarantee that we cannot find the price within the
    // current sell levels. So, we add a new level right before this one (the
    // first level that executes this block is guaranteed to be the very next
    // level here).
    if ((!is_buy) && should_create && (level.get_price() > price)) {
      insert_depth_level_before(level, price, is_buy);
      return &level;
    }
  }

  // If we haven't returned yet, we haven't found the level yet and we still can
  // search. So we search through the excess buy/sell orders.
  if (is_buy) {
    // Search in excess buy levels.
    auto find_result = m_excess_buy_levels.find(price);

    // If found in excess levels, return location.
    if (find_result != m_excess_buy_levels.end())
      return &find_result->second;

    // Else not found, insert if one should be created.
    if (should_create) {
      auto [it, inserted] = m_excess_buy_levels.emplace(price, DepthLevel{});
      it->second.init(price, true);
      return &it->second;
    }
  } else {
    // Search in excess sell levels.
    auto find_result = m_excess_sell_levels.find(price);

    // If found in excess levels, return location.
    if (find_result != m_excess_sell_levels.end())
      return &find_result->second;

    // Else not found, insert if one should be created.
    if (should_create) {
      auto [it, inserted] = m_excess_sell_levels.emplace(price, DepthLevel{});
      it->second.init(price, true);
      return &it->second;
    }
  }

  return nullptr;
}

void DepthOrderBook::insert_depth_level_before(DepthLevel &level, Price price,
                                               bool is_buy) {
  const DepthLevel &last_side_level =
      is_buy ? get_last_buy_depth_level() : get_last_sell_depth_level();

  // If the last level has valid data, it will become an excess level (since
  // inserting one will write over it), so we should add it to the relevant
  // excess levels map.
  if (last_side_level.get_price() != INVALID_LEVEL_PRICE) {
    DepthLevel excess_level;
    excess_level.init(0, true);     // Will assign over price.
    excess_level = last_side_level; // Uses custom assignment operator function.

    if (is_buy)
      m_excess_buy_levels.emplace(last_side_level.get_price(), excess_level);
    else
      m_excess_sell_levels.emplace(last_side_level.get_price(), excess_level);
  }

  ++m_last_change;

  // Now we need to go through all the levels after the level param and copy it
  // into the level after it (copy index into index + 1), to make sure we don't
  // copy over levels before using them, we iterate in reverse order.
  DepthLevelsSection &relevant_section =
      is_buy ? get_buy_depth_levels() : get_sell_depth_levels();

  // Find reverse iterator to the provided level.
  // NOTE: We look at the element after level since apparently
  // make_reverse_iterator returns the reverse iterator to the element BEFORE
  // the input element pointer.
  auto target_iter = std::make_reverse_iterator(&level + 1);

  for (auto iter = relevant_section.rbegin() + 1; iter <= target_iter; ++iter) {
    DepthLevel &curr = *iter;
    DepthLevel &next = *(iter - 1);

    // Copy the current element onto the next one.
    next = curr; // Uses custom assignment operator function.

    // If the current element (element being copied) is valid (price != 0), then
    // we can update the last change of the next one (the one that we're copying
    // onto).
    if (curr.get_price() != INVALID_LEVEL_PRICE)
      next.set_last_change(m_last_change);
  }

  // Now we know for sure that the level param should be empty, so we refresh it
  // to be so before using it in the caller.
  level.init(price, false);
}

void DepthOrderBook::erase_depth_level(DepthLevel &level, bool is_buy) {
  // If the level being erased is from the excess, remove it from the relevant
  // excess map.
  if (level.get_is_excess()) {
    if (is_buy)
      m_excess_buy_levels.erase(level.get_price());
    else
      m_excess_sell_levels.erase(level.get_price());
    return;
  }

  ++m_last_change;

  // Else the level being erased is not excess, so we copy over from the worse
  // levels. NOTE: This logic is different from the insert_level_before copy
  // over logic because in THAT function we are shifting all of the levels after
  // the level we want to insert down the list. In THIS function, we are erasing
  // a level so we are shifting all the levels after it up.
  DepthLevelsSection &relevant_section =
      is_buy ? get_buy_depth_levels() : get_sell_depth_levels();

  // This logic sets the iterator related to the level param reference. It does
  // this by first computing the pointer offset between level and the first
  // element of the section, and advances the begin() iterator that many times
  // to get the resulting iterator.
  auto start_iter =
      std::next(relevant_section.begin(), &level - &relevant_section[0]);

  for (auto iter = start_iter; iter != relevant_section.end() - 1; ++iter) {
    DepthLevel &curr = *iter;
    DepthLevel &next = *(iter + 1);

    // If curr is a valid level (price != 0) OR if its exactly "level".
    // NOTE: This check is meaningful since the first instance of
    // INVALID_LEVEL_PRICE means that ALL the levels after this MUST have
    // INVALID_LEVEL_PRICE (the levels are in sorted order). This check just
    // allows us to skip extra work. We do need to have an extra check for the
    // level itself, since even if it's invalid, we still need to copy the stuff
    // after it (we're erasing it after all).
    if ((curr.get_price() != INVALID_LEVEL_PRICE) || (&curr == &level)) {
      // Copy to current level from one lower.
      curr = next;
      // Mark the current level as updated.
      curr.set_last_change(m_last_change);
    }
  }

  // Now, we've shifted all the levels, and we are left with the last level,
  // which can either represent "level" itself or is just a placeholder level we
  // can replace. ONLY in the case where the last level is "level" or it's
  // valid, we can copy over from the first level from excess. This is because
  // if the last level was invalid, all the rest would be too (there would be no
  // excess in that case), so we would save work.
  DepthLevel &last_side_level =
      is_buy ? get_last_buy_depth_level() : get_last_sell_depth_level();

  if ((&level == &last_side_level) ||
      (last_side_level.get_price() == INVALID_LEVEL_PRICE)) {
    if (is_buy) {
      auto best_buy = m_excess_buy_levels.begin();
      if (best_buy != m_excess_buy_levels.end()) {
        last_side_level = best_buy->second;
        m_excess_buy_levels.erase(best_buy);
      } else {
        // There is no excess, so we just set the last level to
        // INVALID_LEVEL_PRICE.
        last_side_level.init(INVALID_LEVEL_PRICE, false);
        last_side_level.set_last_change(m_last_change);
      }
    } else {
      auto best_sell = m_excess_sell_levels.begin();
      if (best_sell != m_excess_sell_levels.end()) {
        last_side_level = best_sell->second;
        m_excess_sell_levels.erase(best_sell);
      } else {
        // There is no excess, so we just set the last level to
        // INVALID_LEVEL_PRICE.
        last_side_level.init(INVALID_LEVEL_PRICE, false);
        last_side_level.set_last_change(m_last_change);
      }
    }
    last_side_level.set_last_change(m_last_change);
  }
}