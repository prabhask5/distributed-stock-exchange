#pragma once
#include "SecurityListRequest.hpp"
#include <quickfix/Message.h>

#include "HeaderAdapter.hpp"


class SecurityListRequestAdapter
{
	public:
		static void FIX2DDS(const FIX::Message&, DistributedStockExchange_SecurityListRequest::SecurityListRequest& ddsMsg )  __attribute__ ((visibility ("default")));
		static void DDS2FIX(const DistributedStockExchange_SecurityListRequest::SecurityListRequest& ddsMsg, FIX::Message&)  __attribute__ ((visibility ("default")));

};