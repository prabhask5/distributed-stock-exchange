#pragma once

#include "MatchingEngineDataWriterContainer.hpp"
#include <ExecutionReport.hpp>
#include <ExecutionReportLogger.hpp>
#include <LoggerHelper.hpp>

// Helper class to publish execution reports via FastDDS, sending to FIX
// gateway. Execution reports are populated within more specific classes (i.e
// Order, OrderException), then actually sent via this class. Note: this is
// necessary since multiple market event handlers are abstracted from the market
// class, this class keeps me from repeating the publish logic in each event
// handler class.
class ExecutionReportPublisher {
public:
  ExecutionReportPublisher(DataWriterContainerPtr data_writer_container_ptr);

  void publish_report(DistributedStockExchange_ExecutionReport::ExecutionReport
                          &executionReport);

private:
  std::string get_id();

private:
  static uint32_t s_id_seed;
  DataWriterContainerPtr m_data_writer_container_ptr;
};

using ExecutionReportPublisherPtr = std::shared_ptr<ExecutionReportPublisher>;