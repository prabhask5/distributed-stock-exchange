#pragma once

#include "Order.hpp"
#include <string>

class OrderBook {
public:
  // The symbol is just the stock ticker symbol
  OrderBook(const std::string &symbol);

  // Getter const functions

  const std::string &get_symbol() const;

  Price get_market_price() const;

  // Setter functions

  void set_symbol(const std::string &symbol);

  void set_market_price(Price market_price);
};