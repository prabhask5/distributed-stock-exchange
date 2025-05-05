#pragma once
#include "SecurityList.hpp"
#include <quickfix/Message.h>

#include "HeaderAdapter.hpp"


class SecurityListAdapter
{
	public:
		static void FIX2DDS(const FIX::Message&, DistributedStockExchange_SecurityList::SecurityList& ddsMsg )  __attribute__ ((visibility ("default")));
		static void DDS2FIX(const DistributedStockExchange_SecurityList::SecurityList& ddsMsg, FIX::Message&)  __attribute__ ((visibility ("default")));

};