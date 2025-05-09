#pragma once

// This class contains several callback functions that run logic on certain
// state changes related to a depth order book.
class DepthEventHandler {
public:
  DepthEventHandler(Market *market);

  void on_depth_change(const DepthOrderBook *depth_order_book);

private:
  Market *m_market;
};