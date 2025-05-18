#pragma once

#include "MarketDataServiceTypes.hpp"
#include <DefaultDomainParticipantTypes.hpp>

// This class takes in market data incremental refresh DDS messages from the
// matching engine, and updates the internal incremental refresh cache for
// MarketDataService to use to respond to market data request messages.
class MarketDataIncrementalRefreshDataReaderListener
    : public DataReaderListener {
public:
  MarketDataIncrementalRefreshDataReaderListener(
      const IncrementalRefreshMapPtr &incremental_refresh_map_ptr);

  virtual void on_data_available(DataReader *reader) override;

private:
  IncrementalRefreshMapPtr m_incremental_refresh_map_ptr;
};