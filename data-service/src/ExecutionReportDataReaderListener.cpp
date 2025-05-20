#include "ExecutionReportDataReaderListener.hpp"
#include <DefaultDomainParticipantConstants.hpp>
#include <ExecutionReport.hpp>
#include <ExecutionReportLogger.hpp>

ExecutionReportDataReaderListener::ExecutionReportDataReaderListener(
    const UserToOrderExecutionReportsMapPtr
        &user_to_order_execution_reports_map_ptr)
    : m_user_to_order_execution_reports_map_ptr(
          user_to_order_execution_reports_map_ptr) {}

void ExecutionReportDataReaderListener::on_data_available(DataReader *reader) {
  DistributedStockExchange_ExecutionReport::ExecutionReport execution_report;
  eprosima::fastdds::dds::SampleInfo info;

  if (reader->take_next_sample(&execution_report, &info) ==
      eprosima::fastdds::dds::RETCODE_OK) {
    if (info.valid_data) {
      std::stringstream ss;
      ExecutionReportLogger::log(ss, execution_report);
      LOG4CXX_INFO(logger, "Execution Report: [" << ss.str() << "]");

      std::string username = execution_report.DestinationUser();

      // Turn the incoming execution report into a smart pointer, than add it to
      // the order execution reports list map to use in OrderMassStatusService.

      auto oer_map_iter =
          m_user_to_order_execution_reports_map_ptr->find(username);
      if (oer_map_iter == m_user_to_order_execution_reports_map_ptr->end())
        oer_map_iter =
            m_user_to_order_execution_reports_map_ptr
                ->emplace(username,
                          std::make_shared<OrderToExecutionReportsMap>())
                .first;

      ExecutionReportPtr execution_report_ptr = std::make_shared<
          DistributedStockExchange_ExecutionReport::ExecutionReport>(
          execution_report);

      std::string order_id = execution_report.OrderID();

      auto exec_report_list_iter = (*oer_map_iter->second).find(order_id);
      if (exec_report_list_iter == (*oer_map_iter->second).end())
        exec_report_list_iter =
            (*oer_map_iter->second)
                .emplace(order_id, std::make_shared<ExecutionReportList>())
                .first;

      (*exec_report_list_iter->second).emplace_back(execution_report_ptr);
    }
  }
}