#include "AuthService.hpp"
#include "AuthServiceTypes.hpp"
#include "ExecutionReportDataReaderListener.hpp"
#include "FixApplication.hpp"
#include "FixGatewayDataWriterContainer.hpp"
#include "FixGatewayDataWriterContainerTypes.hpp"
#include "FixLogFileFactory.hpp"
#include "FixSocketAcceptor.hpp"
#include "FixSocketAcceptorTypes.hpp"
#include "LogonDataReaderListener.hpp"
#include "LogoutDataReaderListener.hpp"
#include "MarketDataIncrementalRefreshDataReaderListener.hpp"
#include "MarketDataSnapshotFullRefreshDataReaderListener.hpp"
#include "OrderCancelRejectDataReaderListener.hpp"
#include "OrderMassCancelReportDataReaderListener.hpp"
#include "SecurityListDataReaderListener.hpp"
#include <Constants.hpp>
#include <DefaultDomainParticipant.hpp>
#include <DefaultDomainParticipantConstants.hpp>
#include <ExecutionReportPubSubTypes.hpp>
#include <LogonPubSubTypes.hpp>
#include <LogoutPubSubTypes.hpp>
#include <MarketDataIncrementalRefreshPubSubTypes.hpp>
#include <MarketDataRequestPubSubTypes.hpp>
#include <MarketDataSnapshotFullRefreshPubSubTypes.hpp>
#include <NewOrderSinglePubSubTypes.hpp>
#include <OrderCancelRejectPubSubTypes.hpp>
#include <OrderCancelRequestPubSubTypes.hpp>
#include <OrderMassCancelReportPubSubTypes.hpp>
#include <OrderMassCancelRequestPubSubTypes.hpp>
#include <OrderMassStatusRequestPubSubTypes.hpp>
#include <SecurityListPubSubTypes.hpp>
#include <SecurityListRequestPubSubTypes.hpp>
#include <boost/asio.hpp>
#include <boost/program_options.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <iostream>
#include <quickfix/FileStore.h>
#include <quickfix/SessionFactory.h>
#include <quickfix/SessionSettings.h>

std::atomic<bool> is_running;

