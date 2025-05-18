#pragma once

#include <MarketDataIncrementalRefresh.hpp>

// Represents a market data incremental update to send to the data service and
// expose to the user via the FIX gateway.
struct MarketDataUpdate {
  // Symbol of stock ticker of the order book this update is coming from.
  std::string symbol;

  // Data sent.
  DistributedStockExchange_MarketDataIncrementalRefresh::
      MarketDataIncrementalRefresh refreshData;
};