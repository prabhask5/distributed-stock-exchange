#pragma once

#include "MarketTypes.hpp"
#include <DefaultDomainParticipantTypes.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>

// This class takes in order cancel DDS requests and sends them to the market.
class OrderCancelRequestDataReaderListener : public DataReaderListener {
public:
  OrderCancelRequestDataReaderListener(const MarketPtr &market_ptr);

  virtual void on_data_available(DataReader *reader) override;

private:
  MarketPtr m_market_ptr;
};