#include "Market.hpp"
#include "MarketDataUpdate.hpp"
#include "OrderBookStockStatistics.hpp"
#include <memory>
#include <string>

Market::Market(
    const std::string &market_name, const std::string &data_service_name,
    const MarketDataPublisherQueuePtr &market_data_publisher_queue_ptr,
    DataWriterContainerPtr data_writer_container_ptr,
    OrderEventHandlerPtr order_event_handler_ptr,
    TradeEventHandlerPtr trade_event_handler_ptr,
    DepthEventHandlerPtr depth_event_handler_ptr,
    ExecutionReportPublisherPtr execution_report_publisher_ptr) { /* TODO */ }

std::string Market::get_market_name() const { /* TODO */ }

const OrderBookStockStatsMapPtr &
Market::get_order_book_stats_ptr() const { /* TODO */ }

const MarketDataPublisherQueuePtr &
Market::get_market_data_publisher_queue_ptr() const { /* TODO */ }

const DataWriterContainerPtr &
Market::get_data_writer_container_ptr() const { /* TODO */ }

const OrderEventHandlerPtr &
Market::get_order_event_handler_ptr() const { /* TODO */ }

const TradeEventHandlerPtr &
Market::get_trade_event_handler_ptr() const { /* TODO */ }

const DepthEventHandlerPtr &
Market::get_depth_event_handler_ptr() const { /* TODO */ }

const ExecutionReportPublisherPtr &
Market::get_execution_report_publisher_ptr() const { /* TODO */ }

OrderBookPtr &Market::find_book(const std::string &symbol) const { /* TODO */ }

bool Market::is_ready_to_trade() const { /* TODO */ }

OrderBookStockStatsMapPtr &Market::get_order_book_stats_ptr() { /* TODO */ }

MarketDataPublisherQueuePtr &
Market::get_market_data_publisher_queue_ptr() { /* TODO */ }

DataWriterContainerPtr &Market::get_data_writer_container_ptr() { /* TODO */ }

OrderEventHandlerPtr &Market::get_order_event_handler_ptr() { /* TODO */ }

TradeEventHandlerPtr &Market::get_trade_event_handler_ptr() { /* TODO */ }

DepthEventHandlerPtr &Market::get_depth_event_handler_ptr() { /* TODO */ }

ExecutionReportPublisherPtr &
Market::get_execution_report_publisher_ptr() { /* TODO */ }

void Market::submit(const OrderBookPtr &order_book,
                    const OrderPtr &order) { /* TODO */ }

void Market::cancel(const OrderBookPtr &order_book,
                    const std::string &sender_id,
                    const std::string &order_id) { /* TODO */ }

void Market::mass_cancel(const std::string &sender_id) { /* TODO */ }

void Market::cancel_all_orders() { /* TODO */ }

void Market::set_market_price(const std::string &symbol,
                              Price price) { /* TODO */ }

void Market::add_book(const std::string &symbol,
                      bool is_depth_book) { /* TODO */ }