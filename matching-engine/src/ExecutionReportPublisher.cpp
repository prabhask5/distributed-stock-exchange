#include "ExecutionReportPublisher.hpp"

ExecutionReportPublisher::ExecutionReportPublisher(
    DataWriterContainerPtr data_writer_container_ptr)
    : m_data_writer_container_ptr(data_writer_container_ptr) {}

void ExecutionReportPublisher::publish_report(
    DistributedStockExchange_ExecutionReport::ExecutionReport
        &execution_report) {
  execution_report.ExecID(get_id());
  log_debug<std::stringstream, ExecutionReportLogger,
            DistributedStockExchange_ExecutionReport::ExecutionReport>(
      logger, execution_report, "ExecutionReport");

  bool success =
      m_data_writer_container_ptr->execReportDW->write(&execution_report);

  if (!success) {
    LOG4CXX_ERROR(logger, "Execution Report write returned : " << success);
  }
}

// This function generates a string id from incrementing a static class member,
// then casting it into a string
std::string ExecutionReportPublisher::get_id() {
  char id_string_char_repr[64];
  snprintf(id_string_char_repr, 64, "%d", ++s_id_seed);
  std::string execution_report_id = id_string_char_repr;

  return execution_report_id;
}