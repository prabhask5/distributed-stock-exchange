#pragma once

#include "FixMsgTranslatorService.hpp"
#include <DefaultDomainParticipantTypes.hpp>
#include <ExecutionReport.hpp>

// This class takes in execution report DDS messages from the matching engine
// which represent the status of currently executing orders. It takes in those
// DDS messages, translates them into FIX messages, and sends them to the end
// user.
class ExecutionReportDataReaderListener : public DataReaderListener {
public:
  ExecutionReportDataReaderListener(FixApplication &app);

  virtual void on_data_available(DataReader *reader) override;

private:
  FixMsgTranslatorService<
      DistributedStockExchange_ExecutionReport::ExecutionReport>
      m_translator_service;
};