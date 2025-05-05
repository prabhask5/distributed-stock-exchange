#pragma once
#include "Logout.hpp"
#include <quickfix/Message.h>

#include "HeaderAdapter.hpp"

class LogoutAdapter {
public:
  static void FIX2DDS(const FIX::Message &,
                      DistributedStockExchange_Logout::Logout &ddsMsg)
      __attribute__((visibility("default")));
  static void DDS2FIX(const DistributedStockExchange_Logout::Logout &ddsMsg,
                      FIX::Message &) __attribute__((visibility("default")));
};