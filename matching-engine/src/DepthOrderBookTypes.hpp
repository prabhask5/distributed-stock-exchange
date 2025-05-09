#pragma once

#include "DepthOrderBookConstants.hpp"
#include <array>

class DepthLevel; // From DepthLevel.hpp.
using DepthLevels = std::array<DepthLevel, MARKET_DATA_PRICE_DEPTH * 2>;
using DepthLevelsSection = std::array<DepthLevel, MARKET_DATA_PRICE_DEPTH>;