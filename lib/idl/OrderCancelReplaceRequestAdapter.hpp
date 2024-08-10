/* Don't modify, automatically generated file by QuickFIX2FastDDS.py*/
#pragma once
#include "OrderCancelReplaceRequest.h"
#include <quickfix/Message.h>

#include "HeaderAdapter.hpp"

class OrderCancelReplaceRequestAdapter
{
public:
	static void FIX2DDS(const FIX::Message &, OrderCancelReplaceRequest::OrderCancelReplaceRequest &ddsMsg) __attribute__((visibility("default")));
	static void DDS2FIX(const OrderCancelReplaceRequest::OrderCancelReplaceRequest &ddsMsg, FIX::Message &) __attribute__((visibility("default")));
};