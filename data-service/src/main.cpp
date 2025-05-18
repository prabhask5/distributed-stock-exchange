#include "AuthService.hpp"
#include "AuthServiceTypes.hpp"
#include "DataServiceDataWriterContainer.hpp"
#include "DataServiceDataWriterContainerTypes.hpp"
#include "InstrumentReferenceDataService.hpp"
#include "InstrumentReferenceDataServiceTypes.hpp"
#include "MarketDataService.hpp"
#include "MarketDataServiceTypes.hpp"
#include "OrderMassStatusService.hpp"
#include "OrderMassStatusServiceTypes.hpp"
#include "SQLiteConnection.hpp"
#include "SQLiteConnectionTypes.hpp"
#include <Constants.hpp>
#include <DefaultDomainParticipant.hpp>
#include <DefaultDomainParticipantConstants.hpp>
#include <DefaultDomainParticipantTypes.hpp>
#include <ExecutionReport.hpp>
#include <ExecutionReportPubSubType.hpp>
#include <Logon.hpp>
#include <LogonPubSubType.hpp>
#include <Logout.hpp>
#include <LogoutPubSubType.hpp>
#include <MarketDataIncrementalRefresh.hpp>
#include <MarketDataIncrementalRefreshPubSubType.hpp>
#include <MarketDataRequest.hpp>
#include <MarketDataRequestPubSubType.hpp>
#include <MarketDataSnapshotFullRefresh.hpp>
#include <MarketDataSnapshotFullRefreshPubSubType.hpp>
#include <OrderMassStatusRequest.hpp>
#include <OrderMassStatusRequestPubSubType.hpp>
#include <SecurityList.hpp>
#include <SecurityListPubSubType.hpp>
#include <SecurityListRequest.hpp>
#include <SecurityListRequestPubSubType.hpp>
#include <atomic>
#include <boost/asio.hpp>
#include <boost/program_options.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <exception>
#include <iostream>
#include <quickfix/DatabaseConnectionID.h>

std::atomic<bool> is_running;

