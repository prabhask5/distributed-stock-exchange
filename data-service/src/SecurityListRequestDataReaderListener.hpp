#pragma once

#include "InstrumentReferenceDataServiceTypes.hpp"
#include <DefaultDomainParticipantTypes.hpp>

// This class takes in security list request DDS messages from the FIX gateway
// and enqueues them onto the queue for the InstrumentReferenceDataService to
// handle.
class SecurityListRequestDataReaderListener : public DataReaderListener {
public:
  SecurityListRequestDataReaderListener(
      const SecurityListRequestQueuePtr &security_list_request_queue_ptr);

  virtual void on_data_available(DataReader *reader) override;

private:
  SecurityListRequestQueuePtr m_security_list_request_queue_ptr;
};