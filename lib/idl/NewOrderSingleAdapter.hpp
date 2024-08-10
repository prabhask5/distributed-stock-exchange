/* Don't modify, automatically generated file by QuickFIX2FastDDS.py*/
#pragma once
#include "NewOrderSingle.h"
#include <quickfix/Message.h>

#include "HeaderAdapter.hpp"

class NewOrderSingleAdapter
{
public:
	static void FIX2DDS(const FIX::Message &, NewOrderSingle::NewOrderSingle &ddsMsg) __attribute__((visibility("default")));
	static void DDS2FIX(const NewOrderSingle::NewOrderSingle &ddsMsg, FIX::Message &) __attribute__((visibility("default")));
};