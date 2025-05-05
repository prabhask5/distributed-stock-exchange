#pragma once
#include "OrderCancelReplaceRequest.hpp"
#include <quickfix/Message.h>

#include "HeaderAdapter.hpp"


class OrderCancelReplaceRequestAdapter
{
	public:
		static void FIX2DDS(const FIX::Message&, DistributedStockExchange_OrderCancelReplaceRequest::OrderCancelReplaceRequest& ddsMsg )  __attribute__ ((visibility ("default")));
		static void DDS2FIX(const DistributedStockExchange_OrderCancelReplaceRequest::OrderCancelReplaceRequest& ddsMsg, FIX::Message&)  __attribute__ ((visibility ("default")));

};