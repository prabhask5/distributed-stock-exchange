#pragma once

#include "AuthServiceTypes.hpp"
#include <DefaultDomainParticipantTypes.hpp>

// This class takes in logon DDS messages from the FIX gateway and enqueues them
// onto the request queue for AuthService to handle.
class LogonDataReaderListener : public DataReaderListener {
public:
  LogonDataReaderListener(const LogonQueuePtr &logon_queue_ptr);

  virtual void on_data_available(DataReader *reader) override;

private:
  LogonQueuePtr m_logon_queue_ptr;
};