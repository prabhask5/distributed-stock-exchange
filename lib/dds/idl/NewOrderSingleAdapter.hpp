#pragma once
#include "NewOrderSingle.hpp"
#include <quickfix/Message.h>

#include "HeaderAdapter.hpp"


class NewOrderSingleAdapter
{
	public:
		static void FIX2DDS(const FIX::Message&, DistributedStockExchange_NewOrderSingle::NewOrderSingle& ddsMsg )  __attribute__ ((visibility ("default")));
		static void DDS2FIX(const DistributedStockExchange_NewOrderSingle::NewOrderSingle& ddsMsg, FIX::Message&)  __attribute__ ((visibility ("default")));

};