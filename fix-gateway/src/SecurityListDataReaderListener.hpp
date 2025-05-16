#pragma once

#include "FixMsgTranslatorService.hpp"
#include <DefaultDomainParticipantTypes.hpp>
#include <SecurityList.hpp>

// This class takes in initial market metadata, specifically a list of what
// securities are in the market (which are the different stock tickers that are
// in the market). This information comes from the data service. This class
// takes that DDS message, translates it into FIX, and surfaces it to the end
// user.
class SecurityListDataReaderListener : public DataReaderListener {
public:
  SecurityListDataReaderListener(FixApplication &app);

  virtual void on_data_available(DataReader *reader) override;

private:
  FixMsgTranslatorService<DistributedStockExchange_SecurityList::SecurityList>
      m_translator_service;
};