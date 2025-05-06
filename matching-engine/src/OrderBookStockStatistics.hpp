#pragma once

#include <memory>

// This struct tracks the stock statistics tied to an order book
struct OrderBookStockStatistics {
  // The opening price of a stock, which is the price of the first matched trade
  // of the day/time-window
  long open;

  // Lowest trade price so far
  long low;

  // Highest trade price so far
  long high;

  // The total number of shared traded
  long volume;
};

using OrderBookStockStatisticsPtr = std::shared_ptr<OrderBookStockStatistics>;