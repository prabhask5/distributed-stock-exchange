#include "OrderMassCancelRequestDataReaderListener.hpp"
#include "Market.hpp"
#include "MatchingEngineDataWriterContainer.hpp"
#include <DefaultDomainParticipantConstants.hpp>
#include <LoggerHelper.hpp>
#include <OrderMassCancelReport.hpp>
#include <OrderMassCancelReportLogger.hpp>
#include <OrderMassCancelRequest.hpp>
#include <OrderMassCancelRequestLogger.hpp>
#include <quickfix/FixValues.h>

OrderMassCancelRequestDataReaderListener::
    OrderMassCancelRequestDataReaderListener(const MarketPtr &market_ptr)
    : m_market_ptr(market_ptr) {}

void OrderMassCancelRequestDataReaderListener::on_data_available(
    DataReader *reader) {
  DistributedStockExchange_OrderMassCancelRequest::OrderMassCancelRequest
      order_mass_cancel_request;
  eprosima::fastdds::dds::SampleInfo info;

  if (reader->take_next_sample(&order_mass_cancel_request, &info) ==
      eprosima::fastdds::dds::RETCODE_OK) {
    if (info.valid_data) {
      log_debug<std::stringstream, OrderMassCancelRequestLogger,
                DistributedStockExchange_OrderMassCancelRequest::
                    OrderMassCancelRequest>(logger, order_mass_cancel_request,
                                            "OrderMassCancelRequest");

      std::string sender_id = order_mass_cancel_request.SourceUser();

      DistributedStockExchange_OrderMassCancelReport::OrderMassCancelReport
          order_mass_cancel_report;

      order_mass_cancel_report.Source("MATCHING_ENGINE");
      order_mass_cancel_report.Destination(order_mass_cancel_request.Source());
      order_mass_cancel_report.DestinationUser(
          order_mass_cancel_request.SourceUser());

      order_mass_cancel_report.fix_header().MsgType("r");
      order_mass_cancel_report.MassCancelRequestType(
          FIX::MassCancelRequestType_CANCEL_ALL_ORDERS);
      order_mass_cancel_report.MassCancelResponse(
          FIX::MassCancelResponse_CANCEL_ALL_ORDERS);
      order_mass_cancel_report.OrderID(order_mass_cancel_request.ClOrdID());

      log_debug<std::stringstream, OrderMassCancelReportLogger,
                DistributedStockExchange_OrderMassCancelReport::
                    OrderMassCancelReport>(logger, order_mass_cancel_report,
                                           "OrderMassCancelReport");

      bool ret =
          m_market_ptr->get_data_writer_container_ptr()
              ->orderMassCancelReportDW->write(&order_mass_cancel_report);
      if (ret)
        LOG4CXX_ERROR(logger,
                      "Order Mass Cancel Report write returned: " << ret);

      m_market_ptr->mass_cancel_orders(sender_id);
    }
  }
}