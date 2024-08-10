/* Don't modify, automatically generated file by QuickFIX2FastDDS.py*/
#pragma once
#include "Heartbeat.h"
#include <quickfix/Message.h>

#include "HeaderAdapter.hpp"

class HeartbeatAdapter
{
public:
	static void FIX2DDS(const FIX::Message &, Heartbeat::Heartbeat &ddsMsg) __attribute__((visibility("default")));
	static void DDS2FIX(const Heartbeat::Heartbeat &ddsMsg, FIX::Message &) __attribute__((visibility("default")));
};