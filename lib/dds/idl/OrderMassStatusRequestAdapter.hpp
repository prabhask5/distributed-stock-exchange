#pragma once
#include "OrderMassStatusRequest.hpp"
#include <quickfix/Message.h>

#include "HeaderAdapter.hpp"


class OrderMassStatusRequestAdapter
{
	public:
		static void FIX2DDS(const FIX::Message&, DistributedStockExchange_OrderMassStatusRequest::OrderMassStatusRequest& ddsMsg )  __attribute__ ((visibility ("default")));
		static void DDS2FIX(const DistributedStockExchange_OrderMassStatusRequest::OrderMassStatusRequest& ddsMsg, FIX::Message&)  __attribute__ ((visibility ("default")));

};