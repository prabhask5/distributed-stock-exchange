#pragma once

#include "DataServiceDataWriterContainerTypes.hpp"
#include "OrderMassStatusServiceTypes.hpp"
#include "SQLiteConnectionTypes.hpp"
#include <thread>

class OrderMassStatusService {
public:
  OrderMassStatusService(
      const DataWriterContainerPtr &data_writer_container_ptr,
      OrderMassStatusRequestQueuePtr order_mass_status_request_queue_ptr,
      SymbolToOrderExecutionReportsMapPtr
          symbol_to_order_execution_reports_map_ptr);

  ~OrderMassStatusService();

  void service();

  void process_order_mass_status_service_request(
      OrderMassStatusRequestPtr order_mass_status_request_ptr);

private:
  DataWriterContainerPtr m_data_writer_container_ptr;
  OrderMassStatusRequestQueuePtr m_order_mass_status_request_queue_ptr;
  SymbolToOrderExecutionReportsMapPtr
      m_symbol_to_order_execution_reports_map_ptr;
  std::atomic<bool> m_is_running;
  std::thread m_service_thread;
};