/* Don't modify, automatically generated file by QuickFIX2FastDDS.py*/
#pragma once
#include "OrderCancelReject.h"
#include <quickfix/Message.h>

#include "HeaderAdapter.hpp"

class OrderCancelRejectAdapter
{
public:
	static void FIX2DDS(const FIX::Message &, OrderCancelReject::OrderCancelReject &ddsMsg) __attribute__((visibility("default")));
	static void DDS2FIX(const OrderCancelReject::OrderCancelReject &ddsMsg, FIX::Message &) __attribute__((visibility("default")));
};