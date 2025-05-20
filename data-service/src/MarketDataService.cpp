#include "MarketDataService.hpp"
#include "DataServiceDataWriterContainer.hpp"
#include "Instrument.hpp"
#include "SQLiteConnection.hpp"
#include "SQLiteQuery.hpp"
#include <DefaultDomainParticipantConstants.hpp>
#include <LoggerHelper.hpp>
#include <MarketDataIncrementalRefresh.hpp>
#include <MarketDataRequest.hpp>
#include <MarketDataSnapshotFullRefreshLogger.hpp>
#include <quickfix/FixValues.h>

MarketDataService::MarketDataService(
    const FIX::DatabaseConnectionID &database_connection_id,
    const DataWriterContainerPtr &data_writer_container_ptr,
    IncrementalRefreshMapPtr incremental_refresh_map_ptr,
    MarketDataRequestQueuePtr market_data_request_queue_ptr)
    : m_data_writer_container_ptr(data_writer_container_ptr),
      m_incremental_refresh_map_ptr(incremental_refresh_map_ptr),
      m_market_data_request_queue_ptr(market_data_request_queue_ptr) {
  m_sqlite_connection_ptr =
      std::make_unique<SQLiteConnection>(database_connection_id);

  std::atomic_init(&m_is_running, true);
  m_service_thread = std::thread(&MarketDataService::service, this);

  initialize();
}

MarketDataService::~MarketDataService() {
  // Persist all the latest market data entries for a specific instrument
  // name/market name combination from the incremental refresh map.

  // Set up a SQLite query to update the database.
  std::string market_data_update_query_str =
      "UPDATE historical_prices SET last_price = ? WHERE instrument_name = ? "
      "AND market_name = ?;";
  SQLiteQuery market_data_update_query(market_data_update_query_str, false, {});
  for (const auto &entry_iter : *m_incremental_refresh_map_ptr) {
    const Instrument &instrument = entry_iter.first;
    int last_price = entry_iter.second.back().MDEntryPx();

    std::vector<std::string> parameters = {
        std::to_string(last_price), instrument.symbol, instrument.marketName};
    market_data_update_query.set_parameters(std::move(parameters));
    m_sqlite_connection_ptr->execute(market_data_update_query);
  }

  m_is_running.store(false);
  m_service_thread.join();
}

void MarketDataService::initialize() {
  // This query extracts the instrument name, market name, last traded price
  // (from the open price), of all instruments currently in the database. This
  // is basically used to determine the initial incremental refresh data that we
  // store in memory.
  std::string market_data_query_str =
      "SELECT instrument_name, market_name, last_price FROM historical_prices;";
  SQLiteQuery market_data_query(market_data_query_str, true, {});
  m_sqlite_connection_ptr->execute(market_data_query);

  for (int row = 0; row < market_data_query.get_num_rows(); ++row) {
    // Each row represents an individual instrument, with its data.

    std::string symbol = market_data_query.get_value(row, 0);
    std::string market = market_data_query.get_value(row, 1);
    int last_trade_price =
        std::atoi(market_data_query.get_value(row, 2).c_str());

    DistributedStockExchange_MarketDataIncrementalRefresh::NoMDEntries
        last_price_entry;
    last_price_entry.MDUpdateAction(FIX::MDUpdateAction_NEW);
    last_price_entry.MDEntryType(FIX::MDEntryType_OPENING_PRICE);
    last_price_entry.MDEntryPx(last_trade_price);
    last_price_entry.TimeInForce(1);

    m_incremental_refresh_map_ptr->emplace(
        Instrument(market, symbol),
        std::list<
            DistributedStockExchange_MarketDataIncrementalRefresh::NoMDEntries>{
            last_price_entry});
  }
}

