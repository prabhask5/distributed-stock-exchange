#pragma once

#include <map>
#include <memory>
#include <string>

struct OrderBookStockStatistics; // From OrderBookStockStatistics.hpp.
using OrderBookStockStatisticsPtr = std::shared_ptr<OrderBookStockStatistics>;

// This is used to map each order book's stock symbol name to its own stats map.
using OrderBookStockStatsMap =
    std::map<std::string, OrderBookStockStatisticsPtr>;
using OrderBookStockStatsMapPtr = std::shared_ptr<OrderBookStockStatsMap>;