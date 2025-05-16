#pragma once

#include "FixMsgTranslatorService.hpp"
#include <DefaultDomainParticipantTypes.hpp>
#include <MarketDataIncrementalRefresh.hpp>

// Represents information related to the incremental market data refresh sent
// from the data service to the FIX agteway. An incremental market data
// refresh represents the changes in the market data during the duration from
// when the last refresh was sent to now.
// This class takes that DDS message, translates it into a FIX message, and
// surfaces it to the end user.
class MarketDataIncrementalRefreshDataReaderListener
    : public DataReaderListener {
public:
  MarketDataIncrementalRefreshDataReaderListener(FixApplication &app);

  virtual void on_data_available(DataReader *reader) override;

private:
  FixMsgTranslatorService<
      DistributedStockExchange_MarketDataIncrementalRefresh::
          MarketDataIncrementalRefresh>
      m_translator_service;
};