#pragma once

#include "OrderMassStatusServiceTypes.hpp"
#include <DefaultDomainParticipantTypes.hpp>

// This class takes in a order mass status request DDS message from the FIX
// gateway and enqueues it onto the request queue for OrderMassStatusService to
// handle.
class OrderMassStatusRequestDataReaderListener : public DataReaderListener {
public:
  OrderMassStatusRequestDataReaderListener(
      const OrderMassStatusRequestQueuePtr
          &order_mass_status_request_queue_ptr);

  virtual void on_data_available(DataReader *reader) override;

private:
  OrderMassStatusRequestQueuePtr m_order_mass_status_request_queue_ptr;
};