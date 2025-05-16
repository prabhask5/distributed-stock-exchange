#include "MarketDataIncrementalRefreshDataReaderListener.hpp"
#include "FixApplication.hpp"
#include "FixMsgTranslatorServiceTypes.hpp"
#include <HeaderAdapter.hpp>
#include <quickfix/fix50/MarketDataIncrementalRefresh.h>

TranslatorFunc<DistributedStockExchange_MarketDataIncrementalRefresh::
                   MarketDataIncrementalRefresh> const
    market_data_refresh_translator =
        [](FixApplication &app,
           DistributedStockExchange_MarketDataIncrementalRefresh::
               MarketDataIncrementalRefresh &market_data_refresh) {
          FIX::Message market_data_incremental_refresh_message;

          market_data_refresh.fix_header().SendingTime(0);
          HeaderAdapter::DDS2FIX(
              market_data_refresh.fix_header(),
              market_data_incremental_refresh_message.getHeader());

          // Iterate through the market data entries (representing each
          // incremental update).
          for (int incremental_update = 0;
               incremental_update < market_data_refresh.c_NoMDEntries().size();
               incremental_update++) {
            DistributedStockExchange_MarketDataIncrementalRefresh::NoMDEntries
                entry = market_data_refresh.c_NoMDEntries()[incremental_update];

            // In the case this market data entry doesn't do anything, we skip.
            if (entry.MDUpdateAction() == 0)
              continue;

            FIX50::MarketDataIncrementalRefresh::NoMDEntries fix_entry;

            // Copy the data from the DDS market data entry.
            FIX::MDUpdateAction update_action(entry.MDUpdateAction());
            FIX::Symbol symbol(entry.Symbol());
            FIX::SecurityExchange security_exchange(entry.SecurityExchange());
            FIX::MDEntryType entry_type(entry.MDEntryType());
            FIX::MDEntryPx entry_px(entry.MDEntryPx());
            FIX::MDEntrySize entry_size(entry.MDEntrySize());

            // Paste the data into the FIX market data entry.
            fix_entry.setField(update_action);
            fix_entry.setField(symbol);
            fix_entry.setField(security_exchange);
            fix_entry.setField(entry_type);
            fix_entry.setField(entry_px);
            fix_entry.setField(entry_size);

            market_data_incremental_refresh_message.addGroup(fix_entry);
          }

          app.broadcast_to_clients(market_data_incremental_refresh_message);
        };

MarketDataIncrementalRefreshDataReaderListener::
    MarketDataIncrementalRefreshDataReaderListener(FixApplication &app)
    : m_translator_service(
          app, market_data_refresh_translator,
          "MarketDataIncrementalRefreshDataReaderListenerTranslator") {}

void MarketDataIncrementalRefreshDataReaderListener::on_data_available(
    DataReader *reader) {
  DistributedStockExchange_MarketDataIncrementalRefresh::
      MarketDataIncrementalRefresh market_data_refresh;
  eprosima::fastdds::dds::SampleInfo info;

  if (reader->take_next_sample(&market_data_refresh, &info) ==
      eprosima::fastdds::dds::RETCODE_OK) {
    if (info.valid_data)
      m_translator_service.enqueue_dds_message(market_data_refresh);
  }
}