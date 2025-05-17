#pragma once

#include <memory>

class MarketDataPublisherService; // From MarketDataPublisherService.hpp.
using MarketDataPublisherServicePtr =
    std::unique_ptr<MarketDataPublisherService>;