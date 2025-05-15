#pragma once

#include <deque>
#include <set>

namespace FIX {
class SessionID; // From quickfix/Log.h.

}

using SessionSet = std::set<FIX::SessionID>;
using MessageQueue = std::deque<std::string>;