#include "DepthEventHandler.hpp"
#include "DepthEventHandlerTypes.hpp"
#include "ExecutionReportPublisher.hpp"
#include "ExecutionReportPublisherTypes.hpp"
#include "Market.hpp"
#include "MarketDataPublisherService.hpp"
#include "MarketDataPublisherServiceTypes.hpp"
#include "MarketDataSnapshotFullRefreshDataReaderListener.hpp"
#include "MarketDataUpdate.hpp"
#include "MarketDataUpdateTypes.hpp"
#include "MarketTypes.hpp"
#include "MatchingEngineDataWriterContainer.hpp"
#include "MatchingEngineDataWriterContainerTypes.hpp"
#include "NewOrderSingleDataReaderListener.hpp"
#include "OrderBookStockStatistics.hpp"
#include "OrderBookStockStatisticsTypes.hpp"
#include "OrderCancelRequestDataReaderListener.hpp"
#include "OrderEventHandler.hpp"
#include "OrderEventHandlerTypes.hpp"
#include "OrderMassCancelRequestDataReaderListener.hpp"
#include "SecurityListDataReaderListener.hpp"
#include "SecurityListRequestDataWriterListener.hpp"
#include "SecurityListRequestDataWriterListenerTypes.hpp"
#include "TradeEventHandler.hpp"
#include "TradeEventHandlerTypes.hpp"
#include <Constants.hpp>
#include <DefaultDomainParticipant.hpp>
#include <DefaultDomainParticipantConstants.hpp>
#include <ExecutionReportPubSubTypes.hpp>
#include <MarketDataIncrementalRefreshPubSubTypes.hpp>
#include <MarketDataRequestPubSubTypes.hpp>
#include <MarketDataSnapshotFullRefreshPubSubTypes.hpp>
#include <NewOrderSinglePubSubTypes.hpp>
#include <OrderCancelRejectPubSubTypes.hpp>
#include <OrderCancelRequestPubSubTypes.hpp>
#include <OrderMassCancelReportPubSubTypes.hpp>
#include <OrderMassCancelRequestPubSubTypes.hpp>
#include <SecurityListPubSubTypes.hpp>
#include <SecurityListRequestPubSubTypes.hpp>
#include <boost/asio.hpp>
#include <boost/program_options.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>

std::atomic<bool> is_running;

