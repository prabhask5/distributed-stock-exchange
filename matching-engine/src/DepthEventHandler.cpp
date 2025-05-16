#include "DepthEventHandler.hpp"
#include "DepthLevel.hpp"
#include "DepthOrderBook.hpp"
#include "DepthOrderBookConstants.hpp"
#include "Market.hpp"
#include "MarketDataIncrementalRefreshLogger.hpp"
#include "MarketDataUpdate.hpp"
#include "OrderBookStockStatistics.hpp"
#include "OrderBookStockStatisticsTypes.hpp"
#include <DefaultDomainParticipantConstants.hpp>
#include <MarketDataIncrementalRefresh.hpp>
#include <quickfix/Message.h>

DepthEventHandler::DepthEventHandler(
    const OrderBookStockStatsMapPtr &order_book_stats_map_ptr,
    const MarketDataPublisherQueuePtr &market_data_publisher_queue_ptr,
    const std::string &market_name)
    : m_order_book_stats_map_ptr(order_book_stats_map_ptr),
      m_market_data_publisher_queue_ptr(market_data_publisher_queue_ptr),
      m_market_name(market_name) {}

void set_market_data_stats_entry(
    DistributedStockExchange_MarketDataIncrementalRefresh::NoMDEntries
        &md_entry,
    const std::string &market_name, const std::string &symbol,
    const char md_action, const char md_entry_type, const float md_entry_px,
    const float md_entry_size) {
  md_entry.SecurityExchange(market_name);
  md_entry.Symbol(symbol);
  md_entry.MDUpdateAction(md_action);
  md_entry.MDEntryType(md_entry_type);

  if (md_entry_type == FIX::MDEntryType_TRADE_VOLUME) {
    md_entry.MDEntrySize(md_entry_size);
  } else {
    md_entry.MDEntryPx(md_entry_px);
  }
}

void DepthEventHandler::on_depth_change(
    const DepthOrderBook *depth_order_book) {
  std::shared_ptr<MarketDataUpdate> market_data_update =
      std::make_shared<MarketDataUpdate>();

  // Fill in simple market data update metadata.
  market_data_update->symbol = depth_order_book->get_symbol();
  market_data_update->refresh_data.Source("MATCHING_ENGINE");
  market_data_update->refresh_data.fix_header().MsgType("X");
  market_data_update->refresh_data.c_NoMDEntries().resize(
      3 * MARKET_DATA_PRICE_DEPTH);

  // Populate the inner market data entry.
  DistributedStockExchange_MarketDataIncrementalRefresh::NoMDEntries md_entry;
  md_entry.Symbol(depth_order_book->get_symbol());
  md_entry.SecurityExchange(m_market_name);
  md_entry.MDUpdateAction('0');

  size_t md_entry_index = 0;

  for (auto &level : depth_order_book->get_depth_levels()) {
    md_entry.MDEntrySize(level.get_total_quantity());

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

  int market_data_index =
      MARKET_DATA_PRICE_DEPTH * 2; // total length of depth levels array

  // Here we're writing all the information from the depth order book statistics
  // to the market data incremental refresh inner data entries.
  auto current_order_book_stats =
      m_order_book_stats_map_ptr->find(depth_order_book->get_symbol());

  if (current_order_book_stats != m_order_book_stats_map_ptr->end()) {
    std::string symbol = depth_order_book->get_symbol();

    if (current_order_book_stats->second->volume > 0) {
      // Populate the data related to the "trade" action with the market price.
      set_market_data_stats_entry(
          market_data_update->refresh_data.c_NoMDEntries()[market_data_index++],
          m_market_name, symbol, FIX::MDUpdateAction_NEW,
          FIX::MDEntryType_TRADE, depth_order_book->get_market_price(), 0);

      // Populate the data related to the trade volume with the "volume"
      // statistics.
      set_market_data_stats_entry(
          market_data_update->refresh_data.c_NoMDEntries()[market_data_index++],
          m_market_name, symbol, FIX::MDUpdateAction_NEW,
          FIX::MDEntryType_TRADE_VOLUME, 0,
          current_order_book_stats->second->volume);

      // Populate the data related to the opening price with the "open"
      // statistics.
      set_market_data_stats_entry(
          market_data_update->refresh_data.c_NoMDEntries()[market_data_index++],
          m_market_name, symbol, FIX::MDUpdateAction_NEW,
          FIX::MDEntryType_OPENING_PRICE,
          current_order_book_stats->second->open, 0);

      // Populate the data related to the low price with the "low" statistics.
      set_market_data_stats_entry(
          market_data_update->refresh_data.c_NoMDEntries()[market_data_index++],
          m_market_name, symbol, FIX::MDUpdateAction_NEW,
          FIX::MDEntryType_TRADING_SESSION_LOW_PRICE,
          current_order_book_stats->second->low, 0);

      // Populate the data related to the high price with the "high" statistics.
      set_market_data_stats_entry(
          market_data_update->refresh_data.c_NoMDEntries()[market_data_index++],
          m_market_name, symbol, FIX::MDUpdateAction_NEW,
          FIX::MDEntryType_TRADING_SESSION_HIGH_PRICE,
          current_order_book_stats->second->high, 0);
    } else {
      // Populate the data related to the opening price with the "high"
      // statistics.
      set_market_data_stats_entry(
          market_data_update->refresh_data.c_NoMDEntries()[market_data_index++],
          m_market_name, symbol, FIX::MDUpdateAction_NEW,
          FIX::MDEntryType_OPENING_PRICE,
          current_order_book_stats->second->high, 0);
    }
  }

  m_market_data_publisher_queue_ptr->push(market_data_update);
}