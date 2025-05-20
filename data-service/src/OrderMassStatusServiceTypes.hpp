#pragma once

#include <boost/lockfree/spsc_queue.hpp>
#include <list>
#include <map>
#include <memory>

namespace DistributedStockExchange_OrderMassStatusRequest {
class OrderMassStatusRequest;
}

using OrderMassStatusRequestPtr = std::unique_ptr<
    DistributedStockExchange_OrderMassStatusRequest::OrderMassStatusRequest>;

// This is a special type of queue called a Single-Producer Single-Consumer
// (SPSC) lock-free queue. This queue adds items from one thread and consumes
// items from a DIFFERENT thread. This queue is lock free and doesn't use atomic
// operations- this is because there is a buffer (head/tail) so the pointers for
// the consumer/producer never cross, leading to no concurrency issues. In the
// context of the distributed stock exchange, this queue takes in order mass
// status requests from the data reader listener, and handles those requests
// using information from the matching engine.
using OrderMassStatusRequestQueue =
    boost::lockfree::spsc_queue<OrderMassStatusRequestPtr,
                                boost::lockfree::capacity<1024>>;
using OrderMassStatusRequestQueuePtr =
    std::shared_ptr<OrderMassStatusRequestQueue>;

namespace DistributedStockExchange_ExecutionReport {
class ExecutionReport;
}

// This is a pointer to an execution report, which represents the status info
// from a trade for an order.
using ExecutionReportPtr =
    std::shared_ptr<DistributedStockExchange_ExecutionReport::ExecutionReport>;

// This a list containing multiple execution reports. One order can have many
// execution reports if it gets partially filled multiple times.
using ExecutionReportList = std::list<ExecutionReportPtr>;
using ExecutionReportListPtr = std::shared_ptr<ExecutionReportList>;

// Maps the list of execution reports to an order by the order id.
using OrderToExecutionReportsMap =
    std::unordered_map<std::string, ExecutionReportListPtr>;
using OrderToExecutionReportsMapPtr =
    std::shared_ptr<OrderToExecutionReportsMap>;

// Maps a stock ticker symbol, to a map of orders with their execution reports.
using UserToOrderExecutionReportsMap =
    std::unordered_map<std::string, OrderToExecutionReportsMapPtr>;
using UserToOrderExecutionReportsMapPtr =
    std::shared_ptr<UserToOrderExecutionReportsMap>;