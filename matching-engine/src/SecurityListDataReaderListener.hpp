#pragma once

#include "MarketTypes.hpp"
#include <DefaultDomainParticipantTypes.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>

// This class takes in initial market metadata, specifically a list of what
// securities are in the market (which are the different stock tickers that are
// in the market).
class SecurityListRequestDataReaderListener : public DataReaderListener {
public:
  SecurityListRequestDataReaderListener(const MarketPtr &market_ptr);

  void on_data_available(DataReader *reader) override;

private:
  MarketPtr m_market_ptr;
};