#include "LogoutDataReaderListener.hpp"
#include <DefaultDomainParticipantConstants.hpp>
#include <Logout.hpp>
#include <LogoutLogger.hpp>

LogoutDataReaderListener::LogoutDataReaderListener(
    const DataWriterContainerPtr &data_writer_container_ptr)
    : m_data_writer_container_ptr(data_writer_container_ptr) {}

void LogoutDataReaderListener::on_data_available(DataReader *reader) {
  DistributedStockExchange_Logout::Logout logout;
  eprosima::fastdds::dds::SampleInfo info;

  if (reader->take_next_sample(&logout, &info) ==
      eprosima::fastdds::dds::RETCODE_OK) {
    if (info.valid_data) {
      std::stringstream ss;
      LogoutLogger::log(ss, logout);
      LOG4CXX_INFO(logger, "Data Reader Logout : [" << ss.str() << "]");
    }
  }
}