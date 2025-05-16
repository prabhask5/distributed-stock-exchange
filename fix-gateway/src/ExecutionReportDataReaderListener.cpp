#include "ExecutionReportDataReaderListener.hpp"
#include "FixApplication.hpp"
#include "FixMsgTranslatorServiceTypes.hpp"
#include <ExecutionReportAdapter.hpp>
#include <quickfix/Message.h>

TranslatorFunc<DistributedStockExchange_ExecutionReport::ExecutionReport> const
    exec_report_translator =
        [](FixApplication &app,
           DistributedStockExchange_ExecutionReport::ExecutionReport
               &execution_report) {
          FIX::Message execution_report_message;

          execution_report.fix_header().BeginString("FIX.4.4");
          execution_report.fix_header().TargetCompID(
              execution_report.Destination());
          execution_report.fix_header().SenderCompID(
              execution_report.DestinationUser());
          execution_report.fix_header().SendingTime(0);

          ExecutionReportAdapter::DDS2FIX(execution_report,
                                          execution_report_message);

          app.publish_to_client(execution_report_message);
        };

ExecutionReportDataReaderListener::ExecutionReportDataReaderListener(
    FixApplication &app)
    : m_translator_service(app, exec_report_translator,
                           "ExecutionReportDataReaderListenerTranslator", 100) {
}

void ExecutionReportDataReaderListener::on_data_available(DataReader *reader) {
  DistributedStockExchange_ExecutionReport::ExecutionReport execution_report;
  eprosima::fastdds::dds::SampleInfo info;

  if (reader->take_next_sample(&execution_report, &info) ==
      eprosima::fastdds::dds::RETCODE_OK) {
    if (info.valid_data)
      m_translator_service.enqueue_dds_message(execution_report);
  }
}