#pragma once

#include "Order.hpp"

// These are constants used with the depth order book logic.
namespace {
// Represents an invalid price for a depth in the order book.
// This is 0 since the price of a stock should never be zero.
const Price INVALID_LEVEL_PRICE(0);

// Represents the price of a buy market order in the depth order book.
// Since this is aggregated by price for level orders and since each buy market
// order should be matched with the best (highest) sell orders available, this
// is equal to the max quantity value.
const Price MARKET_ORDER_BUY_SORT_PRICE(QUANTITY_MAX);

// Represents the price of a sell market order in the depth order book.
// Since this is aggregated by price for level orders and since each sell market
// order should be matched with the best (lowest) buy orders available, this is
// equal to 0.
const Price MARKET_ORDER_SELL_SORT_PRICE(0);
} // namespace

// This class represents one level of the depth order book.
class DepthLevel {
public:
  // Getter const functions.

  Price get_price() const;

  uint32_t get_order_count() const;

  Quantity get_aggregate_quantity() const;

  bool get_is_excess() const;

  ChangeId get_last_change() const;

private:
  Price m_price;
  uint32_t m_order_count;
  Quantity m_aggregate_quantity;
  bool m_is_excess;
  ChangeId m_last_change;
};