/* Don't modify, automatically generated file by QuickFIX2FastDDS.py*/
#pragma once
#include "Logon.h"
#include <quickfix/Message.h>

#include "HeaderAdapter.hpp"

class LogonAdapter
{
public:
	static void FIX2DDS(const FIX::Message &, Logon::Logon &ddsMsg) __attribute__((visibility("default")));
	static void DDS2FIX(const Logon::Logon &ddsMsg, FIX::Message &) __attribute__((visibility("default")));
};