void MarketDataService::service() {
  while (m_is_running.load()) {
    if (!m_sqlite_connection_ptr->is_connected()) {
      LOG4CXX_ERROR(logger, "Auth Service is not connected to the database.\n");
      return;
    }

    while (!m_market_data_request_queue_ptr->empty()) {
      MarketDataRequestPtr market_data_request_ptr;
      m_market_data_request_queue_ptr->pop(market_data_request_ptr);
      process_market_data_request(std::move(market_data_request_ptr));
    }

    std::this_thread::sleep_for(
        std::chrono::duration<long double, std::milli>(1000));
  }
}

void MarketDataService::process_market_data_request(
    MarketDataRequestPtr market_data_request_ptr) {
  for (int symbol_index = 0;
       symbol_index < market_data_request_ptr->c_NoRelatedSym().size();
       ++symbol_index) {
    DistributedStockExchange_MarketDataSnapshotFullRefresh::
        MarketDataSnapshotFullRefresh market_data_snapshot_full_refresh;

    market_data_snapshot_full_refresh.fix_header().BeginString(
        market_data_request_ptr->fix_header().BeginString());
    market_data_snapshot_full_refresh.fix_header().MsgType("W");

    market_data_snapshot_full_refresh.Source(
        market_data_request_ptr->Destination());
    market_data_snapshot_full_refresh.Destination(
        market_data_request_ptr->Source());
    market_data_snapshot_full_refresh.DestinationUser(
        market_data_request_ptr->SourceUser());

    std::string market_name =
        market_data_request_ptr->c_NoRelatedSym()[symbol_index]
            .SecurityExchange();
    std::string symbol =
        market_data_request_ptr->c_NoRelatedSym()[symbol_index].Symbol();

    if (populate_market_data_snapshot_full_refresh(
            Instrument(market_name, symbol),
            market_data_snapshot_full_refresh)) {
      eprosima::fastdds::dds::ReturnCode_t code =
          m_data_writer_container_ptr->marketDataSnapshotFullRefreshDW->write(
              &market_data_snapshot_full_refresh);
      if (code != eprosima::fastdds::dds::RETCODE_OK)
        LOG4CXX_ERROR(logger,
                      "Market Data Snapshot Data Write returned: " << code);
    }
  }
}

bool MarketDataService::populate_market_data_snapshot_full_refresh(
    const Instrument &instrument,
    DistributedStockExchange_MarketDataSnapshotFullRefresh::
        MarketDataSnapshotFullRefresh &market_data_snapshot_full_refresh) {
  market_data_snapshot_full_refresh.Symbol(instrument.symbol);
  market_data_snapshot_full_refresh.SecurityExchange(instrument.marketName);

  // Now we copy all the market data entries for this instrument from the stored
  // info in the cache.

  auto md_entry_iter = m_incremental_refresh_map_ptr->find(instrument);
  if (md_entry_iter == m_incremental_refresh_map_ptr->end())
    return false;

  market_data_snapshot_full_refresh.c_NoMDEntries().resize(
      md_entry_iter->second.size());

  auto md_entry_index = 0;
  for (const auto &md_entry : md_entry_iter->second) {
    market_data_snapshot_full_refresh.c_NoMDEntries()[md_entry_index]
        .MDEntryType(md_entry.MDEntryType());
    market_data_snapshot_full_refresh.c_NoMDEntries()[md_entry_index].MDEntryPx(
        md_entry.MDEntryPx());
    market_data_snapshot_full_refresh.c_NoMDEntries()[md_entry_index]
        .MDEntrySize(md_entry.MDEntrySize());
    market_data_snapshot_full_refresh.c_NoMDEntries()[md_entry_index]
        .TimeInForce(0);

    md_entry_index++;
  }

  log_debug<std::stringstream, MarketDataSnapshotFullRefreshLogger,
            DistributedStockExchange_MarketDataSnapshotFullRefresh::
                MarketDataSnapshotFullRefresh>(
      logger, market_data_snapshot_full_refresh,
      "MarketDataSnapshotFullRefresh");
  return true;
}