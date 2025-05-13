#pragma once

#include "MarketDataUpdateTypes.hpp"
#include "OrderBookStockStatisticsTypes.hpp"

class Market;         // From Market.hpp;
class DepthOrderBook; // From DepthOrderBook.hpp;

// This class contains several callback functions that run logic on certain
// state changes related to a depth order book.
class DepthEventHandler {
public:
  DepthEventHandler(
      const OrderBookStockStatsMapPtr &order_book_stats_map_ptr,
      const MarketDataPublisherQueuePtr &market_data_publisher_queue_ptr,
      const std::string &market_name);

  void on_depth_change(const DepthOrderBook *depth_order_book);

private:
  OrderBookStockStatsMapPtr m_order_book_stats_map_ptr;
  MarketDataPublisherQueuePtr m_market_data_publisher_queue_ptr;
  std::string m_market_name;
};