#pragma once

#include "DataServiceDataWriterContainerTypes.hpp"
#include "MarketDataServiceTypes.hpp"
#include "SQLiteConnectionTypes.hpp"
#include <MarketDataSnapshotFullRefresh.hpp>
#include <quickfix/DatabaseConnectionID.h>
#include <thread>

// This service spins a separate thread to handle all incoming market data
// requests into the data service. The data service stores instrument data it
// gets from the matching engine, and forwards that information to the FIX
// gateway to surface to the user.
class MarketDataService {
public:
  MarketDataService(const FIX::DatabaseConnectionID &database_connection_id,
                    IncrementalRefreshMapPtr incremental_refresh_map_ptr,
                    MarketDataRequestQueuePtr market_data_request_queue);

  ~MarketDataService();

  void initialize();

  void service();

private:
  void
  process_market_data_request(MarketDataRequestPtr market_data_request_ptr);

  bool populate_market_data_snapshot_full_refresh(
      const Instrument &instrument,
      DistributedStockExchange_MarketDataSnapshotFullRefresh::
          MarketDataSnapshotFullRefresh &market_data_snapshot_full_refresh);

private:
  SQLiteConnectionPtr m_sqlite_connection_ptr;
  DataWriterContainerPtr m_data_writer_container_ptr;
  IncrementalRefreshMapPtr m_incremental_refresh_map_ptr;
  MarketDataRequestQueuePtr m_market_data_request_queue;
  std::atomic<bool> m_is_running;
  std::thread m_service_thread;
};