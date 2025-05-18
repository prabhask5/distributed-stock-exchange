#include "OrderMassStatusRequestDataReaderListener.hpp"
#include <DefaultDomainParticipantConstants.hpp>
#include <OrderMassStatusRequest.hpp>
#include <OrderMassStatusRequestLogger.hpp>

OrderMassStatusRequestDataReaderListener::
    OrderMassStatusRequestDataReaderListener(
        const OrderMassStatusRequestQueuePtr
            &order_mass_status_request_queue_ptr)
    : m_order_mass_status_request_queue_ptr(
          order_mass_status_request_queue_ptr) {}

void OrderMassStatusRequestDataReaderListener::on_data_available(
    DataReader *reader) {
  DistributedStockExchange_OrderMassStatusRequest::OrderMassStatusRequest
      order_mass_status_request;
  eprosima::fastdds::dds::SampleInfo info;

  if (reader->take_next_sample(&order_mass_status_request, &info) ==
      eprosima::fastdds::dds::RETCODE_OK) {
    if (info.valid_data) {

      std::stringstream ss;
      OrderMassStatusRequestLogger::log(ss, order_mass_status_request);
      LOG4CXX_INFO(logger, "OrderMassStatusRequest :" << ss.str());

      m_order_mass_status_request_queue_ptr->push(
          std::make_unique<DistributedStockExchange_OrderMassStatusRequest::
                               OrderMassStatusRequest>(
              order_mass_status_request));
    }
  }
}