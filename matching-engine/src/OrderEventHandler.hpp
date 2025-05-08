#pragma once

#include "ExecutionReportPublisher.hpp"
#include "Order.hpp"
#include <ExecutionReport.hpp>
#include <quickfix/Message.h>

// This class contains several callback functions that run logic on certain
// state changes for an order within the order book.
class OrderEventHandler {
public:
  OrderEventHandler(ExecutionReportPublisherPtr execution_report_publisher);

  void on_order_accept(const OrderPtr &order);

  void on_order_reject(const OrderPtr &order, const char *reason);

  void on_order_fill(const OrderPtr &order, const OrderPtr &matched_order,
                     Quantity quantity_filled, Cost fill_cost);

  void on_order_cancelled(const OrderPtr &order);

  void on_order_cancel_reject(const OrderPtr &order, const char *reason);

private:
  ExecutionReportPublisherPtr m_execution_report_publisher;
};

using OrderEventHandlerPtr = std::shared_ptr<OrderEventHandler>;