#pragma once
#include "OrderCancelReject.hpp"
#include <quickfix/Message.h>

#include "HeaderAdapter.hpp"

class OrderCancelRejectAdapter {
public:
  static void
  FIX2DDS(const FIX::Message &,
          DistributedStockExchange_OrderCancelReject::OrderCancelReject &ddsMsg)
      __attribute__((visibility("default")));
  static void
  DDS2FIX(const DistributedStockExchange_OrderCancelReject::OrderCancelReject
              &ddsMsg,
          FIX::Message &) __attribute__((visibility("default")));
};