#include "SecurityListDataReaderListener.hpp"
#include "Market.hpp"
#include "MatchingEngineDataWriterContainer.hpp"
#include <DefaultDomainParticipantConstants.hpp>
#include <LoggerHelper.hpp>
#include <MarketDataRequest.hpp>
#include <MarketDataRequestLogger.hpp>
#include <SecurityList.hpp>
#include <SecurityListLogger.hpp>
#include <quickfix/FixValues.h>

SecurityListRequestDataReaderListener::SecurityListRequestDataReaderListener(
    const MarketPtr &market_ptr)
    : m_market_ptr(market_ptr) {}

void SecurityListRequestDataReaderListener::on_data_available(
    DataReader *reader) {
  DistributedStockExchange_SecurityList::SecurityList security_list;
  eprosima::fastdds::dds::SampleInfo info;

  // We only need this data for initial market setup, if market is ready to go,
  // we don't need this data.
  if (m_market_ptr->is_ready_to_trade())
    return;

  if (reader->take_next_sample(&security_list, &info) ==
      eprosima::fastdds::dds::RETCODE_OK) {
    if (info.valid_data) {
      std::stringstream ss;
      SecurityListLogger::log(ss, security_list);
      LOG4CXX_INFO(logger, "SecurityList : [" << ss.str() << "]");

      for (uint32_t sec_index = 0;
           sec_index < security_list.c_NoRelatedSym().size(); sec_index++) {
        std::string instrument =
            security_list.c_NoRelatedSym()[sec_index].Symbol();
        m_market_ptr->add_book(instrument, true);
      }

      // Request to recieve opening price.
      DistributedStockExchange_MarketDataRequest::MarketDataRequest
          market_data_request;

      market_data_request.Destination("DATA_SERVICE");
      market_data_request.DestinationUser(
          m_market_ptr->get_data_service_name());
      market_data_request.Source("MATCHING_ENGINE");
      market_data_request.SourceUser(m_market_ptr->get_market_name());
      market_data_request.c_NoMDEntryTypes().resize(1);
      market_data_request.c_NoMDEntryTypes()[0].MDEntryType(
          FIX::MDEntryType_TRADE); // Last trade price.
      market_data_request.c_NoRelatedSym().resize(
          m_market_ptr->get_order_book_map().size());

      int index = 0;
      for (auto &order_book : m_market_ptr->get_order_book_map()) {
        market_data_request.c_NoRelatedSym()[index].Symbol(order_book.first);
        market_data_request.c_NoRelatedSym()[index].SecurityExchange(
            m_market_ptr->get_market_name());
        index++;
      }

      log_info<std::stringstream, MarketDataRequestLogger,
               DistributedStockExchange_MarketDataRequest::MarketDataRequest>(
          logger, market_data_request, "MarketDataRequest");
      eprosima::fastdds::dds::ReturnCode_t code =
          m_market_ptr->get_data_writer_container_ptr()
              ->marketDataRequestDW->write(&market_data_request);
      if (code != eprosima::fastdds::dds::RETCODE_OK)
        LOG4CXX_ERROR(logger, "MarketDataRequest write returned : " << code);
    }
  }
}