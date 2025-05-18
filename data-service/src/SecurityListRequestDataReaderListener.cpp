#include "SecurityListRequestDataReaderListener.hpp"
#include <DefaultDomainParticipantConstants.hpp>
#include <LoggerHelper.hpp>
#include <SecurityListRequest.hpp>
#include <SecurityListRequestLogger.hpp>

SecurityListRequestDataReaderListener::SecurityListRequestDataReaderListener(
    const SecurityListRequestQueuePtr &security_list_request_queue_ptr)
    : m_security_list_request_queue_ptr(security_list_request_queue_ptr) {}

void SecurityListRequestDataReaderListener::on_data_available(
    DataReader *reader) {
  DistributedStockExchange_SecurityListRequest::SecurityListRequest
      security_list_request;
  eprosima::fastdds::dds::SampleInfo info;

  if (reader->take_next_sample(&security_list_request, &info) ==
      eprosima::fastdds::dds::RETCODE_OK) {
    if (info.valid_data) {
      log_info<
          std::stringstream, SecurityListRequestLogger,
          DistributedStockExchange_SecurityListRequest::SecurityListRequest>(
          logger, security_list_request, "SecurityListRequest");
      m_security_list_request_queue_ptr->push(
          std::make_unique<DistributedStockExchange_SecurityListRequest::
                               SecurityListRequest>(security_list_request));
    }
  }
}