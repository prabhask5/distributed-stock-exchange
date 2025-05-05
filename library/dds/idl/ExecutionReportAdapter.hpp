#pragma once
#include "ExecutionReport.hpp"
#include <quickfix/Message.h>

#include "HeaderAdapter.hpp"

class ExecutionReportAdapter {
public:
  static void
  FIX2DDS(const FIX::Message &,
          DistributedStockExchange_ExecutionReport::ExecutionReport &ddsMsg)
      __attribute__((visibility("default")));
  static void DDS2FIX(
      const DistributedStockExchange_ExecutionReport::ExecutionReport &ddsMsg,
      FIX::Message &) __attribute__((visibility("default")));
};