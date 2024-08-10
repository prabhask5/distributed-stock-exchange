/* Don't modify, automatically generated file by QuickFIX2FastDDS.py*/
#pragma once
#include "MarketDataRequest.h"
#include <quickfix/Message.h>

#include "HeaderAdapter.hpp"

class MarketDataRequestAdapter
{
public:
	static void FIX2DDS(const FIX::Message &, MarketDataRequest::MarketDataRequest &ddsMsg) __attribute__((visibility("default")));
	static void DDS2FIX(const MarketDataRequest::MarketDataRequest &ddsMsg, FIX::Message &) __attribute__((visibility("default")));
};