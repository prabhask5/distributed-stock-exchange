#pragma once

#include <cstdint>

using Price = uint64_t;
using Cost = uint64_t;

namespace {
// The price of a market order is 0, since it's determined by the price of the
// matching order.
const Price MARKET_ORDER_PRICE(0);

// Defines the delta in price of an price-unchanged order, 0.
const Price PRICE_UNCHANGED(0);
} // namespace

// This class represents a price of an order specifically. This is more complex
// than a normal price since comparing prices (finding the best match) has
// different logic depending on whether the order is a buy or sell order.
class OrderPrice {
public:
  OrderPrice(Price price, bool is_buy);

  // Getter const functions.

  Price get_price() const;

  bool is_buy() const;

  bool is_market_order() const;

  // Check whether these order prices can be matched.
  // An order price can be matched if:
  // - buy: the prices are the same, if the price is MARKET_ORDER_PRICE (order
  // is market order), or if the price is greater than the corresponding sell
  // order.
  // - sell: the prices are the same, if the other order is MARKET_ORDER_PRICE
  // (order is market order), or if the price is less than the corresponding buy
  // order.
  //
  // NOTE: we should check that both orders are not the same type before calling
  // this function!
  bool matches(const OrderPrice &other) const;

  // Used to sort orders on the multimap.
  // Returns true iff:
  // - Price is MARKET_ORDER_PRICE (order is market order) and other is not.
  // - If buy and other price is less.
  // - If sell and other price is more.
  // NOTE: Assumes both are on same side.
  bool operator<(const OrderPrice &other) const;

  // Just checks if the inner prices are equal.
  // NOTE: Assumes both are on same side.
  bool operator==(const OrderPrice &other) const;

  // Just checks if the inner prices are not equal.
  // NOTE: Assumes both are on same side.
  bool operator!=(const OrderPrice &other) const;

  // Used to sort orders on the multimap.
  // Returns true iff:
  // - Price is NOT MARKET_ORDER_PRICE and other is.
  // - If buy and other price is more.
  // - If sell and other price is less.
  // NOTE: Assumes both are on same side.
  bool operator>(const OrderPrice &other) const;

private:
  Price m_price;
  bool m_is_buy;
};