#pragma once

#include "OrderTypes.hpp"

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