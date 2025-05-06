#pragma once

#include <algorithm>
#include <memory>

// This struct tracks the stock statistics tied to an order book
struct OrderBookStockStatistics {
  // The opening price of a stock, which is the price of the first matched trade
  // of the day/time-window
  Price open;

  // Lowest trade price so far
  Price low;

  // Highest trade price so far
  Price high;

  // The total number of shared traded
  Quantity volume;
};

using OrderBookStockStatisticsPtr = std::shared_ptr<OrderBookStockStatistics>;
using OrderBookStockStatsMap =
    std::map<std::string, OrderBookStockStatisticsPtr>;
using OrderBookStockStatsMapPtr = std::shared_ptr<OrderBookStockStatsMap>;

inline void
update_symbol_stats(OrderBookStockStatsMapPtr &order_book_stats_map_ptr,
                    const OrderBook *order_book, Quantity quantity,
                    Price price) {
  auto symbol_stats = order_book_stats_map_ptr->find(order_book->get_symbol());

  if (symbol_stats == order_book_stats_map_ptr->end()) {
    OrderBookStockStatisticsPtr stats_ptr =
        std::make_shared<OrderBookStockStatistics>();

    stats_ptr->volume = quantity;
    stats_ptr->open = stats_ptr->low = stats_ptr->high = price;

    order_book_stats_map_ptr->emplace(order_book->get_symbol(), stats_ptr);
  } else {
    symbol_stats->second->volume += quantity;

    symbol_stats->second->low = std::min(symbol_stats->second->low, price);
    symbol_stats->second->high = std::max(symbol_stats->second->high, price);
  }
}