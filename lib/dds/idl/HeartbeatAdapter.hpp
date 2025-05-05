#pragma once
#include "Heartbeat.hpp"
#include <quickfix/Message.h>

#include "HeaderAdapter.hpp"

class HeartbeatAdapter {
public:
  static void FIX2DDS(const FIX::Message &,
                      DistributedStockExchange_Heartbeat::Heartbeat &ddsMsg)
      __attribute__((visibility("default")));
  static void
  DDS2FIX(const DistributedStockExchange_Heartbeat::Heartbeat &ddsMsg,
          FIX::Message &) __attribute__((visibility("default")));
};