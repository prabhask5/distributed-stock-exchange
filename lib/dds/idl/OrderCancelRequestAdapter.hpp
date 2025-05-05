#pragma once
#include "OrderCancelRequest.hpp"
#include <quickfix/Message.h>

#include "HeaderAdapter.hpp"

class OrderCancelRequestAdapter {
public:
  static void FIX2DDS(
      const FIX::Message &,
      DistributedStockExchange_OrderCancelRequest::OrderCancelRequest &ddsMsg)
      __attribute__((visibility("default")));
  static void
  DDS2FIX(const DistributedStockExchange_OrderCancelRequest::OrderCancelRequest
              &ddsMsg,
          FIX::Message &) __attribute__((visibility("default")));
};