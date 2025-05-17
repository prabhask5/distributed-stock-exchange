#pragma once

#include "FixSocketConnectionTypes.hpp"
#include <map>

using PortSessionSetMap = std::map<int, SessionSet>;

class FixSocketConnection; // From FixSocketConnection.hpp.
using FDSocketConnectionMap = std::map<int, FixSocketConnection *>;

class FixSocketAcceptor; // From FixSocketAcceptor.hpp.
using FixSocketAcceptorPtr = std::unique_ptr<FixSocketAcceptor>;