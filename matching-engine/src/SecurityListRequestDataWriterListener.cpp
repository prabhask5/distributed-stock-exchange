#include "SecurityListRequestDataWriterListener.hpp"
#include "Market.hpp"
#include <DefaultDomainParticipantConstants.hpp>
#include <LoggerHelper.hpp>
#include <SecurityListRequest.hpp>
#include <SecurityListRequestLogger.hpp>

SecurityListRequestDataWriterListener::SecurityListRequestDataWriterListener(
    const MarketPtr &market_ptr)
    : m_matched(0), m_market_ptr(market_ptr) {}

void SecurityListRequestDataWriterListener::on_publication_matched(
    DataWriter *dwr,
    const eprosima::fastdds::dds::PublicationMatchedStatus &info) {
  if (info.current_count_change >= 1) {
    m_matched = info.total_count;
    LOG4CXX_INFO(logger,
                 "SecurityListRequestDataWriterListener Publisher Matched:"
                     << m_matched);
    if (!m_market_ptr->is_ready_to_trade()) {
      DistributedStockExchange_SecurityListRequest::SecurityListRequest
          security_list_request;

      security_list_request.Destination("DATA_SERVICE");
      security_list_request.DestinationUser(
          m_market_ptr->get_data_service_name());
      security_list_request.Source("MATCHINE_ENGINE");
      security_list_request.SourceUser(m_market_ptr->get_market_name());

      log_info<
          std::stringstream, SecurityListRequestLogger,
          DistributedStockExchange_SecurityListRequest::SecurityListRequest>(
          logger, security_list_request, "SecurityListRequest");

      auto ret = dwr->write(&security_list_request);
      if (ret)
        LOG4CXX_ERROR(logger, "SecurityListRequest write returned : " << ret);
    }
  } else if (info.current_count_change == -1) {
    m_matched = info.total_count;
    LOG4CXX_INFO(logger,
                 "SecurityListRequestDataWriterListener Publisher UnMatched:"
                     << m_matched);
  } else {
    LOG4CXX_INFO(logger, "Is not a valid value for PublicationMatchedStatus "
                         "current count change."
                             << info.current_count_change);
  }
}