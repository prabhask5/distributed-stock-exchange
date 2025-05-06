#include <OrderException.hpp>

OrderException::OrderException(const std::string &order_id, const int reason)
    : m_order_id(order_id), m_reason(reason) {
  std::ostringstream oss;
  oss << "Order " << m_order_id << " failed for reason error code " << m_reason;
  m_error_message = oss.str();
}

OrderException::OrderException(
    const DistributedStockExchange_NewOrderSingle::NewOrderSingle
        &new_order_single,
    const int reason)
    : OrderException::OrderException(new_order_single.ClOrdID(), reason) {}

const char *OrderException::what() const { return m_error_message.c_str(); }

void OrderException::populate_execution_report(
    DistributedStockExchange_ExecutionReport::ExecutionReport
        &execution_report) {
  execution_report.OrderID(m_order_id);
  execution_report.ExecType(FIX::ExecType_REJECTED);
  execution_report.OrdStatus(FIX::OrdStatus_REJECTED);
  execution_report.OrdRejReason(m_reason);
}

void OrderException::populate_order_cancel_reject(
    DistributedStockExchange_OrderCancelReject::OrderCancelReject
        &order_cancel_reject) {
  order_cancel_reject.CxlRejResponseTo(m_reason);
}