#pragma once

#include "MarketTypes.hpp"
#include <DefaultDomainParticipantTypes.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>

// This class takes in DDS requests for new orders and adds them to the market
// order book.
class NewOrderSingleDataReaderListener : public DataReaderListener {
public:
  NewOrderSingleDataReaderListener(const MarketPtr &market_ptr);

  void on_data_available(DataReader *reader) override;

private:
  MarketPtr m_market_ptr;
};