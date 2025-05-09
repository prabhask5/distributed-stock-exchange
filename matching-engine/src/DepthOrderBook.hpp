#pragma once

#include "DepthLevel.hpp"
#include "DepthOrderBookTypes.hpp"
#include "OrderBook.hpp"

// This class represents a depth order book.
// A depth order book (also called a limit order book) is a type of order book
// that aggregates buy and sell orders based on price (a regular order book
// aggregates all buy/sell orders). This is specifically useful for limit orders
// since they can only be matched with their price ceiling/floor as the
// restriction. A single depth level of a depth order book represents all the
// buy/sell orders at a certain price, ordered based on price-time priority.
class DepthOrderBook : public OrderBook {
public:
  // Getter const functions.

  // Gets the overall levels array, returned value is NOT mutable.
  const DepthLevels &get_levels() const;

  // Gets the buy levels portion of the overall levels array, returned value is
  // NOT mutable.
  const DepthLevelsSection &get_buy_levels() const;

  // Gets the sell levels portion of the overall levels array, returned value is
  // NOT mutable.
  const DepthLevelsSection &get_sell_levels() const;

  // Gets the first buy level, returned value is NOT mutable.
  const DepthLevel &get_first_buy_level() const;

  // Gets the last buy level, returned value is NOT mutable.
  const DepthLevel &get_last_buy_level() const;

  // Gets the first sell level, returned value is NOT mutable.
  const DepthLevel &get_first_sell_level() const;

  // Gets the last sell level, returned value is NOT mutable.
  const DepthLevel &get_last_sell_level() const;

  // Getter mutable functions.

  // Gets the overall levels array, returned value is mutable.
  DepthLevels &get_levels();

  // Gets the buy levels portion of the overall levels array, returned value is
  // mutable.
  DepthLevelsSection &get_buy_levels();

  // Gets the sell levels portion of the overall levels array, returned value is
  // mutable.
  DepthLevelsSection &get_sell_levels();

  // Gets the first buy level, returned value is mutable.
  DepthLevel &get_first_buy_level();

  // Gets the last buy level, returned value is mutable.
  DepthLevel &get_last_buy_level();

  // Gets the first sell level, returned value is mutable.
  DepthLevel &get_first_sell_level();

  // Gets the last sell level, returned value is mutable.
  DepthLevel &get_last_sell_level();

private:
  // This array represents the different levels of the depth order book.
  // We separate the buy and sell orders (the first SIZE elements are buy
  // orders, and the last SIZE elements are sell orders). This the depth order
  // book class should implement easy ways to get the "best" buy/sell order, and
  // iterate through the array.
  //
  // Buy levels: m_levels[0] to m_levels[SIZE - 1]
  // Sell levels: m_levels[SIZE] to m_levels[2*SIZE - 1]
  DepthLevels m_levels;
};