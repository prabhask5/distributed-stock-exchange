#pragma once

#include "OrderTypes.hpp"

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