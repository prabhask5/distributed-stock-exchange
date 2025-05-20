#include "OrderMassStatusService.hpp"
#include "DataServiceDataWriterContainer.hpp"
#include "SQLiteConnection.hpp"
#include "SQLiteQuery.hpp"
#include <DefaultDomainParticipantConstants.hpp>
#include <ExecutionReport.hpp>
#include <Header.hpp>
#include <OrderMassStatusRequest.hpp>

OrderMassStatusService::OrderMassStatusService(
    const FIX::DatabaseConnectionID &database_connection_id,
    const DataWriterContainerPtr &data_writer_container_ptr,
    OrderMassStatusRequestQueuePtr order_mass_status_request_queue_ptr,
    UserToOrderExecutionReportsMapPtr user_to_order_execution_reports_map_ptr)
    : m_data_writer_container_ptr(data_writer_container_ptr),
      m_order_mass_status_request_queue_ptr(
          order_mass_status_request_queue_ptr),
      m_user_to_order_execution_reports_map_ptr(
          user_to_order_execution_reports_map_ptr) {
  m_sqlite_connection_ptr =
      std::make_unique<SQLiteConnection>(database_connection_id);

  std::atomic_init(&m_is_running, true);
  m_service_thread = std::thread(&OrderMassStatusService::service, this);

  initialize();
}

OrderMassStatusService::~OrderMassStatusService() {
  // Persist all the execution report information in the database before
  // shutdown.

  // For optimization, begin transaction before many inserts.
  SQLiteQuery begin_tx("BEGIN TRANSACTION;", false, {});
  m_sqlite_connection_ptr->execute(begin_tx);

  std::string insert_query_str = "INSERT INTO execution_reports VALUES (?, ?, "
                                 "?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, "
                                 "?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
  SQLiteQuery insert_query(insert_query_str, false, {});
  for (const auto &user_pair : *m_user_to_order_execution_reports_map_ptr) {
    const std::string &user = user_pair.first;
    const auto &order_map = user_pair.second;

    for (const auto &order_pair : *order_map) {
      const std::string &order_id = order_pair.first;
      const auto &report_list = order_pair.second;

      for (const auto &report : *report_list) {
        std::vector<std::string> params = {
            user,
            order_id,
            report->fix_header().BeginString(),
            std::to_string(report->fix_header().BodyLength()),
            report->fix_header().MsgType(),
            report->fix_header().SenderCompID(),
            report->fix_header().TargetCompID(),
            std::to_string(report->fix_header().MsgSeqNum()),
            report->fix_header().SenderSubID(),
            report->fix_header().TargetSubID(),
            std::to_string(report->fix_header().SendingTime()),
            report->OrigClOrdID(),
            report->ExecID(),
            std::string(1, report->ExecType()),
            std::string(1, report->OrdStatus()),
            std::to_string(report->OrdRejReason()),
            report->Symbol(),
            report->SecurityExchange(),
            std::string(1, report->Side()),
            std::to_string(report->OrderQty()),
            std::string(1, report->OrdType()),
            std::to_string(report->Price()),
            std::to_string(report->StopPx()),
            std::string(1, report->TimeInForce()),
            report->ExecInst(),
            std::to_string(report->LastQty()),
            std::to_string(report->LastPx()),
            std::to_string(report->LeavesQty()),
            std::to_string(report->CumQty()),
            std::to_string(report->AvgPx()),
            std::to_string(report->TransactTime()),
            report->Text()};

        insert_query.set_parameters(std::move(params));
        m_sqlite_connection_ptr->execute(insert_query);
      }
    }
  }

  // Commit transaction after inserts.
  SQLiteQuery end_tx("COMMIT;", false, {});
  m_sqlite_connection_ptr->execute(end_tx);

  m_is_running.store(false);
  m_service_thread.join();
}

