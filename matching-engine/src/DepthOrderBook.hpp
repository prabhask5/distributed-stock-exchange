#pragma once

#include "DepthEventHandlerTypes.hpp"
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
  // The symbol is just the stock ticker symbol.
  DepthOrderBook(const std::string &symbol = "unknown",
                 Price initial_market_price = 0);

  // Getter const functions.

  // Gets the overall levels array, returned value is NOT mutable.
  const DepthLevels &get_depth_levels() const;

  // Gets the buy levels portion of the overall levels array, returned value is
  // NOT mutable.
  const DepthLevelsSection &get_buy_depth_levels() const;

  // Gets the sell levels portion of the overall levels array, returned value is
  // NOT mutable.
  const DepthLevelsSection &get_sell_depth_levels() const;

  // Gets the first buy level, returned value is NOT mutable.
  const DepthLevel &get_first_buy_depth_level() const;

  // Gets the last buy level, returned value is NOT mutable.
  const DepthLevel &get_last_buy_depth_level() const;

  // Gets the first sell level, returned value is NOT mutable.
  const DepthLevel &get_first_sell_depth_level() const;

  // Gets the last sell level, returned value is NOT mutable.
  const DepthLevel &get_last_sell_depth_level() const;

  ChangeId get_last_change() const;

  ChangeId get_last_published_change() const;

  bool changed_since_last_publish() const;

  // Getter mutable functions.

  // Gets the overall levels array, returned value is mutable.
  DepthLevels &get_depth_levels();

  // Gets the buy levels portion of the overall levels array, returned value is
  // mutable.
  DepthLevelsSection &get_buy_depth_levels();

  // Gets the sell levels portion of the overall levels array, returned value is
  // mutable.
  DepthLevelsSection &get_sell_depth_levels();

  // Gets the first buy level, returned value is mutable.
  DepthLevel &get_first_buy_depth_level();

  // Gets the last buy level, returned value is mutable.
  DepthLevel &get_last_buy_depth_level();

  // Gets the first sell level, returned value is mutable.
  DepthLevel &get_first_sell_depth_level();

  // Gets the last sell level, returned value is mutable.
  DepthLevel &get_last_sell_depth_level();

  // Setter functions.
  void set_depth_event_handler_ptr(const DepthEventHandlerPtr &handler_ptr);

protected:
  // Virtual functions for DepthOrderBook.hpp to implement additional
  // functionality onto.

  virtual void on_accept(const OrderPtr &order) override;

  virtual void on_accept_stop(const OrderPtr &order) override;

  virtual void on_trigger_stop(const OrderPtr &order) override;

  virtual void on_reject(const OrderPtr &order, const char *reason) override;

  virtual void on_fill(const OrderPtr &order, const OrderPtr &matched_order,
                       Quantity quantity_filled, Price fill_price) override;

  virtual void on_cancel(const OrderPtr &order) override;

  virtual void on_cancel_stop(const OrderPtr &order) override;

  virtual void on_cancel_reject(const OrderPtr &order,
                                const char *reason) override;

  virtual void on_trade(const OrderBook *book, Quantity quantity,
                        Price price) override;

  virtual void on_order_book_change() override;

private:
  // Action functions.

  void depth_add_order(Price price, Quantity quantity, bool is_buy);

  // Ignore future fill quantity for a side, due to a match at the accept time
  // of an order.
  void depth_ignore_fill_quantity(Quantity quantity, bool is_buy);

  void depth_fill_order(Price price, Quantity fill_quantity, bool filled,
                        bool is_buy);

  // Cancel or fill an order.
  bool depth_close_order(Price price, Quantity quantity_in_market, bool is_buy);

  void depth_change_quantity_order(Price price, Quantity delta, bool is_buy,
                                   bool is_increase);

  void publish_last_change();

  // Find the associated depth level for the price, if should_create = true,
  // creates the depth level if not found. Returns the pointer to the depth
  // level if found/created, nullptr if not.
  DepthLevel *find_depth_level(Price price, bool is_buy,
                               bool should_create = true);

  // Inserts a new level before the level pointer at the price, and shifts down
  // all the levels after it.
  void insert_depth_level_before(DepthLevel &level, Price price, bool is_buy);

  // Erases the level pointer.
  void erase_depth_level(DepthLevel &level, bool is_buy);

private:
  // This array represents the different levels of the depth order book.
  // We separate the buy and sell orders (the first SIZE elements are buy
  // orders, and the last SIZE elements are sell orders). This the depth order
  // book class should implement easy ways to get the "best" buy/sell order, and
  // iterate through the array.
  // All the buy/sell orders are sorted by price (decreasing order for buys and
  // increasing order for sells).
  //
  // Buy levels: m_levels[0] to m_levels[SIZE - 1]
  // Sell levels: m_levels[SIZE] to m_levels[2*SIZE - 1]
  DepthLevels m_levels;

  // Depth order book metadata.
  ChangeId m_last_change;
  ChangeId m_last_published_change;
  Quantity m_ignore_buy_fill_quantity; // Represents the amount of fill quantity
                                       // on the buy side to ignore.
  Quantity m_ignore_sell_fill_quantity; // Represents the amount of fill
                                        // quantity on the sell side to ignore.

  // Depth levels state management containers.
  BuyLevelMap m_excess_buy_levels;
  SellLevelMap m_excess_sell_levels;

  // Event handlers.
  DepthEventHandlerPtr m_depth_event_handler_ptr;
};