#include "DepthLevel.hpp"

Price DepthLevel::get_price() const { return m_price; }

uint32_t DepthLevel::get_order_count() const { return m_order_count; }

Quantity DepthLevel::get_aggregate_quantity() const {
  return m_aggregate_quantity;
}

bool DepthLevel::get_is_excess() const { return m_is_excess; }

ChangeId DepthLevel::get_last_change() const { return m_last_change; }
