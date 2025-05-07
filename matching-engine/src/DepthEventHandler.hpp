#pragma once

#include "DepthOrderBook.hpp"
#include "Market.hpp"
#include "MarketDataUpdate.hpp"
#include <MarketDataIncrementalRefreshLogger.hpp>
#include <quickfix/Message.h>

// This class contains several callback functions that run logic on certain
// state changes related to a depth order book.
class DepthEventHandler {
public:
  DepthEventHandler(Market *market);

  void on_depth_change(const DepthOrderBook *depth_order_book,
                       const DepthLevel *depth_level);

private:
  Market *m_market;
};