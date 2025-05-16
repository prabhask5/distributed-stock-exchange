#include "OrderMassCancelReportDataReaderListener.hpp"
#include "FixApplication.hpp"
#include "FixMsgTranslatorServiceTypes.hpp"
#include <OrderMassCancelReportAdapter.hpp>

TranslatorFunc<
    DistributedStockExchange_OrderMassCancelReport::OrderMassCancelReport> const
    order_mass_cancel_report_translator =
        [](FixApplication &app,
           DistributedStockExchange_OrderMassCancelReport::OrderMassCancelReport
               &order_mass_cancel_report) {
          FIX::Message order_mass_cancel_report_message;

          order_mass_cancel_report.fix_header().BeginString("FIX.4.4");
          order_mass_cancel_report.fix_header().SendingTime(0);
          order_mass_cancel_report.fix_header().SenderCompID(
              order_mass_cancel_report.fix_header().TargetSubID());

          OrderMassCancelReportAdapter::DDS2FIX(
              order_mass_cancel_report, order_mass_cancel_report_message);

          app.publish_to_client(order_mass_cancel_report_message);
        };

OrderMassCancelReportDataReaderListener::
    OrderMassCancelReportDataReaderListener(FixApplication &app)
    : m_translator_service(app, order_mass_cancel_report_translator,
                           "OrderMassCancelReportDataReaderListenerTranslator",
                           500) {}

void OrderMassCancelReportDataReaderListener::on_data_available(
    DataReader *reader) {
  DistributedStockExchange_OrderMassCancelReport::OrderMassCancelReport
      order_mass_cancel_report;
  eprosima::fastdds::dds::SampleInfo info;

  if (reader->take_next_sample(&order_mass_cancel_report, &info) ==
      eprosima::fastdds::dds::RETCODE_OK) {
    if (info.valid_data)
      m_translator_service.enqueue_dds_message(order_mass_cancel_report);
  }
}
