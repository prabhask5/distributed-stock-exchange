#pragma once
#include "Header.hpp"
#include <quickfix/Message.h>



class HeaderAdapter
{
	public:
		static void FIX2DDS(const FIX::FieldMap&, DistributedStockExchange::Header& ddsMsg )  __attribute__ ((visibility ("default")));
		static void DDS2FIX(const DistributedStockExchange::Header& ddsMsg, FIX::FieldMap&)  __attribute__ ((visibility ("default")));

};