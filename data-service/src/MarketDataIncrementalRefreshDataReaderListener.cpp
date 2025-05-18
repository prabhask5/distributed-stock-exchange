#include "MarketDataIncrementalRefreshDataReaderListener.hpp"
#include "Instrument.hpp"
#include "MarketDataIncrementalRefreshLogger.hpp"
#include <DefaultDomainParticipantConstants.hpp>
#include <MarketDataIncrementalRefresh.hpp>

MarketDataIncrementalRefreshDataReaderListener::
    MarketDataIncrementalRefreshDataReaderListener(
        const IncrementalRefreshMapPtr &incremental_refresh_map_ptr)
    : m_incremental_refresh_map_ptr(incremental_refresh_map_ptr) {}

void MarketDataIncrementalRefreshDataReaderListener::on_data_available(
    DataReader *reader) {
  DistributedStockExchange_MarketDataIncrementalRefresh::
      MarketDataIncrementalRefresh market_data_refresh;
  eprosima::fastdds::dds::SampleInfo info;

  if (reader->take_next_sample(&market_data_refresh, &info) ==
      eprosima::fastdds::dds::RETCODE_OK) {
    if (info.valid_data) {
      std::stringstream ss;
      MarketDataIncrementalRefreshLogger::log(ss, market_data_refresh);
      LOG4CXX_INFO(logger,
                   "MarketDataIncrementalRefresh : [" << ss.str() << "]");

      // Make a copy of the incremental refresh map pointer to copy all of the
      // market data entries from the DDS message.
      IncrementalRefreshMap current_market_data_snapshot;

      for (int incremental_update_index = 0;
           incremental_update_index <
           market_data_refresh.c_NoMDEntries().size();
           ++incremental_update_index) {
        auto md_entry =
            market_data_refresh.c_NoMDEntries()[incremental_update_index];

        auto market_name = md_entry.SecurityExchange();
        auto symbol = md_entry.Symbol();

        Instrument instrument(market_name, symbol);

        auto md_entry_list_itr = current_market_data_snapshot.find(instrument);

        // If this entry list doesn't already exist, we make one, else we just
        // add to it.
        if (md_entry_list_itr == current_market_data_snapshot.end()) {
          MarketDataEntryList entry_list;
          entry_list.push_back(md_entry);
          current_market_data_snapshot.emplace(instrument, entry_list);
        } else
          md_entry_list_itr->second.push_back(md_entry);
      }

      // Copy all the market data entry lists into the member variable to use in
      // MarketDataService.
      for (auto &md_entry : current_market_data_snapshot)
        (*m_incremental_refresh_map_ptr)[md_entry.first] = md_entry.second;
    }
  }
}