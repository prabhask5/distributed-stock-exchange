#pragma once

#include "MarketTypes.hpp"
#include <DefaultDomainParticipantTypes.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>

// This class represents a listener for the market data snapshot data reader. It
// takes in market price data for each stock ticker symbol and updates the
// market order books accordingly.
class MarketDataSnapshotFullRefreshDataReaderListener
    : public DataReaderListener {
public:
  MarketDataSnapshotFullRefreshDataReaderListener(const MarketPtr &market_ptr);

  virtual void on_data_available(DataReader *reader) override;

private:
  MarketPtr m_market_ptr;
};