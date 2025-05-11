#pragma once

#include "OrderTypes.hpp"

// This class represents one level of the depth order book.
class DepthLevel {
public:
  DepthLevel();

  // Initializes/refreshes already created depth level with price, is_excess.
  // Similar logic to constructor.
  void init(Price price, bool is_excess);

  DepthLevel &operator=(const DepthLevel &other);

  // Getter const functions.

  Price get_price() const;

  uint32_t get_order_count() const;

  Quantity get_total_quantity() const;

  bool get_is_excess() const;

  ChangeId get_last_change() const;

  bool changed_since(ChangeId last_published_change) const;

  // Action functions.

  void add_order(Quantity quantity);

  void increase_quantity(Quantity delta);

  void decrease_quantity(Quantity delta);

  void set(Price price, Quantity quantity, uint32_t order_count,
           ChangeId last_change = 0);

  // Return true if the level is now empty.
  bool close_order(Quantity closed_quantity);

  void set_last_change(ChangeId last_change);

private:
  Price m_price;
  uint32_t m_order_count;
  Quantity m_total_quantity;

  // This represents if the level this class is associated with is in the
  // "excess" side of the order book. In other words, this means that this side
  // has more orders than the other side at this price depth level.
  bool m_is_excess;

  ChangeId m_last_change;
};