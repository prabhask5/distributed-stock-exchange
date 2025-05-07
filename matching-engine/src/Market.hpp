#pragma once

#include <memory>
#include <string>

// This class represents the top level abstraction of the matching engine.
// An instance of the market class will contain one or more order books/depth
// order books, and provides all the functionality to take orders in and match
// them with corresponding orders. The market class also manages stats related
// to the matching engine to be sent to the user via reports or to the data
// service.
class Market {
public:
  // Getter const functions.

  std::string get_market_name() const;

private:
  std::string m_market_name;
};

using MarketPtr = std::shared_ptr<Market>;