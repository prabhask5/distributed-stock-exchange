#include "NewOrderSingleDataReaderListener.hpp"
#include "ExecutionReportPublisher.hpp"
#include "Market.hpp"
#include "Order.hpp"
#include "OrderException.hpp"
#include "OrderTypes.hpp"
#include <DefaultDomainParticipantConstants.hpp>
#include <ExecutionReport.hpp>
#include <LoggerHelper.hpp>
#include <NewOrderSingle.hpp>
#include <NewOrderSingleLogger.hpp>
#include <quickfix/FixValues.h>

NewOrderSingleDataReaderListener::NewOrderSingleDataReaderListener(
    const MarketPtr &market_ptr)
    : m_market_ptr(market_ptr) {}

void NewOrderSingleDataReaderListener::on_data_available(DataReader *reader) {
  DistributedStockExchange_NewOrderSingle::NewOrderSingle new_order_single;
  eprosima::fastdds::dds::SampleInfo info;

  if (reader->take_next_sample(&new_order_single, &info) ==
      eprosima::fastdds::dds::RETCODE_OK) {
    if (info.valid_data) {
      log_debug<std::stringstream, NewOrderSingleLogger,
                DistributedStockExchange_NewOrderSingle::NewOrderSingle>(
          logger, new_order_single, "NewOrderSingle");

      std::string symbol = new_order_single.Symbol();
      std::string security_exchange = new_order_single.SecurityExchange();

      try {
        OrderBookPtr order_book = m_market_ptr->find_book(symbol);
        if (!order_book)
          throw OrderException(new_order_single,
                               FIX::OrdRejReason_UNKNOWN_SYMBOL);

        std::string order_id = new_order_single.ClOrdID();

        bool is_buy = false;
        if (new_order_single.Side() == '1')
          is_buy = true;

        std::string gateway = new_order_single.Source();
        std::string data_service = new_order_single.DestinationUser();
        std::string sender_id = new_order_single.SourceUser();

        auto quantity = new_order_single.OrderQty();
        auto price = new_order_single.Price();
        auto stop_price = new_order_single.StopPx();

        OrderConditions conditions = OrderCondition::NONE;

        // ExecInst is string in the data dictionary but char in quickfix/c++.
        // <field number='18' name='ExecInst' type='MULTIPLEVALUESTRING'>
        if (new_order_single.ExecInst().length() > 0 &&
            new_order_single.ExecInst()[0] == FIX::ExecInst_ALL_OR_NONE)
          conditions = OrderCondition::ALL_OR_NONE;

        if (new_order_single.TimeInForce() ==
            FIX::TimeInForce_IMMEDIATE_OR_CANCEL)
          conditions = OrderCondition::IMM_OR_CANCEL;
        else if (new_order_single.TimeInForce() ==
                 FIX::TimeInForce_FILL_OR_KILL)
          conditions = OrderCondition::FILL_OR_KILL;

        OrderPtr order = std::make_shared<Order>(
            order_id, sender_id, is_buy, quantity, price, stop_price, symbol,
            gateway, data_service,
            m_market_ptr->get_data_writer_container_ptr(), security_exchange,
            conditions);
        m_market_ptr->submit_order(order_book, order);
      } catch (OrderException &orderException) {
        DistributedStockExchange_ExecutionReport::ExecutionReport
            execution_report;

        execution_report.Source("MATCHING_ENGINE");
        execution_report.SourceUser(new_order_single.DestinationUser());
        execution_report.Destination(new_order_single.Source());
        execution_report.DestinationUser(new_order_single.SourceUser());
        execution_report.fix_header().MsgType("8");
        execution_report.Symbol(symbol);
        execution_report.SecurityExchange(security_exchange);
        execution_report.Side(new_order_single.Side());
        execution_report.Text("Order Rejected");
        execution_report.CumQty(0);
        execution_report.LeavesQty(0);
        execution_report.Price(0);
        execution_report.TimeInForce(new_order_single.TimeInForce());
        execution_report.OrdType(new_order_single.OrdType());
        execution_report.ExecInst(new_order_single.ExecInst());
        execution_report.TransactTime(
            std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::system_clock::now().time_since_epoch())
                .count());

        orderException.populate_execution_report(execution_report);
        m_market_ptr->get_execution_report_publisher_ptr()->publish_report(
            execution_report);
      }
    }
  }
}