int main(int argc, char *argv[]) {
  LOG4CXX_INFO(logger, "Matching engine starting up");

  try {
    // Set up program options to select config file to use for matching engine
    // preferences.
    std::string config_file = "";
    boost::program_options::options_description options{"Options"};

    options.add_options()("help,h", "Help screen")(
        "config,c",
        boost::program_options::value<std::string>()->default_value(""),
        "Matching Engine Config File");

    boost::program_options::variables_map vm;
    boost::program_options::store(parse_command_line(argc, argv, options), vm);
    boost::program_options::notify(vm);

    if (vm.count("help"))
      std::cout << options << '\n';
    else if (vm.count("config"))
      config_file = vm["config"].as<std::string>();

    if (config_file.empty())
      std::runtime_error("Error: Config file name is not specified.");

    boost::property_tree::ptree pt;
    boost::property_tree::ini_parser::read_ini(config_file, pt);

    // Parse preferences from config file.
    std::string data_service_name =
        pt.get<std::string>("matchingengine.dataservice");
    std::string market_name = pt.get<std::string>("matchingengine.market");

    unsigned int data_pub_interval =
        pt.get<int>("marketdata.data_pub_interval");

    LOG4CXX_INFO(logger, "Market Name| "
                             << market_name << " |Data Service Name| "
                             << data_service_name
                             << " |Market Data Publication Interval| "
                             << data_pub_interval);

    // Set up market data publisher queue, data writer container, execution
    // report publisher, and order book stock statistics dependency pointers.
    MarketDataPublisherQueuePtr market_data_publisher_queue_ptr =
        std::make_shared<MarketDataPublisherQueue>();
    DataWriterContainerPtr data_writer_container_ptr =
        std::make_shared<DataWriterContainer>();
    ExecutionReportPublisherPtr execution_report_publisher_ptr =
        std::make_shared<ExecutionReportPublisher>(data_writer_container_ptr);
    OrderBookStockStatsMapPtr order_book_stats_map_ptr =
        std::make_shared<OrderBookStockStatsMap>();

    // Set up event handler dependency pointers.
    OrderEventHandlerPtr order_event_handler_ptr =
        std::make_shared<OrderEventHandler>(execution_report_publisher_ptr);
    TradeEventHandlerPtr trade_event_handler_ptr =
        std::make_shared<TradeEventHandler>(order_book_stats_map_ptr);
    DepthEventHandlerPtr depth_event_handler_ptr =
        std::make_shared<DepthEventHandler>(order_book_stats_map_ptr,
                                            market_data_publisher_queue_ptr,
                                            market_name);

    // Make top level Market class.
    MarketPtr market_ptr = std::make_shared<Market>(
        market_name, data_service_name, market_data_publisher_queue_ptr,
        std::move(data_writer_container_ptr),
        std::move(order_event_handler_ptr), std::move(trade_event_handler_ptr),
        std::move(depth_event_handler_ptr),
        std::move(execution_report_publisher_ptr),
        std::move(order_book_stats_map_ptr));

    // Filter incoming topic data by only the properties that would match the
    // data needed for each data reader listener implementation.

    // Filter expression for the MARKET_NAME specified in config file.
    // Filter for messages for this market/security exchange.
    std::string destination_market_filter =
        "Destination = %0 and SecurityExchange = %1";

    // Filter for mass cancel - when client disconnects all orders get
    // cancelled.
    std::string matching_engine_filter = "Destination = %0";

    // Market filter: Securities List, Open Prices(Market Data Snap Shot).
    std::string market_filter = "DestinationUser = %0";

    // Make DDS participant representing matching engine, and set up
    // publisher/subscriber.
    auto participant_ptr =
        std::make_shared<DefaultDomainParticipant>(0, "MatchingEngine");

    participant_ptr->create_publisher();
    participant_ptr->create_subscriber();

    // Set up FastDDS topics and data readers for incoming requests.

    // New Order Single Request.
    auto new_order_single_topic_tuple = participant_ptr->make_topic<
        DistributedStockExchange_NewOrderSingle::NewOrderSinglePubSubType,
        DistributedStockExchange_NewOrderSingle::NewOrderSingle>(
        NEW_ORDER_SINGLE_TOPIC_NAME);
    auto new_order_single_data_reader_tuple =
        participant_ptr->make_data_reader_tuple(
            new_order_single_topic_tuple,
            new NewOrderSingleDataReaderListener(market_ptr),
            "FILTER_MATCHING_ENGINE_NEW_ORDER_SINGLE",
            destination_market_filter, {"MATCHING_ENGINE", market_name});

    // Order Cancel Request.
    auto order_cancel_request_topic_tuple = participant_ptr->make_topic<
        DistributedStockExchange_OrderCancelRequest::
            OrderCancelRequestPubSubType,
        DistributedStockExchange_OrderCancelRequest::OrderCancelRequest>(
        ORDER_CANCEL_REQUEST_TOPIC_NAME);
    auto order_cancel_request_data_reader_tuple =
        participant_ptr->make_data_reader_tuple(
            order_cancel_request_topic_tuple,
            new OrderCancelRequestDataReaderListener(market_ptr),
            "FILTER_MATCHING_ENGINE_ORDER_CANCEL_REQUEST",
            destination_market_filter, {"MATCHING_ENGINE", market_name});

    // Order Mass Cancel Request.
    auto order_mass_cancel_request_topic_tuple =
        participant_ptr
            ->make_topic<DistributedStockExchange_OrderMassCancelRequest::
                             OrderMassCancelRequestPubSubType,
                         DistributedStockExchange_OrderMassCancelRequest::
                             OrderMassCancelRequest>(
                ORDER_MASS_CANCEL_REQUEST_TOPIC_NAME);
    auto order_mass_cancel_request_topic_data_reader_tuple =
        participant_ptr->make_data_reader_tuple(
            order_mass_cancel_request_topic_tuple,
            new OrderMassCancelRequestDataReaderListener(market_ptr),
            "FILTER_MATCHING_ENGINE_ORDER_MASS_CANCEL_REQUEST",
            matching_engine_filter, {"MATCHING_ENGINE"});

    // Security List.
    auto security_list_topic_tuple = participant_ptr->make_topic<
        DistributedStockExchange_SecurityList::SecurityListPubSubType,
        DistributedStockExchange_SecurityList::SecurityList>(
        SECURITY_LIST_TOPIC_NAME);
    auto security_list_topic_request_data_reader_tuple =
        participant_ptr->make_data_reader_tuple(
            security_list_topic_tuple,
            new SecurityListDataReaderListener(market_ptr),
            "FILTER_MATCHING_ENGINE_SECURITY_LIST", market_filter,
            {market_ptr->get_market_name()});

    // Market Data Snapshot Full Refresh.
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
            new MarketDataSnapshotFullRefreshDataReaderListener(market_ptr),
            "FILTER_MATCHING_ENGINE_FULL_SNAPSHOT_REQUEST", market_filter,
            {market_ptr->get_market_name()});

    // Set up FastDDS topics and data writers for outgoing reports/requests.

    // Execution Report.
    auto execution_report_topic_tuple = participant_ptr->make_topic<
        DistributedStockExchange_ExecutionReport::ExecutionReportPubSubType,
        DistributedStockExchange_ExecutionReport::ExecutionReport>(
        EXECUTION_REPORT_TOPIC_NAME);
    data_writer_container_ptr->execReportDW =
        participant_ptr->make_data_writer(execution_report_topic_tuple);

    // Mass Cancel Report.
    auto order_mass_cancel_report_topic_tuple = participant_ptr->make_topic<
        DistributedStockExchange_OrderMassCancelReport::
            OrderMassCancelReportPubSubType,
        DistributedStockExchange_OrderMassCancelReport::OrderMassCancelReport>(
        ORDER_MASS_CANCEL_REPORT_TOPIC_NAME);
    data_writer_container_ptr->orderMassCancelReportDW =
        participant_ptr->make_data_writer(order_mass_cancel_report_topic_tuple);

    // Security List Request.
    auto security_list_request_topic_tuple = participant_ptr->make_topic<
        DistributedStockExchange_SecurityListRequest::
            SecurityListRequestPubSubType,
        DistributedStockExchange_SecurityListRequest::SecurityListRequest>(
        SECURITY_LIST_REQUEST_TOPIC_NAME);
    SecurityListRequestDataWriterListenerPtr
        security_list_request_data_writer_listener_ptr =
            std::make_unique<SecurityListRequestDataWriterListener>(market_ptr);
    data_writer_container_ptr->securityListRequestDW =
        participant_ptr->make_data_writer(
            security_list_request_topic_tuple,
            security_list_request_data_writer_listener_ptr.get());

    // Order Cancel Reject Report.
    auto order_cancel_reject_topic_tuple = participant_ptr->make_topic<
        DistributedStockExchange_OrderCancelReject::OrderCancelRejectPubSubType,
        DistributedStockExchange_OrderCancelReject::OrderCancelReject>(
        ORDER_CANCEL_REJECT_TOPIC_NAME);
    data_writer_container_ptr->orderCancelRejectDW =
        participant_ptr->make_data_writer(order_cancel_reject_topic_tuple);

    // Market Data Incremental Refresh Report.
    auto market_data_incremental_refresh_topic_tuple =
        participant_ptr
            ->make_topic<DistributedStockExchange_MarketDataIncrementalRefresh::
                             MarketDataIncrementalRefreshPubSubType,
                         DistributedStockExchange_MarketDataIncrementalRefresh::
                             MarketDataIncrementalRefresh>(
                MARKET_DATA_INCREMENTAL_REFRESH_TOPIC_NAME);
    data_writer_container_ptr->marketDataIncrementalRefreshDW =
        participant_ptr->make_data_writer(
            market_data_incremental_refresh_topic_tuple);

    // Market Data Request.
    auto market_data_request_topic_tuple = participant_ptr->make_topic<
        DistributedStockExchange_MarketDataRequest::MarketDataRequestPubSubType,
        DistributedStockExchange_MarketDataRequest::MarketDataRequest>(
        MARKET_DATA_REQUEST_TOPIC_NAME);
    data_writer_container_ptr->marketDataRequestDW =
        participant_ptr->make_data_writer(market_data_request_topic_tuple);

    // Create and start the market data publisher service thread.
    MarketDataPublisherServicePtr market_data_publisher_service_ptr =
        std::make_shared<MarketDataPublisherService>(
            data_writer_container_ptr->marketDataIncrementalRefreshDW.get(),
            std::move(market_data_publisher_queue_ptr), data_pub_interval);

    // Officially start the matching engine by flipping the flag on.
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
    LOG4CXX_ERROR(
        logger,
        "Exception during the initialization of Matching Engine: " << e.what());

    std::cout << e.what() << std::endl;
    return 1;
  }
  return 0;
}
