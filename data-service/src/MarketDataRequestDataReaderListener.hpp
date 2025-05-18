#pragma once

#include "MarketDataServiceTypes.hpp"
#include <DefaultDomainParticipantTypes.hpp>

// This class takes in market data request DDS messages from the FIX gateway and
// enqueues them onto the request queue for the MarketDataService to handle.
class MarketDataRequestDataReaderListener : public DataReaderListener {
public:
  MarketDataRequestDataReaderListener(
      const MarketDataRequestQueuePtr &market_data_request_queue_ptr);

  virtual void on_data_available(DataReader *reader) override;

private:
  MarketDataRequestQueuePtr m_market_data_request_queue_ptr;
};