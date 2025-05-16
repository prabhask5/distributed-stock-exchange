#include "MarketDataSnapshotFullRefreshDataReaderListener.hpp"
#include "FixApplication.hpp"
#include "FixMsgTranslatorServiceTypes.hpp"
#include <HeaderAdapter.hpp>
#include <quickfix/fix50/MarketDataSnapshotFullRefresh.h>

TranslatorFunc<DistributedStockExchange_MarketDataSnapshotFullRefresh::
                   MarketDataSnapshotFullRefresh> const
    market_data_full_snapshot_translator =
        [](FixApplication &app,
           DistributedStockExchange_MarketDataSnapshotFullRefresh::
               MarketDataSnapshotFullRefresh
                   &market_data_snapshot_full_refresh) {
          FIX::Message market_data_snapshot_full_refresh_message;

          market_data_snapshot_full_refresh.fix_header().SendingTime(0);
          market_data_snapshot_full_refresh.fix_header().TargetCompID(
              market_data_snapshot_full_refresh.Destination());
          market_data_snapshot_full_refresh.fix_header().SenderCompID(
              market_data_snapshot_full_refresh.DestinationUser());

          HeaderAdapter::DDS2FIX(
              market_data_snapshot_full_refresh.fix_header(),
              market_data_snapshot_full_refresh_message.getHeader());

          FIX::Symbol symbol(market_data_snapshot_full_refresh.Symbol());
          FIX::SecurityExchange security_exchange(
              market_data_snapshot_full_refresh.SecurityExchange());

          market_data_snapshot_full_refresh_message.setField(symbol);
          market_data_snapshot_full_refresh_message.setField(security_exchange);

          for (int snapshot_refresh = 0;
               snapshot_refresh <
               market_data_snapshot_full_refresh.c_NoMDEntries().size();
               ++snapshot_refresh) {
            DistributedStockExchange_MarketDataSnapshotFullRefresh::NoMDEntries
                entry = market_data_snapshot_full_refresh
                            .c_NoMDEntries()[snapshot_refresh];

            FIX50::MarketDataSnapshotFullRefresh::NoMDEntries fix_entry;

            // Copy the data from the DDS message.
            FIX::MDEntryType entry_type(entry.MDEntryType());
            FIX::MDEntryPx entry_px(entry.MDEntryPx());
            FIX::MDEntrySize entry_size(entry.MDEntrySize());

            // Paste the data into the FIX message.
            fix_entry.setField(entry_type);
            fix_entry.setField(entry_px);
            fix_entry.setField(entry_size);

            market_data_snapshot_full_refresh_message.addGroup(fix_entry);
          }

          app.publish_to_client(market_data_snapshot_full_refresh_message);
        };

MarketDataSnapshotFullRefreshDataReaderListener::
    MarketDataSnapshotFullRefreshDataReaderListener(FixApplication &app)
    : m_translator_service(
          app, market_data_full_snapshot_translator,
          "MarketDataSnapshotFullRefreshDataReaderListenerTranslator") {}

void MarketDataSnapshotFullRefreshDataReaderListener::on_data_available(
    DataReader *reader) {
  DistributedStockExchange_MarketDataSnapshotFullRefresh::
      MarketDataSnapshotFullRefresh market_data_snapshot_full_refresh;
  eprosima::fastdds::dds::SampleInfo info;

  if (reader->take_next_sample(&market_data_snapshot_full_refresh, &info) ==
      eprosima::fastdds::dds::RETCODE_OK) {
    if (info.valid_data)
      m_translator_service.enqueue_dds_message(
          market_data_snapshot_full_refresh);
  }
}
