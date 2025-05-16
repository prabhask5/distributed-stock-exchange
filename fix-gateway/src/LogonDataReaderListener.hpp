#pragma once

#include "FixMsgTranslatorService.hpp"
#include <DefaultDomainParticipantTypes.hpp>
#include <Logon.hpp>

// This class takes in a Logon message from the data service for authentication.
// It takes in those DDS messages, translates them into FIX messages, and
// processes the DDS logon through FIX to create a new active FIX session.
class LogonDataReaderListener : public DataReaderListener {
public:
  LogonDataReaderListener(FixApplication &app);

  virtual void on_data_available(DataReader *reader) override;

private:
  FixMsgTranslatorService<DistributedStockExchange_Logon::Logon>
      m_translator_service;
};