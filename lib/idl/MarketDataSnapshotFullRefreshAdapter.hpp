/* Don't modify, automatically generated file by QuickFIX2FastDDS.py*/
#pragma once
#include "MarketDataSnapshotFullRefresh.h"
#include <quickfix/Message.h>

#include "HeaderAdapter.hpp"

class MarketDataSnapshotFullRefreshAdapter
{
public:
	static void FIX2DDS(const FIX::Message &, MarketDataSnapshotFullRefresh::MarketDataSnapshotFullRefresh &ddsMsg) __attribute__((visibility("default")));
	static void DDS2FIX(const MarketDataSnapshotFullRefresh::MarketDataSnapshotFullRefresh &ddsMsg, FIX::Message &) __attribute__((visibility("default")));
};