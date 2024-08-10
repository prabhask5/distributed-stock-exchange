/* Don't modify, automatically generated file by QuickFIX2FastDDS.py*/
#pragma once
#include "OrderCancelRequest.h"
#include <quickfix/Message.h>

#include "HeaderAdapter.hpp"

class OrderCancelRequestAdapter
{
public:
	static void FIX2DDS(const FIX::Message &, OrderCancelRequest::OrderCancelRequest &ddsMsg) __attribute__((visibility("default")));
	static void DDS2FIX(const OrderCancelRequest::OrderCancelRequest &ddsMsg, FIX::Message &) __attribute__((visibility("default")));
};