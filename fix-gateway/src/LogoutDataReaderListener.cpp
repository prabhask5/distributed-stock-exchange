#include "LogoutDataReaderListener.hpp"
#include "FixApplication.hpp"
#include "FixMsgTranslatorServiceTypes.hpp"
#include "LogoutAdapter.hpp"
#include "LogoutLogger.hpp"
#include <DefaultDomainParticipantConstants.hpp>

TranslatorFunc<DistributedStockExchange_Logout::Logout> const
    logout_translator = [](FixApplication &app,
                           DistributedStockExchange_Logout::Logout &logout) {
      FIX::Message logout_message;

      logout.fix_header().SendingTime(0);

      LogoutAdapter::DDS2FIX(logout, logout_message);

      std::string connection_token = logout.DestinationUser();
      app.process_dds_logout(connection_token, logout_message);
    };

LogoutDataReaderListener::LogoutDataReaderListener(FixApplication &app)
    : m_translator_service(app, logout_translator,
                           "LogoutDataReaderListenerTranslator") {}

void LogoutDataReaderListener::on_data_available(DataReader *reader) {
  DistributedStockExchange_Logout::Logout logout;
  eprosima::fastdds::dds::SampleInfo info;

  if (reader->take_next_sample(&logout, &info) ==
      eprosima::fastdds::dds::RETCODE_OK) {
    if (info.valid_data)
      m_translator_service.enqueue_dds_message(logout);
  }
}