int main(int argc, char *argv[]) {
  LOG4CXX_INFO(logger, "Data service starting up");

  try {
    // Set up program options to select config file to use for matching engine
    // preferences.
    std::string config_file = "";
    boost::program_options::options_description options{"Options"};

    options.add_options()("help,h", "Help screen")(
        "config,c",
        boost::program_options::value<std::string>()->default_value(""),
        "QuickFIX Config file");

    boost::program_options::variables_map vm;
    boost::program_options::store(parse_command_line(argc, argv, options), vm);
    boost::program_options::notify(vm);

    if (vm.count("help"))
      std::cout << options << '\n';
    else if (vm.count("config"))
      config_file = vm["config"].as<std::string>();

    if (config_file.empty())
      throw std::runtime_error("Error: Config file name is not specified.");

    boost::property_tree::ptree pt;
    boost::property_tree::ini_parser::read_ini(config_file, pt);

    // Parse settings/preferences from config file.
    std::string data_service_name = pt.get<std::string>("dataservice.name");
    std::string database_file = pt.get<std::string>("database.database_file");

    // Parse env variables.
    auto home_env = std::getenv("DSE_HOME");
    auto exchange_home_env = std::getenv("EXCHANGE_BASEDIR");
    if (home_env == nullptr || exchange_home_env == nullptr)
      throw std::runtime_error("DSE_HOME and/or EXCHANGE_BASEDIR is not set");

    // Make DDS participant representing data service, and set up
    // publisher/subscriber.
    DefaultDomainParticipant participant(0, data_service_name);

    participant.create_publisher();
    participant.create_subscriber();

    // Make the database connection id from ENV variables to allow services to
    // connect to the database.
    FIX::DatabaseConnectionID database_connection_id(
        std::string(home_env) + std::string(exchange_home_env) + "/data/" +
            database_file,
        "", "", "", 0);

    // Test check if the SQLite connection works.
    SQLiteConnectionPtr sql_connection_ptr =
        std::make_unique<SQLiteConnection>(database_connection_id);
    if (!sql_connection_ptr->is_connected())
      throw std::runtime_error("Data Service is not connected to the database");

    // Make the data service data writer container dependency.
    DataWriterContainerPtr data_writer_container_ptr =
        std::make_unique<DataWriterContainer>();

    // Set up FastDDS topics and data writers for outgoing reports/requests.

    // Logon response.
    auto logon_topic_tuple =
        participant.make_topic<DistributedStockExchange_Logon::LogonPubSubType,
                               DistributedStockExchange_Logon::Logon>(
            LOGON_TOPIC_NAME);
    data_writer_container_ptr->logOnDW =
        participant.make_data_writer<DistributedStockExchange_Logon::Logon>(
            logon_topic_tuple);

    // Logout response.
    auto logout_topic_tuple =
        participant
            .make_topic<DistributedStockExchange_Logout::LogoutPubSubType,
                        DistributedStockExchange_Logout::Logout>(
                LOGOUT_TOPIC_NAME);
    data_writer_container_ptr->logOutDW =
        participant.make_data_writer<DistributedStockExchange_Logout::Logout>(
            logout_topic_tuple);

    // Market Data Snapshot Full Refresh response.
    auto market_data_snapshot_full_refresh_topic_tuple =
        participant
            .make_topic<DistributedStockExchange_MarketDataSnapshotFullRefresh::
                            MarketDataSnapshotFullRefreshPubSubType,
                        DistributedStockExchange_MarketDataSnapshotFullRefresh::
                            MarketDataSnapshotFullRefresh>(
                MARKET_DATA_SNAPSHOT_FULL_REFRESH_TOPIC_NAME);
    data_writer_container_ptr->marketDataSnapshotFullRefreshDW =
        participant.make_data_writer<
            DistributedStockExchange_MarketDataSnapshotFullRefresh::
                MarketDataSnapshotFullRefresh>(
            market_data_snapshot_full_refresh_topic_tuple);

    // Execution Report response.
    auto execution_report_topic_tuple = participant.make_topic<
        DistributedStockExchange_ExecutionReport::ExecutionReportPubSubType,
        DistributedStockExchange_ExecutionReport::ExecutionReport>(
        EXECUTION_REPORT_TOPIC_NAME);
    data_writer_container_ptr->executionReportDW = participant.make_data_writer<
        DistributedStockExchange_ExecutionReport::ExecutionReport>(
        execution_report_topic_tuple);

    // Security List response.
    auto security_list_topic_tuple = participant.make_topic<
        DistributedStockExchange_SecurityList::SecurityListPubSubType,
        DistributedStockExchange_SecurityList::SecurityList>(
        SECURITY_LIST_TOPIC_NAME);
    data_writer_container_ptr->securityListDW = participant.make_data_writer<
        DistributedStockExchange_SecurityList::SecurityList>(
        security_list_topic_tuple);

    // Set up the different data service dependencies.

    // AuthService.
    LogonQueuePtr logon_queue_ptr = std::make_shared<LogonQueue>();

    // InstrumentReferenceDataService.
    SecurityListRequestQueuePtr security_list_request_queue_ptr =
        std::make_shared<SecurityListRequestQueue>();

    // OrderMassStatusService.
    OrderMassStatusRequestQueuePtr order_mass_status_request_queue_ptr =
        std::make_shared<OrderMassStatusRequestQueue>();
    SymbolToOrderExecutionReportsMapPtr
        symbol_to_order_execution_reports_map_ptr =
            std::make_shared<SymbolToOrderExecutionReportsMap>();

    // MarketDataService.
    MarketDataRequestQueuePtr market_data_request_queue_ptr =
        std::make_shared<MarketDataRequestQueue>();
    IncrementalRefreshMapPtr incremental_refresh_map_ptr =
        std::make_shared<IncrementalRefreshMap>();

    // Set up FastDDS topics and data readers for incoming requests.

    // Execution Report data.
    auto execution_report_data_reader_tuple =
        participant.make_data_reader_tuple(
            execution_report_topic_tuple,
            new ExecutionReportDataReaderListener(
                symbol_to_order_execution_reports_map_ptr),
            "EXECUTION_REPORT_FILTER", "Source = %0 and SourceUser = %1",
            {"MATCHING_ENGINE", participant.get_participant_name()});

    // Logon request.
    auto logon_data_reader_tuple = participant.make_data_reader_tuple(
        logon_topic_tuple, new LogonDataReaderListener(logon_request_queue_ptr),
        "FILTERED_LOGON", "DestinationUser = %0",
        {participant.get_participant_name()});

    // Market Data Incremental Refresh data.
    auto market_data_incremental_refresh_topic_tuple =
        participant
            .make_topic<DistributedStockExchange_MarketDataIncrementalRefresh::
                            MarketDataIncrementalRefreshPubSubType,
                        DistributedStockExchange_MarketDataIncrementalRefresh::
                            MarketDataIncrementalRefresh>(
                MARKET_DATA_INCREMENTAL_REFRESH_TOPIC_NAME);
    auto market_data_incremental_refresh_data_reader_tuple =
        participant.make_data_reader_tuple(
            market_data_incremental_refresh_topic_tuple,
            new MarketDataIncrementalRefreshDataReaderListener(
                incremental_refresh_map_ptr));

    // Market Data request.
    auto market_data_request_topic_tuple = participant.make_topic<
        DistributedStockExchange_MarketDataRequest::MarketDataRequestPubSubType,
        DistributedStockExchange_MarketDataRequest::MarketDataRequest>(
        MARKET_DATA_REQUEST_TOPIC_NAME);
    auto market_data_request_data_reader_tuple =
        participant.make_data_reader_tuple(
            market_data_request_topic_tuple,
            new MarketDataRequestDataReaderListener(
                market_data_request_queue_ptr),
            "FILTERED_MARKET_DATA_REQUEST", "DestinationUser = %0",
            {participant.get_participant_name()});

    // Order Mass Status request.
    auto order_mass_status_request_topic_tuple =
        participant.make_topic<DistributedStockExchange_OrderMassStatusRequest::
                                   OrderMassStatusRequestPubSubType,
                               DistributedStockExchange_OrderMassStatusRequest::
                                   OrderMassStatusRequest>(
            ORDER_MASS_STATUS_REQUEST_TOPIC_NAME);
    auto order_mass_status_request_data_reader_tuple =
        participant.make_data_reader_tuple(
            order_mass_status_request_topic_tuple,
            new OrderMassStatusRequestDataReaderListener(
                order_mass_status_request_queue_ptr),
            "MASS_STATUS_REQUEST_FILTER",
            "Destination = %0 and DestinationUser = %1",
            {"DATA_SERVICE", participant.get_participant_name()});

    // Security List request.
    auto security_list_request_topic_tuple = participant.make_topic<
        DistributedStockExchange_SecurityListRequest::
            SecurityListRequestPubSubType,
        DistributedStockExchange_SecurityListRequest::SecurityListRequest>(
        SECURITY_LIST_REQUEST_TOPIC_NAME);
    auto security_list_request_data_reader_tuple =
        participant.make_data_reader_tuple(
            security_list_request_topic_tuple,
            new SecurityListRequestDataReaderListener(
                security_list_request_queue_ptr),
            "SECURITY_LIST_REQUEST_FILTER", "DestinationUser = %0",
            {participant.get_participant_name()});

    // Set up the different data service threads.
    AuthService auth_service(database_connection_id, data_writer_container_ptr,
                             std::move(logon_queue_ptr));
    InstrumentReferenceDataService instrument_reference_data_service(
        database_connection_id, data_writer_container_ptr,
        std::move(security_list_request_queue_ptr));
    MarketDataService market_data_service(
        database_connection_id, data_writer_container_ptr,
        std::move(incremental_refresh_map_ptr),
        std::move(market_data_request_queue_ptr));
    OrderMassStatusService order_mass_status_service(
        data_writer_container_ptr,
        std::move(order_mass_status_request_queue_ptr),
        std::move(symbol_to_order_execution_reports_map_ptr));

    // Officially start the data service by flipping the flag on.
    std::atomic_init(&is_running, true);

    // Lastly, handle signals to stop the service.

    boost::asio::io_context io_service;
    boost::asio::signal_set signals(io_service, SIGINT, SIGTERM);

    signals.async_wait([&](const boost::system::error_code &ec,
                           int signal_number) {
      if (!ec) {
        std::cout << "Signal number " << signal_number << std::endl;
        std::cout << "Gracefully stopping the timer and exiting" << std::endl;
        is_running.store(false);
      } else {
        std::cout << "Error " << ec.value() << " - " << ec.message()
                  << " - Signal number - " << signal_number << std::endl;
      }
    });

    io_service.run();
  } catch (std::exception &e) {
    LOG4CXX_ERROR(logger, "Exception during the initialization of Data Service"
                              << e.what());
    return 1;
  }
}