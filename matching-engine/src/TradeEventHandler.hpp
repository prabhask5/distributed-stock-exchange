#pragma once

#include "OrderBookStockStatisticsTypes.hpp"
#include "OrderTypes.hpp"

class OrderBook; // From OrderBook.hpp.

// This class contains several callback functions that run logic on certain
// state changes for a trade within the order book.
class TradeEventHandler {
public:
  void on_trade(const OrderBook *order_book,
                OrderBookStockStatsMapPtr &order_book_stats_map_ptr,
                Quantity quantity, Cost cost);
};