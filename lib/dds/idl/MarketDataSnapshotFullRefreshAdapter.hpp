#pragma once
#include "MarketDataSnapshotFullRefresh.hpp"
#include <quickfix/Message.h>

#include "HeaderAdapter.hpp"


class MarketDataSnapshotFullRefreshAdapter
{
	public:
		static void FIX2DDS(const FIX::Message&, DistributedStockExchange_MarketDataSnapshotFullRefresh::MarketDataSnapshotFullRefresh& ddsMsg )  __attribute__ ((visibility ("default")));
		static void DDS2FIX(const DistributedStockExchange_MarketDataSnapshotFullRefresh::MarketDataSnapshotFullRefresh& ddsMsg, FIX::Message&)  __attribute__ ((visibility ("default")));

};