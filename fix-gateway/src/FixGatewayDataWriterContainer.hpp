#pragma once

#include "DefaultDomainParticipantTypes.hpp"

struct DataWriterContainer {
  // Data writer to send a log on request to the data service to handle
  // authentication. This data writer goes from FIX gateway -> data service.
  DataWriterPtr logOnDW;

  // Data writer to send a new order single request to the matching engine for
  // order book management. This data writer goes from FIX gateway -> matching
  // engine.
  DataWriterPtr newOrderSingleDW;

  // Data writer to send a order cancel request to the matching engine for order
  // book management. This data writer goes from FIX gateway -> matching engine.
  DataWriterPtr orderCancelRequestDW;

  // Data writer to send a order mass cancel request to the matching engine for
  // order book management. This data writer goes from FIX gateway -> matching
  // engine.
  DataWriterPtr orderMassCancelRequestDW;

  // Data writer to send a security list request to the data service to expose
  // stock exchange security information to the user. This data writer goes from
  // FIX gateway -> data service.
  DataWriterPtr securityListRequestDW;

  // Data writer to send a market data request to the data service to expose
  // stock exchange market data information to the user. This data writer goes
  // from FIX gateway -> data service.
  DataWriterPtr marketDataRequestDW;

  // Data writer to send a order mass status request to the data service to
  // expose stock exchange order status information to the user. This data
  // writer goes from FIX gateway -> data service.
  DataWriterPtr orderMassStatusRequestDW;
};