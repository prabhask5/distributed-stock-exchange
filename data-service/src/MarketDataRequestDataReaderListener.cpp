#include "MarketDataRequestDataReaderListener.hpp"
#include <DefaultDomainParticipantConstants.hpp>
#include <MarketDataRequest.hpp>
#include <MarketDataRequestLogger.hpp>

MarketDataRequestDataReaderListener::MarketDataRequestDataReaderListener(
    const MarketDataRequestQueuePtr &market_data_request_queue_ptr)
    : m_market_data_request_queue_ptr(market_data_request_queue_ptr) {}

void MarketDataRequestDataReaderListener::on_data_available(
    DataReader *reader) {
  DistributedStockExchange_MarketDataRequest::MarketDataRequest
      market_data_request;
  eprosima::fastdds::dds::SampleInfo info;

  if (reader->take_next_sample(&market_data_request, &info) ==
      eprosima::fastdds::dds::RETCODE_OK) {
    if (info.valid_data) {
      std::stringstream ss;
      MarketDataRequestLogger::log(ss, market_data_request);
      LOG4CXX_INFO(logger, "MarketDataRequest : [" << ss.str() << "]");

      m_market_data_request_queue_ptr->push(
          std::make_unique<
              DistributedStockExchange_MarketDataRequest::MarketDataRequest>(
              market_data_request));
    }
  }
}