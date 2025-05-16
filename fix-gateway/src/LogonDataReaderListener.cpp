#include "LogonDataReaderListener.hpp"
#include "FixApplication.hpp"
#include "FixMsgTranslatorServiceTypes.hpp"
#include "LogonAdapter.hpp"
#include "LogonLogger.hpp"
#include <DefaultDomainParticipantConstants.hpp>

TranslatorFunc<DistributedStockExchange_Logon::Logon> const logon_translator =
    [](FixApplication &app, DistributedStockExchange_Logon::Logon &logon) {
      FIX::Message logon_message;

      logon.fix_header().SendingTime(0);
      logon.fix_header().TargetSubID(logon.DestinationUser());

      LogonAdapter::DDS2FIX(logon, logon_message);

      app.process_dds_logon(logon_message);
    };

LogonDataReaderListener::LogonDataReaderListener(FixApplication &app)
    : m_translator_service(app, logon_translator,
                           "LogonDataReaderListenerTranslator") {}

void LogonDataReaderListener::on_data_available(DataReader *reader) {
  DistributedStockExchange_Logon::Logon logon;
  eprosima::fastdds::dds::SampleInfo info;

  if (reader->take_next_sample(&logon, &info) ==
      eprosima::fastdds::dds::RETCODE_OK) {
    if (info.valid_data)
      m_translator_service.enqueue_dds_message(logon);
  }
}