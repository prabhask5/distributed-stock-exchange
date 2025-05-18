#include "OrderMassStatusService.hpp"
#include "DataServiceDataWriterContainer.hpp"
#include <DefaultDomainParticipantConstants.hpp>
#include <ExecutionReport.hpp>
#include <OrderMassStatusRequest.hpp>

OrderMassStatusService::OrderMassStatusService(
    const DataWriterContainerPtr &data_writer_container_ptr,
    OrderMassStatusRequestQueuePtr order_mass_status_request_queue_ptr,
    SymbolToOrderExecutionReportsMapPtr
        symbol_to_order_execution_reports_map_ptr)
    : m_data_writer_container_ptr(data_writer_container_ptr),
      m_order_mass_status_request_queue_ptr(
          order_mass_status_request_queue_ptr),
      m_symbol_to_order_execution_reports_map_ptr(
          symbol_to_order_execution_reports_map_ptr) {
  std::atomic_init(&m_is_running, true);
  m_service_thread = std::thread(&OrderMassStatusService::service, this);
}

OrderMassStatusService::~OrderMassStatusService() {
  m_is_running.store(false);
  m_service_thread.join();
}

void OrderMassStatusService::service() {
  while (m_is_running.load()) {
    while (!m_order_mass_status_request_queue_ptr->empty()) {
      OrderMassStatusRequestPtr order_mass_status_request_ptr;
      m_order_mass_status_request_queue_ptr->pop(order_mass_status_request_ptr);
      process_order_mass_status_service_request(
          std::move(order_mass_status_request_ptr));
    }

    std::this_thread::sleep_for(
        std::chrono::duration<long double, std::milli>(1000));
  }
}

void OrderMassStatusService::process_order_mass_status_service_request(
    OrderMassStatusRequestPtr order_mass_status_request_ptr) {
  std::string username = order_mass_status_request_ptr->SourceUser();
  auto oer_map_itr =
      m_symbol_to_order_execution_reports_map_ptr->find(username);
  if (oer_map_itr == m_symbol_to_order_execution_reports_map_ptr->end())
    return;

  OrderToExecutionReportsMapPtr order_to_execution_reports_map_ptr =
      oer_map_itr->second;

  for (auto &exec_list_iter : *order_to_execution_reports_map_ptr) {
    for (auto &exec_report_iter : *(exec_list_iter.second)) {
      exec_report_iter->Source(order_mass_status_request_ptr->Destination());
      exec_report_iter->Destination(order_mass_status_request_ptr->Source());
      exec_report_iter->DestinationUser(username);

      eprosima::fastdds::dds::ReturnCode_t code =
          m_data_writer_container_ptr->executionReportDW->write(
              exec_report_iter.get());

      if (code != eprosima::fastdds::dds::RETCODE_OK)
        LOG4CXX_ERROR(
            logger,
            "MassOrderStatusRequestDataReader/Execution Report write returned :"
                << code);
    }
  }
}