#pragma once

#include <memory>

using Price = uint64_t;
using Cost = uint64_t;
using Quantity = uint64_t;
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

class Order; // From Order.hpp.
using OrderPtr = std::shared_ptr<Order>;