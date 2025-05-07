#pragma once

#include "Order.hpp"
#include <string>

// This class represents one stock in the matching engine.
// Orders come in and get matched with other orders in this order book.
// Every matching engine will have multiple order books.
// Each order book stores the current market price and the orders that are
// currently being executed.
class OrderBook {
public:
  // The symbol is just the stock ticker symbol.
  OrderBook(const std::string &symbol);

  // Getter const functions.

  const std::string &get_symbol() const;

  Price get_market_price() const;

  // Setter functions.

  void set_symbol(const std::string &symbol);

  void set_market_price(Price market_price);
};