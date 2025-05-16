#include "SecurityListDataReaderListener.hpp"
#include "FixApplication.hpp"
#include "FixMsgTranslatorServiceTypes.hpp"
#include <HeaderAdapter.hpp>
#include <quickfix/fix44/SecurityList.h>

TranslatorFunc<DistributedStockExchange_SecurityList::SecurityList> const
    security_list_translator =
        [](FixApplication &app,
           DistributedStockExchange_SecurityList::SecurityList &security_list) {
          FIX44::SecurityList security_list_message;

          security_list.fix_header().SendingTime(0);
          security_list.fix_header().TargetCompID(security_list.Destination());
          security_list.fix_header().SenderCompID(
              security_list.DestinationUser());

          HeaderAdapter::DDS2FIX(security_list.fix_header(),
                                 security_list_message.getHeader());

          // Here we're copying specific pieces of information from the DDS
          // message and adding it to the final FIX message.

          FIX::SecurityReqID security_req_id(security_list.SecurityReqID());
          security_list_message.setField(security_req_id);

          FIX::SecurityResponseID security_response_id(
              security_list.SecurityResponseID());
          security_list_message.setField(security_response_id);

          FIX::SecurityRequestResult security_request_result(
              security_list.SecurityRequestResult());
          security_list_message.setField(security_request_result);

          // Iterate throguh every individual security in the security list.
          for (int sec_index = 0;
               sec_index < security_list.c_NoRelatedSym().size(); sec_index++) {
            // More of the same thing, copy data over to the FIX message from
            // the DDS one.

            FIX44::SecurityList::NoRelatedSym related_symbol;

            std::string instrument =
                security_list.c_NoRelatedSym()[sec_index].Symbol();
            std::string exchange =
                security_list.c_NoRelatedSym()[sec_index].SecurityExchange();
            std::string ref_data =
                security_list.c_NoRelatedSym()[sec_index].Text();

            FIX::Symbol symbol(instrument);
            related_symbol.setField(symbol);

            FIX::SecurityExchange securityExchange(exchange);
            related_symbol.setField(securityExchange);

            if (ref_data.size() > 0) {
              FIX::Text text(ref_data);
              related_symbol.setField(text);
            }

            security_list_message.addGroup(related_symbol);
          }

          app.publish_to_client(security_list_message);
        };

SecurityListDataReaderListener::SecurityListDataReaderListener(
    FixApplication &app)
    : m_translator_service(app, security_list_translator,
                           "SecurityListDataReaderListenerTranslator") {}

void SecurityListDataReaderListener::on_data_available(DataReader *reader) {
  DistributedStockExchange_SecurityList::SecurityList security_list;
  eprosima::fastdds::dds::SampleInfo info;

  if (reader->take_next_sample(&security_list, &info) ==
      eprosima::fastdds::dds::RETCODE_OK) {
    if (info.valid_data)
      m_translator_service.enqueue_dds_message(security_list);
  }
}
