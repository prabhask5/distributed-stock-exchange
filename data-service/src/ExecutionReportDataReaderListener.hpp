#pragma once

#include "OrderMassStatusServiceTypes.hpp"
#include <DefaultDomainParticipantTypes.hpp>

// This class takes in execution report DDS messages from the matching engine
// and updates the internal execution reports map cache for
// OrderMassStatusService to use to handle order mass status requests.
class ExecutionReportDataReaderListener : public DataReaderListener {
public:
  ExecutionReportDataReaderListener(
      const SymbolToOrderExecutionReportsMapPtr
          &symbol_to_order_execution_reports_map_ptr);

  virtual void on_data_available(DataReader *reader) override;

private:
  SymbolToOrderExecutionReportsMapPtr
      m_symbol_to_order_execution_reports_map_ptr;
};