#pragma once

#include "DepthOrderBookConstants.hpp"
#include <array>
#include <map>

class DepthLevel; // From DepthLevel.hpp.
using DepthLevels = std::array<DepthLevel, MARKET_DATA_PRICE_DEPTH * 2>;
using DepthLevelsSection = std::array<DepthLevel, MARKET_DATA_PRICE_DEPTH>;

using BuyLevelMap = std::map<Price, DepthLevel, std::greater<Price>>;
using SellLevelMap = std::map<Price, DepthLevel, std::less<Price>>;