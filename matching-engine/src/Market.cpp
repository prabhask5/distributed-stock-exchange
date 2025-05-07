#include "Market.hpp"

Market::Market(MarketDataPublisherQueuePtr market_data_publisher_queue_ptr)
    : m_market_data_publisher_queue_ptr(market_data_publisher_queue_ptr) {}

std::string Market::get_market_name() const { return m_market_name; }

OrderBookStockStatsMapPtr Market::get_order_book_stats_ptr() const {
  return m_order_book_stats_ptr;
}

MarketDataPublisherQueuePtr
Market::get_market_data_publisher_queue_ptr() const {
  return m_market_data_publisher_queue_ptr;
}