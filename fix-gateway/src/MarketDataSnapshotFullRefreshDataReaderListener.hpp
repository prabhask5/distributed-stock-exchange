#pragma once

#include "FixMsgTranslatorService.hpp"
#include <DefaultDomainParticipantTypes.hpp>
#include <MarketDataSnapshotFullRefresh.hpp>

// This class represents a listener for the market data snapshot data reader. It
// takes in market price data for each stock ticker symbol from the data service
// and surfaces it to the end user, translating from DDS to FIX.
class MarketDataSnapshotFullRefreshDataReaderListener
    : public DataReaderListener {
public:
  MarketDataSnapshotFullRefreshDataReaderListener(FixApplication &app);

  virtual void on_data_available(DataReader *reader) override;

private:
  FixMsgTranslatorService<
      DistributedStockExchange_MarketDataSnapshotFullRefresh::
          MarketDataSnapshotFullRefresh>
      m_translator_service;
};