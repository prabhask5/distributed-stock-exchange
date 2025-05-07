#include "Market.hpp"

std::string Market::get_market_name() const { return m_market_name; }

OrderBookStockStatsMapPtr Market::get_order_book_stats_ptr() const {
  return m_order_book_stats_ptr;
}