#include "OrderCancelRejectDataReaderListener.hpp"
#include "FixApplication.hpp"
#include "FixMsgTranslatorServiceTypes.hpp"
#include <HeaderAdapter.hpp>
#include <OrderCancelRejectAdapter.hpp>

TranslatorFunc<
    DistributedStockExchange_OrderCancelReject::OrderCancelReject> const
    order_cancel_reject_translator =
        [](FixApplication &app,
           DistributedStockExchange_OrderCancelReject::OrderCancelReject
               &order_cancel_reject) {
          FIX::Message order_cancel_reject_message;

          order_cancel_reject.fix_header().SendingTime(0);

          HeaderAdapter::DDS2FIX(order_cancel_reject.fix_header(),
                                 order_cancel_reject_message.getHeader());
          OrderCancelRejectAdapter::DDS2FIX(order_cancel_reject,
                                            order_cancel_reject_message);

          app.publish_to_client(order_cancel_reject_message);
        };

OrderCancelRejectDataReaderListener::OrderCancelRejectDataReaderListener(
    FixApplication &app)
    : m_translator_service(app, order_cancel_reject_translator,
                           "OrderCancelRejectDataReaderListenerTranslator",
                           100) {}

void OrderCancelRejectDataReaderListener::on_data_available(
    DataReader *reader) {
  DistributedStockExchange_OrderCancelReject::OrderCancelReject
      order_cancel_reject;
  eprosima::fastdds::dds::SampleInfo info;

  if (reader->take_next_sample(&order_cancel_reject, &info) ==
      eprosima::fastdds::dds::RETCODE_OK) {
    if (info.valid_data)
      m_translator_service.enqueue_dds_message(order_cancel_reject);
  }
}
