#include "OrderCancelRequestDataReaderListener.hpp"
#include "Market.hpp"
#include "MatchingEngineDataWriterContainer.hpp"
#include "OrderException.hpp"
#include <DefaultDomainParticipantConstants.hpp>
#include <LoggerHelper.hpp>
#include <OrderCancelReject.hpp>
#include <OrderCancelRejectLogger.hpp>
#include <OrderCancelRequest.hpp>
#include <OrderCancelRequestLogger.hpp>
#include <quickfix/FixValues.h>

OrderCancelRequestDataReaderListener::OrderCancelRequestDataReaderListener(
    const MarketPtr &market_ptr)
    : m_market_ptr(market_ptr) {}

void OrderCancelRequestDataReaderListener::on_data_available(
    DataReader *reader) {
  DistributedStockExchange_OrderCancelRequest::OrderCancelRequest
      order_cancel_request;
  eprosima::fastdds::dds::SampleInfo info;

  if (reader->take_next_sample(&order_cancel_request, &info) ==
      eprosima::fastdds::dds::RETCODE_OK) {
    if (info.valid_data) {
      log_debug<
          std::stringstream, OrderCancelRequestLogger,
          DistributedStockExchange_OrderCancelRequest::OrderCancelRequest>(
          logger, order_cancel_request, "OrderCancelRequest");

      std::string symbol = order_cancel_request.Symbol();

      try {
        OrderBookPtr order_book = m_market_ptr->find_book(symbol);
        if (!order_book)
          throw OrderException(order_cancel_request,
                               FIX::OrdRejReason_UNKNOWN_SYMBOL);

        std::string order_id = order_cancel_request.OrigClOrdID();
        std::string sender_id = order_cancel_request.SourceUser();

        m_market_ptr->cancel_order(order_book, sender_id, order_id);

      } catch (OrderException &orderException) {
        DistributedStockExchange_OrderCancelReject::OrderCancelReject
            order_cancel_reject;

        order_cancel_reject.Source("MATCHING_ENGINE");
        order_cancel_reject.Destination(order_cancel_request.Source());
        order_cancel_reject.fix_header().MsgType("9");
        order_cancel_reject.Text("Cancel Rejected REJECT");
        order_cancel_reject.ClOrdID(order_cancel_request.ClOrdID());

        log_debug<
            std::stringstream, OrderCancelRejectLogger,
            DistributedStockExchange_OrderCancelReject::OrderCancelReject>(
            logger, order_cancel_reject, "OrderCancelReject");

        bool ret = m_market_ptr->get_data_writer_container_ptr()
                       ->orderCancelRejectDW->write(&order_cancel_reject);
        if (ret)
          LOG4CXX_ERROR(logger, "Order Cancel Reject write returned: " << ret);
      }
    }
  }
}