#include "MarketDataSnapshotFullRefreshDataReaderListener.hpp"
#include "Market.hpp"
#include <DefaultDomainParticipantConstants.hpp>
#include <LoggerHelper.hpp>
#include <MarketDataSnapshotFullRefresh.hpp>
#include <MarketDataSnapshotFullRefreshLogger.hpp>

MarketDataSnapshotFullRefreshDataReaderListener::
    MarketDataSnapshotFullRefreshDataReaderListener(const MarketPtr &market_ptr)
    : m_market_ptr(market_ptr) {}

void MarketDataSnapshotFullRefreshDataReaderListener::on_data_available(
    DataReader *reader) {
  DistributedStockExchange_MarketDataSnapshotFullRefresh::
      MarketDataSnapshotFullRefresh market_data_snapshot_full_refresh;
  eprosima::fastdds::dds::SampleInfo info;

  if (reader->take_next_sample(&market_data_snapshot_full_refresh, &info) ==
      eprosima::fastdds::dds::RETCODE_OK) {
    if (info.valid_data) {
      log_info<std::stringstream, MarketDataSnapshotFullRefreshLogger,
               DistributedStockExchange_MarketDataSnapshotFullRefresh::
                   MarketDataSnapshotFullRefresh>(
          logger, market_data_snapshot_full_refresh,
          "MarketDataSnapshotFullRefresh");

      for (int index = 0;
           index < market_data_snapshot_full_refresh.c_NoMDEntries().size();
           index++) {
        {
          auto symbol = market_data_snapshot_full_refresh.Symbol();
          m_market_ptr->set_market_price(
              symbol, market_data_snapshot_full_refresh.c_NoMDEntries()[index]
                          .MDEntryPx());
        }
      }
    }
  }
}