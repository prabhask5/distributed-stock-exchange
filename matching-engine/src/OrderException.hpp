#pragma once

#include <ExecutionReport.hpp>
#include <NewOrderSingle.hpp>
#include <OrderCancelReject.hpp>
#include <exception>
#include <quickfix/FixValues.h>
#include <sstream>
#include <string>

// This class is used to represent an exception
class OrderException : public std::exception {
public:
  OrderException(const std::string &order_id, const int reason);

  OrderException(const DistributedStockExchange_NewOrderSingle::NewOrderSingle
                     &new_order_single,
                 const int reason);

  const char *what() const noexcept override;

  void populate_execution_report(
      DistributedStockExchange_ExecutionReport::ExecutionReport
          &execution_report);

  void populate_order_cancel_reject(
      DistributedStockExchange_OrderCancelReject::OrderCancelReject
          &order_cancel_reject);

private:
  std::string m_order_id;
  int m_reason;
  std::string m_error_message;
};