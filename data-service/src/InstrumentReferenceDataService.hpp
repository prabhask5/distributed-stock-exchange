#pragma once

#include "DataServiceDataWriterContainerTypes.hpp"
#include "InstrumentReferenceDataServiceTypes.hpp"
#include "SQLiteConnectionTypes.hpp"
#include <quickfix/DatabaseConnectionID.h>
#include <thread>

// This service spins a separate thread to handle all incoming security list
// requests to the data service. This service uses instrument reference data
// information parsed from the database alongside a security list request queue
// to handle incoming requests with stored data.
class InstrumentReferenceDataService {
public:
  InstrumentReferenceDataService(
      const FIX::DatabaseConnectionID &database_connection_id,
      const DataWriterContainerPtr &data_writer_container_ptr,
      SecurityListRequestQueuePtr security_list_request_queue_ptr);

  ~InstrumentReferenceDataService();

  void initialize();

  void service();

  void
  process_ref_data_request(SecurityListRequestPtr security_list_request_ptr);

private:
  SQLiteConnectionPtr m_sqlite_connection_ptr;
  DataWriterContainerPtr m_data_writer_container_ptr;
  SecurityListRequestQueuePtr m_security_list_request_queue_ptr;
  InstrumentMap m_instrument_map;
  UserInstrumentListMap m_user_instrument_list_map;
  InstrumentIdToRefDataMap m_instrument_id_to_ref_data_map;
  std::atomic<bool> m_is_running;
  std::thread m_service_thread;
};