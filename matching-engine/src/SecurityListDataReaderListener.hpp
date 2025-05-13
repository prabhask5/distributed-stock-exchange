#pragma once

#include "MarketTypes.hpp"
#include <DefaultDomainParticipantTypes.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>

class SecurityListRequestDataReaderListener : public DataReaderListener {
public:
  SecurityListRequestDataReaderListener(const MarketPtr &market_ptr);

  void on_data_available(DataReader *reader) override;

private:
  MarketPtr m_market_ptr;
};