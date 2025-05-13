#pragma once

#include "MarketTypes.hpp"
#include <DefaultDomainParticipantTypes.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>

class MarketDataSnapshotFullRefreshDataReaderListener
    : public DataReaderListener {
public:
  MarketDataSnapshotFullRefreshDataReaderListener(const MarketPtr &market_ptr);

  virtual void on_data_available(DataReader *reader) override;

private:
  MarketPtr m_market_ptr;
};