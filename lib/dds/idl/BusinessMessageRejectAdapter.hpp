#pragma once
#include "BusinessMessageReject.hpp"
#include <quickfix/Message.h>

#include "HeaderAdapter.hpp"

class BusinessMessageRejectAdapter {
public:
  static void
  FIX2DDS(const FIX::Message &,
          DistributedStockExchange_BusinessMessageReject::BusinessMessageReject
              &ddsMsg) __attribute__((visibility("default")));
  static void DDS2FIX(const DistributedStockExchange_BusinessMessageReject::
                          BusinessMessageReject &ddsMsg,
                      FIX::Message &) __attribute__((visibility("default")));
};