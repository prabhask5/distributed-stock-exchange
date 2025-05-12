#pragma once

#include "ExecutionReportPublisherTypes.hpp"
#include "OrderTypes.hpp"

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

  void on_stop_order_triggered(const OrderPtr &order);

private:
  ExecutionReportPublisherPtr m_execution_report_publisher;
};