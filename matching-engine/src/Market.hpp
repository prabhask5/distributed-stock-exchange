#pragma once

#include "DepthEventHandlerTypes.hpp"
#include "ExecutionReportPublisherTypes.hpp"
#include "MarketDataUpdateTypes.hpp"
#include "MarketTypes.hpp"
#include "MatchingEngineDataWriterContainerTypes.hpp"
#include "OrderBookStockStatisticsTypes.hpp"
#include "OrderEventHandlerTypes.hpp"
#include "TradeEventHandlerTypes.hpp"

// This class represents the top level abstraction of the matching engine.
// An instance of the market class will contain one or more order books/depth
// order books, and provides all the functionality to take orders in and match
// them with corresponding orders. The market class also manages stats related
// to the matching engine to be sent to the user via reports or to the data
// service.
//
// NOTE: Since this is the master class, all the smart pointers to dependencies
// should be owned by this class, and main.cpp where the dependencies are
// created should NOT be an owner (pass by value and use std::move).
//
// NOTE: This does not include MarketDataPublisherQueuePtr since that is owned
// by the MarketDataPublisherService class.
class Market {

public:
  Market(const std::string &market_name, const std::string &data_service_name,
         const MarketDataPublisherQueuePtr &market_data_publisher_queue_ptr,
         DataWriterContainerPtr data_writer_container_ptr,
         OrderEventHandlerPtr order_event_handler_ptr,
         TradeEventHandlerPtr trade_event_handler_ptr,
         DepthEventHandlerPtr depth_event_handler_ptr,
         ExecutionReportPublisherPtr execution_report_publisher_ptr);

  // Getter const functions.

  std::string get_market_name() const;

  std::string get_data_service_name() const;

  const OrderBookMap &get_order_book_map() const;

  const OrderBookStockStatsMapPtr &get_order_book_stats_ptr() const;

  const MarketDataPublisherQueuePtr &
  get_market_data_publisher_queue_ptr() const;

  const DataWriterContainerPtr &get_data_writer_container_ptr() const;

  const ExecutionReportPublisherPtr &get_execution_report_publisher_ptr() const;

  OrderBookPtr find_book(const std::string &symbol) const;

  bool is_ready_to_trade() const;

  // Getter mutable functions.

  OrderBookStockStatsMapPtr &get_order_book_stats_ptr();

  MarketDataPublisherQueuePtr &get_market_data_publisher_queue_ptr();

  DataWriterContainerPtr &get_data_writer_container_ptr();

  ExecutionReportPublisherPtr &get_execution_report_publisher_ptr();

  // Action functions.

  void submit_order(const OrderBookPtr &order_book, const OrderPtr &order);

  void cancel_order(const OrderBookPtr &order_book,
                    const std::string &sender_id, const std::string &order_id);

  void mass_cancel_orders(const std::string &sender_id);

  void cancel_all_orders();

  void set_market_price(const std::string &symbol, Price price);

  void add_book(const std::string &symbol, bool is_depth_book);

private:
  // Market metadata.
  std::string m_market_name;
  std::string m_data_service_name;

  // Market state management containers.
  CustomerOrderMap m_customer_order_map;
  OrderBookMap m_order_book_map;
  OrderBookStockStatsMapPtr m_order_book_stats_ptr;

  // Data writing dependencies.
  DataWriterContainerPtr m_data_writer_container_ptr;
  MarketDataPublisherQueuePtr m_market_data_publisher_queue_ptr;
  ExecutionReportPublisherPtr m_execution_report_publisher_ptr;

  // Event handler dependencies.
  OrderEventHandlerPtr m_order_event_handler_ptr;
  TradeEventHandlerPtr m_trade_event_handler_ptr;
  DepthEventHandlerPtr m_depth_event_handler_ptr;
};