#pragma once

#include <DefaultDomainParticipantTypes.hpp>

struct DataWriterContainer {
  // Data writer to send a log on response to the FIX gateway after processing
  // authentication. This data writer goes from data service -> FIX gateway.
  DataWriterPtr logOnDW;

  // Data writer to send a log out response ot the FIX gateway after processing
  // authentication. This data writer goes from data service -> FIX gateway.
  DataWriterPtr logOutDW;

  // Data writer to send a stored execution report to the FIX gateway to surface
  // to the user. This data writer goes from data service -> FIX gateway.
  DataWriterPtr executionReportDW;

  // Data writer to send a list of securities to the matching engine for
  // initialization. This data writer goes from data service -> matching engine.
  DataWriterPtr securityListDW;

  // Data writer to send a stored market data snapshop to the FIX gateway to
  // surface to the user. This data writer goes from data service -> FIX
  // gateway.
  DataWriterPtr marketDataSnapshotFullRefreshDW;
};