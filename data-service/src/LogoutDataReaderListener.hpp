#pragma once

#include "DataServiceDataWriterContainerTypes.hpp"
#include <DefaultDomainParticipantTypes.hpp>

// This class takes in logout DDS messages from the FIX gateway and just logs
// them I guess, since we don't support external logout requests.
class LogoutDataReaderListener : public DataReaderListener {
public:
  LogoutDataReaderListener(
      const DataWriterContainerPtr &data_writer_container_ptr);

  virtual void on_data_available(DataReader *reader) override;

private:
  DataWriterContainerPtr m_data_writer_container_ptr;
};