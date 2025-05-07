#include "DepthEventHandler.hpp"

DepthEventHandler::DepthEventHandler(Market *market) : m_market(market) {}

void DepthEventHandler::on_depth_change(const DepthOrderBook *depth_order_book,
                                        const DepthLevel *depth_level) {
  std::shared_ptr<MarketDataUpdate> market_data_update =
      std::make_shared<MarketDataUpdate>();

  // Fill in simple market data update metadata.
  market_data_update->symbol = depth_order_book->get_symbol();
  market_data_update->refresh_data.Source("MATCHING_ENGINE");
  market_data_update->refresh_data.fix_header().MsgType("X");
  market_data_update->refresh_data.c_NoMDEntries().resize(15);

  // Populate the inner market data entry.
  DistributedStockExchange_MarketDataIncrementalRefresh::NoMDEntries md_entry;
  md_entry.Symbol(depth_order_book->get_symbol());
  md_entry.SecurityExchange(m_market->get_market_name());
  md_entry.MDUpdateAction('0');

  size_t md_entry_index = 0;

  for (auto &level : depth_order_book->get_levels()) {
    md_entry.MDEntrySize(level.get_aggregate_quantity());

    if (md_entry.MDEntrySize() == 0) {
      md_entry.MDEntryPx(0);
    } else {
      md_entry.MDEntryPx(level.get_price());
    }

    md_entry.MDEntryType(md_entry_index < MARKET_DATA_PRICE_DEPTH
                             ? FIX::MDEntryType_BID
                             : FIX::MDEntryType_OFFER);
    market_data_update->refresh_data.c_NoMDEntries()[md_entry_index++] =
        md_entry;
  }

  // Log relevant info about the market incremental refresh data about to be
  // sent over DDS.
  std::stringstream ss;
  MarketDataIncrementalRefreshLogger::log(ss, market_data_update->refresh_data);
  LOG4CXX_INFO(logger, "MarketDataIncrementalRefresh : [" << ss.str() << "]");
  std::cout << "Update : " << ss.str() << std::endl;
}