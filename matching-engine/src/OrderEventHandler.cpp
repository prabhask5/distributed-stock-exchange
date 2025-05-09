#include "OrderEventHandler.hpp"
#include "ExecutionReportPublisher.hpp"
#include "Order.hpp"
#include <ExecutionReport.hpp>
#include <quickfix/Message.h>

OrderEventHandler::OrderEventHandler(
    ExecutionReportPublisherPtr execution_report_publisher)
    : m_execution_report_publisher(execution_report_publisher) {}

void OrderEventHandler::on_order_accept(const OrderPtr &order) {
  order->on_accepted();

  DistributedStockExchange_ExecutionReport::ExecutionReport execution_report;
  order->populate_execution_report(execution_report, FIX::ExecType_NEW);

  m_execution_report_publisher->publish_report(execution_report);
}

void OrderEventHandler::on_order_reject(const OrderPtr &order,
                                        const char *reason) {
  order->on_rejected(reason);

  DistributedStockExchange_ExecutionReport::ExecutionReport execution_report;
  order->populate_execution_report(execution_report, FIX::ExecType_REJECTED);

  m_execution_report_publisher->publish_report(execution_report);
}

void OrderEventHandler::on_order_fill(const OrderPtr &order,
                                      const OrderPtr &matched_order,
                                      Quantity quantity_filled,
                                      Cost fill_cost) {
  order->on_filled(quantity_filled, fill_cost);
  matched_order->on_filled(quantity_filled, fill_cost);

  DistributedStockExchange_ExecutionReport::ExecutionReport execution_report;
  DistributedStockExchange_ExecutionReport::ExecutionReport
      matched_order_execution_report;

  Quantity order_quant_in_market = order->get_quantity_in_market();
  Quantity matched_order_quant_in_market =
      matched_order->get_quantity_in_market();

  order->populate_execution_report(
      execution_report, order_quant_in_market > 0 ? FIX::ExecType_PARTIAL_FILL
                                                  : FIX::ExecType_FILL);
  matched_order->populate_execution_report(matched_order_execution_report,
                                           matched_order_quant_in_market > 0
                                               ? FIX::ExecType_PARTIAL_FILL
                                               : FIX::ExecType_FILL);

  auto last_px = fill_cost / quantity_filled;
  execution_report.LastPx(last_px);
  matched_order_execution_report.LastPx(last_px);

  execution_report.LastQty(quantity_filled);
  matched_order_execution_report.LastQty(quantity_filled);

  m_execution_report_publisher->publish_report(execution_report);
  m_execution_report_publisher->publish_report(matched_order_execution_report);
}

void OrderEventHandler::on_order_cancelled(const OrderPtr &order) {
  order->on_cancelled();

  DistributedStockExchange_ExecutionReport::ExecutionReport execution_report;
  order->populate_execution_report(execution_report, FIX::ExecType_CANCELED);

  m_execution_report_publisher->publish_report(execution_report);
}

void OrderEventHandler::on_order_cancel_reject(const OrderPtr &order,
                                               const char *reason) {
  order->on_cancel_rejected(reason);
}