#pragma once

#include "FixMsgTranslatorService.hpp"
#include <DefaultDomainParticipantTypes.hpp>
#include <Logout.hpp>

// This class takes in a Logout message from the data service for
// authentication. It takes in those DDS messages, translates them into FIX
// messages, and processes the DDS logout through FIX to send a rejection
// message to the user.
class LogoutDataReaderListener : public DataReaderListener {
public:
  LogoutDataReaderListener(FixApplication &app);

  virtual void on_data_available(DataReader *reader) override;

private:
  FixMsgTranslatorService<DistributedStockExchange_Logout::Logout>
      m_translator_service;
};