int main(int argc, char *argv[]) {
  LOG4CXX_INFO(logger, "FIX gateway starting up");

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
    FixSettingsPtr settings_ptr =
        std::make_unique<FIX::SessionSettings>(config_file);
    std::string fix_gateway_name =
        settings_ptr->get().getString("SenderCompID");

    // Initialize this machines default session from config file.
    FIX::SessionID default_session_id("FIX.4.4", "DEFAULT", "DEFAULT");
    const FIX::Dictionary &session_settings =
        settings_ptr->get(default_session_id);

    // Parse env variables.
    auto home_env = std::getenv("DSE_HOME");
    auto log_env = std::getenv("EXCHANGE_LOGDIR");
    if (home_env == NULL || log_env == NULL)
      throw std::runtime_error("DSE_HOME or/and EXCHANGE_LOGDIR is not set");

    // Create quickfix data dictionary.
    FixDictionaryPtr default_dictionary_ptr = std::make_unique<FIX::Dictionary>(
        settings_ptr->get(default_session_id));
    default_dictionary_ptr->setString(
        "DATADICTIONARY",
        std::string(home_env) + "/spec/" +
            default_dictionary_ptr->getString("DATADICTIONARY"));

    // Parse data service name from data dictionary.
    std::string data_service_name =
        default_dictionary_ptr->getString("DataService");

    LOG4CXX_INFO(logger, "SenderCompID| " << fix_gateway_name
                                          << " |Data Service Name| "
                                          << data_service_name);

    // Make DDS participant representing matching engine, and set up
    // publisher/subscriber.
    DefaultDomainParticipantPtr participant_ptr =
        std::make_unique<DefaultDomainParticipant>(0, fix_gateway_name);

    participant_ptr->create_publisher();
    participant_ptr->create_subscriber();

    // Make the FIX gateway data writer container dependency.
    DataWriterContainerPtr data_writer_container_ptr =
        std::make_unique<DataWriterContainer>();

    // Set up FastDDS topics and data writers for outgoing reports/requests.

    // Logon request.
    auto logon_topic_tuple =
        participant_ptr
            ->make_topic<DistributedStockExchange_Logon::LogonPubSubType,
                         DistributedStockExchange_Logon::Logon>(
                LOGON_TOPIC_NAME);
    data_writer_container_ptr->logOnDW =
        participant_ptr->make_data_writer(logon_topic_tuple);

    // New Order Single request.
    auto new_order_single_topic_tuple = participant_ptr->make_topic<
        DistributedStockExchange_NewOrderSingle::NewOrderSinglePubSubType,
        DistributedStockExchange_NewOrderSingle::NewOrderSingle>(
        NEW_ORDER_SINGLE_TOPIC_NAME);
    data_writer_container_ptr->newOrderSingleDW =
        participant_ptr->make_data_writer(new_order_single_topic_tuple);

    // Order Cancel request.
    auto order_cancel_request_topic_tuple = participant_ptr->make_topic<
        DistributedStockExchange_OrderCancelRequest::
            OrderCancelRequestPubSubType,
        DistributedStockExchange_OrderCancelRequest::OrderCancelRequest>(
        ORDER_CANCEL_REQUEST_TOPIC_NAME);
    data_writer_container_ptr->orderCancelRequestDW =
        participant_ptr->make_data_writer(order_cancel_request_topic_tuple);

    // Order Mass Cancel request.
    auto order_mass_cancel_request_topic_tuple =
        participant_ptr
            ->make_topic<DistributedStockExchange_OrderMassCancelRequest::
                             OrderMassCancelRequestPubSubType,
                         DistributedStockExchange_OrderMassCancelRequest::
                             OrderMassCancelRequest>(
                ORDER_MASS_CANCEL_REQUEST_TOPIC_NAME);
    data_writer_container_ptr->orderMassCancelRequestDW =
        participant_ptr->make_data_writer(
            order_mass_cancel_request_topic_tuple);

    // Order Mass Status request.
    auto order_mass_status_request_topic_tuple =
        participant_ptr
            ->make_topic<DistributedStockExchange_OrderMassStatusRequest::
                             OrderMassStatusRequestPubSubType,
                         DistributedStockExchange_OrderMassStatusRequest::
                             OrderMassStatusRequest>(
                ORDER_MASS_STATUS_REQUEST_TOPIC_NAME);
    data_writer_container_ptr->orderMassCancelRequestDW =
        participant_ptr->make_data_writer(
            order_mass_status_request_topic_tuple);

    // Security List request.
    auto security_list_request_topic_tuple = participant_ptr->make_topic<
        DistributedStockExchange_SecurityListRequest::
            SecurityListRequestPubSubType,
        DistributedStockExchange_SecurityListRequest::SecurityListRequest>(
        SECURITY_LIST_REQUEST_TOPIC_NAME);
    data_writer_container_ptr->securityListRequestDW =
        participant_ptr->make_data_writer(security_list_request_topic_tuple);

    // Market Data request.
    auto market_data_request_topic_tuple = participant_ptr->make_topic<
        DistributedStockExchange_MarketDataRequest::MarketDataRequestPubSubType,
        DistributedStockExchange_MarketDataRequest::MarketDataRequest>(
        MARKET_DATA_REQUEST_TOPIC_NAME);
    data_writer_container_ptr->marketDataRequestDW =
        participant_ptr->make_data_writer(market_data_request_topic_tuple);

    // Make the FixLogFileFactory, FileStoreFactory dependencies.
    std::string fix_prefix =
        "FIXGateway-" + fix_gateway_name + "." + data_service_name;
    FixLogFileFactory log_factory(*settings_ptr, fix_prefix);
    FIX::FileStoreFactory store_factory(log_env);

    // Make the top level FIX app.
    FixApplication app(data_service_name, fix_gateway_name,
                       std::move(data_writer_container_ptr));

    // Set up FastDDS topics and data readers for incoming requests.

    // Filter for Logon/Logout.
    std::string auth_comp_id_filter = "SourceUser=%0";

    // Logon response.
    auto logon_data_reader_tuple = participant_ptr->make_data_reader_tuple(
        logon_topic_tuple, new LogonDataReaderListener(app), "FILTERED_LOGON",
        auth_comp_id_filter, {"AUTH"});

    // Logout request.
    auto logout_topic_tuple =
        participant_ptr
            ->make_topic<DistributedStockExchange_Logout::LogoutPubSubType,
                         DistributedStockExchange_Logout::Logout>(
                LOGOUT_TOPIC_NAME);
    auto logout_data_reader_tuple = participant_ptr->make_data_reader_tuple(
        logout_topic_tuple, new LogoutDataReaderListener(app),
        "FILTERED_LOGOUT", auth_comp_id_filter, {"AUTH"});

    // Filter for Security List.
    std::string target_comp_id_filter = "Destination=%0";

    // Security List response.
    auto security_list_topic_tuple = participant_ptr->make_topic<
        DistributedStockExchange_SecurityList::SecurityListPubSubType,
        DistributedStockExchange_SecurityList::SecurityList>(
        SECURITY_LIST_TOPIC_NAME);
    auto security_list_data_reader_tuple =
        participant_ptr->make_data_reader_tuple(
            security_list_topic_tuple, new SecurityListDataReaderListener(app),
            "FILTERED_REF_DATA", target_comp_id_filter, {fix_gateway_name});

    // Market Data Incremental Refresh response.
    auto market_data_incremental_refresh_topic_tuple =
        participant_ptr
            ->make_topic<DistributedStockExchange_MarketDataIncrementalRefresh::
                             MarketDataIncrementalRefreshPubSubType,
                         DistributedStockExchange_MarketDataIncrementalRefresh::
                             MarketDataIncrementalRefresh>(
                MARKET_DATA_INCREMENTAL_REFRESH_TOPIC_NAME);
    auto market_data_incremental_refresh_data_reader_tuple =
        participant_ptr->make_data_reader_tuple(
            market_data_incremental_refresh_topic_tuple,
            new MarketDataIncrementalRefreshDataReaderListener(app));

    // Market Data Snapshot Full Refresh response.
    auto market_data_snapshot_full_refresh_topic_tuple =
        participant_ptr->make_topic<
            DistributedStockExchange_MarketDataSnapshotFullRefresh::
                MarketDataSnapshotFullRefreshPubSubType,
            DistributedStockExchange_MarketDataSnapshotFullRefresh::
                MarketDataSnapshotFullRefresh>(
            MARKET_DATA_SNAPSHOT_FULL_REFRESH_TOPIC_NAME);
    auto market_data_snapshot_full_refresh_data_reader_tuple =
        participant_ptr->make_data_reader_tuple(
            market_data_snapshot_full_refresh_topic_tuple,
            new MarketDataSnapshotFullRefreshDataReaderListener(app));

    // Execution Report response.
    auto execution_report_topic_tuple = participant_ptr->make_topic<
        DistributedStockExchange_ExecutionReport::ExecutionReportPubSubType,
        DistributedStockExchange_ExecutionReport::ExecutionReport>(
        EXECUTION_REPORT_TOPIC_NAME);
    auto execution_report_data_reader_tuple =
        participant_ptr->make_data_reader_tuple(
            execution_report_topic_tuple,
            new ExecutionReportDataReaderListener(app), "FILTERED_EXEC_REPORT",
            target_comp_id_filter, {fix_gateway_name});

    // Order Cancel Reject response.
    auto order_cancel_reject_topic_tuple = participant_ptr->make_topic<
        DistributedStockExchange_OrderCancelReject::OrderCancelRejectPubSubType,
        DistributedStockExchange_OrderCancelReject::OrderCancelReject>(
        ORDER_CANCEL_REJECT_TOPIC_NAME);
    auto order_cancel_reject_data_reader_tuple =
        participant_ptr->make_data_reader_tuple(
            order_cancel_reject_topic_tuple,
            new OrderCancelRejectDataReaderListener(app),
            "FILTERED_ORDER_CANCEL_REJECT", target_comp_id_filter,
            {fix_gateway_name});

    // Order Mass Cancel Report reponse.
    auto order_mass_cancel_report_topic_tuple = participant_ptr->make_topic<
        DistributedStockExchange_OrderMassCancelReport::
            OrderMassCancelReportPubSubType,
        DistributedStockExchange_OrderMassCancelReport::OrderMassCancelReport>(
        ORDER_MASS_CANCEL_REPORT_TOPIC_NAME);
    auto order_mass_cancel_report_data_reader_tuple =
        participant_ptr->make_data_reader_tuple(
            order_mass_cancel_report_topic_tuple,
            new OrderMassCancelReportDataReaderListener(app),
            "FILTERED_ORDER_MASS_CANCEL", target_comp_id_filter,
            {fix_gateway_name});

    // Make the socket acceptor and session factory to accept new FIX
    // connections, and manage current FIX sessions.
    FixSocketAcceptorPtr acceptor_ptr = std::make_unique<FixSocketAcceptor>(
        app, store_factory, *settings_ptr, log_factory);
    FixSessionFactoryPtr session_factory_ptr =
        std::make_unique<FIX::SessionFactory>(app, store_factory, &log_factory);

    // Set up the auth service dependency, and add it to app.
    AuthServicePtr auth_service_ptr = std::make_unique<AuthService>(
        std::move(settings_ptr), std::move(session_factory_ptr),
        std::move(default_dictionary_ptr), fix_gateway_name);
    app.set_auth_service_ptr(std::move(auth_service_ptr));

    // Start accepting FIX socket connections.
    acceptor_ptr->start();

    // Officially start the FIX gateway by flipping the flag on.
    std::atomic_init(&is_running, true);

    // Lastly, handle signals to stop the service.

    boost::asio::io_context io_service;
    boost::asio::signal_set signals(io_service, SIGINT, SIGTERM);

    signals.async_wait([&](const boost::system::error_code &ec,
                           int signal_number) {
      if (!ec) {
        LOG4CXX_INFO(logger,
                     "Signal number "
                         << signal_number
                         << "Gracefully stopping the timer and exiting");
        is_running.store(false);
      } else {
        LOG4CXX_ERROR(logger, "Error " << ec.value() << " - " << ec.message()
                                       << " - Signal number - "
                                       << signal_number);
      }
    });

    io_service.run();

    acceptor_ptr->stop();
  } catch (std::exception &e) {
    LOG4CXX_ERROR(logger, "Exception during the initialization of FIX Gateway: "
                              << e.what());

    std::cout << e.what() << std::endl;
    return 1;
  }

  return 0;
}