void OrderMassStatusService::initialize() {
  // This query extracts all the information for order execution reports in the
  // database, and uses it as intialization data for the execution reports map
  // that provides the information for the order mass status service to respond
  // to order mass status data requests from the FIX gateway.
  std::string execution_reports_query_str = "SELECT * FROM execution_reports";
  SQLiteQuery execution_reports_query(execution_reports_query_str, true, {});
  m_sqlite_connection_ptr->execute(execution_reports_query);

  for (int i = 0; i < execution_reports_query.get_num_rows(); ++i) {
    std::string user = execution_reports_query.get_value(i, 0);
    std::string order_id = execution_reports_query.get_value(i, 1);

    auto &order_map = (*m_user_to_order_execution_reports_map_ptr)[user];
    if (!order_map)
      order_map = std::make_shared<OrderToExecutionReportsMap>();

    auto &report_list = (*order_map)[order_id];
    if (!report_list)
      report_list = std::make_shared<ExecutionReportList>();

    ExecutionReportPtr report = std::make_shared<
        DistributedStockExchange_ExecutionReport::ExecutionReport>();

    DistributedStockExchange::Header fix_header;
    fix_header.BeginString(execution_reports_query.get_value(i, 2));
    fix_header.BodyLength(std::stol(execution_reports_query.get_value(i, 3)));
    fix_header.MsgType(execution_reports_query.get_value(i, 4));
    fix_header.SenderCompID(execution_reports_query.get_value(i, 5));
    fix_header.TargetCompID(execution_reports_query.get_value(i, 6));
    fix_header.MsgSeqNum(std::stol(execution_reports_query.get_value(i, 7)));
    fix_header.SenderSubID(execution_reports_query.get_value(i, 8));
    fix_header.TargetSubID(execution_reports_query.get_value(i, 9));
    fix_header.SendingTime(
        std::stoull(execution_reports_query.get_value(i, 10)));
    report->fix_header(std::move(fix_header));

    report->OrigClOrdID(execution_reports_query.get_value(i, 11));
    report->ExecID(execution_reports_query.get_value(i, 12));
    report->ExecType(execution_reports_query.get_value(i, 13)[0]);
    report->OrdStatus(execution_reports_query.get_value(i, 14)[0]);
    report->OrdRejReason(std::stol(execution_reports_query.get_value(i, 15)));
    report->Symbol(execution_reports_query.get_value(i, 16));
    report->SecurityExchange(execution_reports_query.get_value(i, 17));
    report->Side(execution_reports_query.get_value(i, 18)[0]);
    report->OrderQty(std::stol(execution_reports_query.get_value(i, 19)));
    report->OrdType(execution_reports_query.get_value(i, 20)[0]);
    report->Price(std::stof(execution_reports_query.get_value(i, 21)));
    report->StopPx(std::stof(execution_reports_query.get_value(i, 22)));
    report->TimeInForce(execution_reports_query.get_value(i, 23)[0]);
    report->ExecInst(execution_reports_query.get_value(i, 24));
    report->LastQty(std::stol(execution_reports_query.get_value(i, 25)));
    report->LastPx(std::stof(execution_reports_query.get_value(i, 26)));
    report->LeavesQty(std::stol(execution_reports_query.get_value(i, 27)));
    report->CumQty(std::stol(execution_reports_query.get_value(i, 28)));
    report->AvgPx(std::stof(execution_reports_query.get_value(i, 29)));
    report->TransactTime(std::stoull(execution_reports_query.get_value(i, 30)));
    report->Text(execution_reports_query.get_value(i, 31));

    report_list->push_back(report);
  }
}

void OrderMassStatusService::service() {
  while (m_is_running.load()) {
    while (!m_order_mass_status_request_queue_ptr->empty()) {
      OrderMassStatusRequestPtr order_mass_status_request_ptr;
      m_order_mass_status_request_queue_ptr->pop(order_mass_status_request_ptr);
      process_order_mass_status_service_request(
          std::move(order_mass_status_request_ptr));
    }

    std::this_thread::sleep_for(
        std::chrono::duration<long double, std::milli>(1000));
  }
}

void OrderMassStatusService::process_order_mass_status_service_request(
    OrderMassStatusRequestPtr order_mass_status_request_ptr) {
  std::string username = order_mass_status_request_ptr->SourceUser();
  auto oer_map_itr = m_user_to_order_execution_reports_map_ptr->find(username);
  if (oer_map_itr == m_user_to_order_execution_reports_map_ptr->end())
    return;

  OrderToExecutionReportsMapPtr order_to_execution_reports_map_ptr =
      oer_map_itr->second;

  for (auto &exec_list_iter : *order_to_execution_reports_map_ptr) {
    for (auto &exec_report_iter : *(exec_list_iter.second)) {
      exec_report_iter->Source(order_mass_status_request_ptr->Destination());
      exec_report_iter->Destination(order_mass_status_request_ptr->Source());
      exec_report_iter->DestinationUser(username);

      eprosima::fastdds::dds::ReturnCode_t code =
          m_data_writer_container_ptr->executionReportDW->write(
              exec_report_iter.get());

      if (code != eprosima::fastdds::dds::RETCODE_OK)
        LOG4CXX_ERROR(
            logger,
            "MassOrderStatusRequestDataReader/Execution Report write returned :"
                << code);
    }
  }
}