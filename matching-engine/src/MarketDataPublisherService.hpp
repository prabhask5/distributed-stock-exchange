#pragma once

#include "MarketDataUpdateTypes.hpp"
#include <DefaultDomainParticipantTypes.hpp>
#include <thread>

// This service is a separate thread to incrementally send market data updates
// to the data service periodically.
class MarketDataPublisherService {
public:
  MarketDataPublisherService(
      DataWriter *market_data_incremental_refresh_dw,
      MarketDataPublisherQueuePtr market_data_publisher_queue_ptr,
      unsigned int price_depth_pub_interval);

  ~MarketDataPublisherService();

  void service();

private:
  // Data writing dependencies.
  MarketDataPublisherQueuePtr m_market_data_publisher_queue_ptr;
  DataWriter *m_market_data_incremental_refresh_dw;

  // Service thread metadata.
  std::atomic<bool> m_is_running;
  std::thread m_publisher_thread;
  unsigned int m_price_depth_pub_interval;
};