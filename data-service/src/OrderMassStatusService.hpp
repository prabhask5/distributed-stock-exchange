#pragma once

#include "DataServiceDataWriterContainerTypes.hpp"
#include "OrderMassStatusServiceTypes.hpp"
#include "SQLiteConnectionTypes.hpp"
#include <quickfix/DatabaseConnectionID.h>
#include <thread>

// This service spins a separate thread to handle all incoming order mass status
// requests to the data service. This class accomplishes that by storing
// execution reports in memory from the execution report data reader listener,
// and copying them into the DDS response.
class OrderMassStatusService {
public:
  OrderMassStatusService(
      const FIX::DatabaseConnectionID &database_connection_id,
      const DataWriterContainerPtr &data_writer_container_ptr,
      OrderMassStatusRequestQueuePtr order_mass_status_request_queue_ptr,
      UserToOrderExecutionReportsMapPtr
          user_to_order_execution_reports_map_ptr);

  ~OrderMassStatusService();

  void initialize();

  void service();

  void process_order_mass_status_service_request(
      OrderMassStatusRequestPtr order_mass_status_request_ptr);

private:
  SQLiteConnectionPtr m_sqlite_connection_ptr;
  DataWriterContainerPtr m_data_writer_container_ptr;
  OrderMassStatusRequestQueuePtr m_order_mass_status_request_queue_ptr;
  UserToOrderExecutionReportsMapPtr m_user_to_order_execution_reports_map_ptr;
  std::atomic<bool> m_is_running;
  std::thread m_service_thread;
};