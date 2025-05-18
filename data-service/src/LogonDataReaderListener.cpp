#include "LogonDataReaderListener.hpp"
#include <DefaultDomainParticipantConstants.hpp>
#include <Logon.hpp>
#include <LogonLogger.hpp>

LogonDataReaderListener::LogonDataReaderListener(
    const LogonQueuePtr &logon_queue_ptr)
    : m_logon_queue_ptr(logon_queue_ptr) {}

void LogonDataReaderListener::on_data_available(DataReader *reader) {
  DistributedStockExchange_Logon::Logon logon;
  eprosima::fastdds::dds::SampleInfo info;

  if (reader->take_next_sample(&logon, &info) ==
      eprosima::fastdds::dds::RETCODE_OK) {
    if (info.valid_data) {
      std::stringstream ss;
      LogonLogger::log(ss, logon);
      LOG4CXX_INFO(logger, "Data Reader Logon : [" << ss.str() << "]"
                                                   << logon.Destination());

      if (logon.Destination().compare("DATA_SERVICE") == 0)
        m_logon_queue_ptr->push(
            std::make_shared<DistributedStockExchange_Logon::Logon>(logon));
    }
  }
}