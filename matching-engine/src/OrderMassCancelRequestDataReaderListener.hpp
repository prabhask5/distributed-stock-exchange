#pragma once

#include "MarketTypes.hpp"
#include <DefaultDomainParticipantTypes.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>

// This class takes in mass order cancel DDS requests and sends them to the
// market.
class OrderMassCancelRequestDataReaderListener : public DataReaderListener {
public:
  OrderMassCancelRequestDataReaderListener(const MarketPtr &market_ptr);

  void on_data_available(DataReader *reader) override;

private:
  MarketPtr m_market_ptr;
};