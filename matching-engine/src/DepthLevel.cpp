#include "DepthLevel.hpp"
#include "DepthLevelConstants.hpp"

DepthLevel::DepthLevel()
    : m_price(INVALID_LEVEL_PRICE), m_order_count(0), m_total_quantity(0) {}

void DepthLevel::init(Price price, bool is_excess) {
  m_price = price;
  m_order_count = 0;
  m_total_quantity = 0;
  m_is_excess = is_excess;
}

DepthLevel &DepthLevel::operator=(const DepthLevel &other) {
  m_price = other.get_price();
  m_order_count = other.get_order_count();
  m_total_quantity = other.get_total_quantity();

  // This condition checks that the other depth level has a price of greater
  // than 0, which I think represents that this depth level was changed before.
  // This is why we copy the last change here.
  if (other.get_price() != INVALID_LEVEL_PRICE) {
    m_last_change = other.get_last_change();
  }

  // Do not copy m_is_excess.

  return *this;
}

Price DepthLevel::get_price() const { return m_price; }

uint32_t DepthLevel::get_order_count() const { return m_order_count; }

Quantity DepthLevel::get_total_quantity() const { return m_total_quantity; }

bool DepthLevel::get_is_excess() const { return m_is_excess; }

ChangeId DepthLevel::get_last_change() const { return m_last_change; }

bool DepthLevel::changed_since(ChangeId last_published_change) const {
  return m_last_change > last_published_change;
}

void DepthLevel::add_order(Quantity quantity) {
  ++m_order_count;
  m_total_quantity += quantity;
}

void DepthLevel::increase_quantity(Quantity delta) {
  m_total_quantity += delta;
}

void DepthLevel::decrease_quantity(Quantity delta) {
  m_total_quantity -= delta;
}

void DepthLevel::set(Price price, Quantity quantity, uint32_t order_count,
                     ChangeId last_change) {
  m_price = price;
  m_total_quantity = quantity;
  m_order_count = order_count;
  m_last_change = last_change;
}

bool DepthLevel::close_order(Quantity closed_quantity) {
  // If the order count is already 0, something has gone wrong.
  if (m_order_count == 0)
    throw std::runtime_error(
        "DepthLevel::close_order - order count is unexpectedly 0.");

  // If the closed_quantity is greater than the total quantity, something has
  // gone wrong.
  if (closed_quantity > m_total_quantity)
    throw std::runtime_error("DepthLevel::close_order - total quantity is "
                             "unexpectedly less than closed_quantity.");

  --m_order_count;
  m_total_quantity -= closed_quantity;

  return m_total_quantity == 0;
}

void DepthLevel::set_last_change(ChangeId last_change) {
  m_last_change = last_change;
}