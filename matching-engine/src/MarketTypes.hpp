#pragma once

#include "OrderBookTypes.hpp"
#include <memory>

class Market; // From Market.hpp.
using MarketPtr = std::shared_ptr<Market>;

// This is used to map each order book's stock symbol to a shared pointer
// pointing to the order book class.
using OrderBookMap = std::unordered_map<std::string, OrderBookPtr>;

// The key here would be the order_id member.
using MarketOrderMap = std::unordered_map<std::string, OrderPtr>;

using MarketOrderMapPtr = std::shared_ptr<MarketOrderMap>;

// The key here would be the sender_id member.
using CustomerOrderMap = std::unordered_map<std::string, MarketOrderMapPtr>;