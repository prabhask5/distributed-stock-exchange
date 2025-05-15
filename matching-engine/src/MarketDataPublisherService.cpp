#define NOMINMAX

#include "MarketDataPublisherService.hpp"
#include "MarketDataUpdate.hpp"
#include "MarketDataUpdateTypes.hpp"
#include <DefaultDomainParticipantConstants.hpp>
#include <LoggerHelper.hpp>
#include <MarketDataIncrementalRefresh.hpp>
#include <MarketDataIncrementalRefreshLogger.hpp>
#include <map>

MarketDataPublisherService::MarketDataPublisherService(
    DataWriter *market_data_incremental_refresh_dw,
    MarketDataPublisherQueuePtr market_data_publisher_queue_ptr,
    unsigned int price_depth_pub_interval)
    : m_market_data_incremental_refresh_dw(market_data_incremental_refresh_dw),
      m_market_data_publisher_queue_ptr(market_data_publisher_queue_ptr),
      m_price_depth_pub_interval(price_depth_pub_interval) {
  std::atomic_init(&m_is_running, true);
  m_publisher_thread = std::thread(&MarketDataPublisherService::service, this);
}

MarketDataPublisherService::~MarketDataPublisherService() {
  m_is_running.store(false);
  m_publisher_thread.join();
}

void MarketDataPublisherService::service() {
  while (m_is_running) {
    // If the queue is empty, we just sleep and wait for the next interval.
    if (m_market_data_publisher_queue_ptr->empty()) {
      std::this_thread::sleep_for(
          std::chrono::duration<long double, std::micro>(
              m_price_depth_pub_interval));
      continue;
    };

    std::map<std::string,
             DistributedStockExchange_MarketDataIncrementalRefresh::
                 MarketDataIncrementalRefresh>
        market_data_updates;

    MarketDataUpdatePtr market_data_update;

    // Pop off all the entries in the queue and store in map for easy access.
    while (m_market_data_publisher_queue_ptr->pop(market_data_update)) {
      market_data_updates[market_data_update->symbol] =
          market_data_update->refresh_data;

      std::stringstream ss;
      MarketDataIncrementalRefreshLogger::log(
          ss, market_data_updates[market_data_update->symbol]);
      LOG4CXX_INFO(logger,
                   "MarketDataIncrementalRefresh : [" << ss.str() << "]");
      std::cout << "Update : " << ss.str() << std::endl;
    }

    // Now from all the entries in the map, we're going to split them into
    // chunks to send over FastDDS to the data service.
    DistributedStockExchange_MarketDataIncrementalRefresh::
        MarketDataIncrementalRefresh market_data_refresh_chunk;

    market_data_refresh_chunk.Source("MATCHING_ENGINE");
    market_data_refresh_chunk.fix_header().MsgType("X");

    size_t max_chunk_size = 10;
    size_t chunk_size = std::min(market_data_updates.size(), max_chunk_size);

    market_data_refresh_chunk.c_NoMDEntries().resize(chunk_size * 14);

    int market_data_update_index = 0;
    int chunk_index = 0;

    for (auto market_data_update_iter : market_data_updates) {
      for (int md_index = 0; md_index < 14; md_index++) {
        market_data_refresh_chunk.c_NoMDEntries()[chunk_index++] =
            (market_data_update_iter.second).c_NoMDEntries()[md_index];
      }

      if ((++market_data_update_index) % max_chunk_size == 0 ||
          market_data_update_index == market_data_updates.size()) {
        log_debug<std::stringstream, MarketDataIncrementalRefreshLogger,
                  DistributedStockExchange_MarketDataIncrementalRefresh::
                      MarketDataIncrementalRefresh>(
            logger, market_data_refresh_chunk, "MarketDataIncrementalRefresh");
        std::cout << "Publishing chunk of "
                  << market_data_refresh_chunk.c_NoMDEntries().size()
                  << " updates" << std::endl;

        std::stringstream ss;
        MarketDataIncrementalRefreshLogger::log(ss, market_data_refresh_chunk);

        eprosima::fastdds::dds::ReturnCode_t code =
            m_market_data_incremental_refresh_dw->write(
                &market_data_refresh_chunk);
        if (code != eprosima::fastdds::dds::RETCODE_OK)
          LOG4CXX_ERROR(logger, "MarketDataIncrementalRefresh :" << code);
      }
    }

    market_data_updates.clear();
    std::this_thread::sleep_for(std::chrono::duration<long double, std::micro>(
        m_price_depth_pub_interval));
  }
}