#include "Market.hpp"
#include "DepthOrderBook.hpp"
#include "DepthOrderBookTypes.hpp"
#include "MarketDataUpdate.hpp"
#include "Order.hpp"
#include "OrderBookStockStatistics.hpp"
#include "OrderException.hpp"
#include <DefaultDomainParticipantConstants.hpp>
#include <memory>
#include <quickfix/FixValues.h>
#include <string>

Market::Market(
    const std::string &market_name, const std::string &data_service_name,
    const MarketDataPublisherQueuePtr &market_data_publisher_queue_ptr,
    DataWriterContainerPtr data_writer_container_ptr,
    OrderEventHandlerPtr order_event_handler_ptr,
    TradeEventHandlerPtr trade_event_handler_ptr,
    DepthEventHandlerPtr depth_event_handler_ptr,
    ExecutionReportPublisherPtr execution_report_publisher_ptr)
    : m_market_name(market_name), m_data_service_name(data_service_name),
      m_market_data_publisher_queue_ptr(market_data_publisher_queue_ptr),
      m_data_writer_container_ptr(data_writer_container_ptr),
      m_order_event_handler_ptr(order_event_handler_ptr),
      m_trade_event_handler_ptr(trade_event_handler_ptr),
      m_depth_event_handler_ptr(depth_event_handler_ptr),
      m_execution_report_publisher_ptr(execution_report_publisher_ptr) {}

std::string Market::get_market_name() const { return m_market_name; }

std::string Market::get_data_service_name() const {
  return m_data_service_name;
}

const OrderBookMap &Market::get_order_book_map() const {
  return m_order_book_map;
}

const OrderBookStockStatsMapPtr &Market::get_order_book_stats_ptr() const {
  return m_order_book_stats_ptr;
}

const MarketDataPublisherQueuePtr &
Market::get_market_data_publisher_queue_ptr() const {
  return m_market_data_publisher_queue_ptr;
}

const DataWriterContainerPtr &Market::get_data_writer_container_ptr() const {
  return m_data_writer_container_ptr;
}

const ExecutionReportPublisherPtr &
Market::get_execution_report_publisher_ptr() const {
  return m_execution_report_publisher_ptr;
}

OrderBookPtr Market::find_book(const std::string &symbol) const {
  auto entry = m_order_book_map.find(symbol);
  if (entry != m_order_book_map.end()) {
    return entry->second;
  }
  return nullptr;
}

bool Market::is_ready_to_trade() const { return m_order_book_map.size() > 0; }

OrderBookStockStatsMapPtr &Market::get_order_book_stats_ptr() {
  return m_order_book_stats_ptr;
}

MarketDataPublisherQueuePtr &Market::get_market_data_publisher_queue_ptr() {
  return m_market_data_publisher_queue_ptr;
}

DataWriterContainerPtr &Market::get_data_writer_container_ptr() {
  return m_data_writer_container_ptr;
}

ExecutionReportPublisherPtr &Market::get_execution_report_publisher_ptr() {
  return m_execution_report_publisher_ptr;
}

void Market::submit_order(const OrderBookPtr &order_book,
                          const OrderPtr &order) {
  auto orders_for_sender_id_iter =
      m_customer_order_map.find(order->get_sender_id());

  OrderMapPtr customer_orders;

  // If we don't find the order map for the sender_id, just make one.
  if (orders_for_sender_id_iter == m_customer_order_map.end()) {
    auto [it, inserted] = m_customer_order_map.emplace(
        order->get_sender_id(), std::make_shared<OrderMap>());
    customer_orders = it->second;
  } else {
    customer_orders = orders_for_sender_id_iter->second;
  }

  auto existing_order_iter = customer_orders->find(order->get_order_id());

  // Cannot add an order that already exists (with the same order_id).
  if (existing_order_iter != customer_orders->end()) {
    throw OrderException(order->get_order_id(),
                         FIX::OrdRejReason_DUPLICATE_ORDER);
  }

  customer_orders->emplace(order->get_order_id(), order);
  order_book->add(order);
}

void Market::cancel_order(const OrderBookPtr &order_book,
                          const std::string &sender_id,
                          const std::string &order_id) {
  auto orders_for_sender_id_iter = m_customer_order_map.find(sender_id);

  // Can't find the order map for this customer.
  if (orders_for_sender_id_iter == m_customer_order_map.end()) {
    LOG4CXX_ERROR(logger, "Customer order map for given sender_id not found: "
                              << sender_id);
    return;
  }

  auto existing_order_iter = orders_for_sender_id_iter->second->find(order_id);

  // Can't find the order for this order_id.
  if (existing_order_iter == orders_for_sender_id_iter->second->end()) {
    throw OrderException(order_id, FIX::CxlRejReason_UNKNOWN_ORDER);
  }

  order_book->cancel(existing_order_iter->second);
}

void Market::mass_cancel_orders(const std::string &sender_id) {
  auto orders_for_sender_id_iter = m_customer_order_map.find(sender_id);

  // Can't find the order map for this customer.
  if (orders_for_sender_id_iter == m_customer_order_map.end()) {
    LOG4CXX_ERROR(logger, "Customer order map for given sender_id not found: "
                              << sender_id);
    return;
  }

  size_t num_orders_to_be_cancelled = orders_for_sender_id_iter->second->size();

  for (auto order_iter : *orders_for_sender_id_iter->second) {
    OrderPtr &order = order_iter.second;
    OrderBookPtr order_book = find_book(order->get_symbol());

    if (!order_book)
      LOG4CXX_ERROR(logger, "Book not found:" << order->get_symbol());
    else if (order->get_quantity_in_market() > 0)
      order_book->cancel(order);
  }

  m_customer_order_map.erase(orders_for_sender_id_iter);
}

void Market::cancel_all_orders() {
  for (auto customer_order_map_iter : m_customer_order_map) {
    for (auto order_iter : *customer_order_map_iter.second) {
      OrderPtr order = order_iter.second;

      OrderBookPtr order_book = find_book(order->get_symbol());
      if (!order_book)
        LOG4CXX_ERROR(logger, "Book not found:" << order->get_symbol());
      else
        order_book->cancel(order);
    }
  }
}

void Market::set_market_price(const std::string &symbol, Price price) {
  OrderBookPtr order_book = find_book(symbol);
  if (!order_book) {
    LOG4CXX_ERROR(logger, "Book not found:" << symbol);
    return;
  }

  std::stringstream ss;
  LOG4CXX_INFO(logger, "Setting market price:" << symbol << price);
  update_symbol_stats(m_order_book_stats_ptr, *order_book, 0, price);
  order_book->set_market_price(price);
}

void Market::add_book(const std::string &symbol, bool is_depth_book) {
  // NOTE: This function assumes all the event handlers are set before calling.

  OrderBookPtr order_book;

  if (is_depth_book) {
    DepthOrderBookPtr depth_order_book =
        std::make_shared<DepthOrderBook>(symbol);
    depth_order_book->set_depth_event_handler_ptr(m_depth_event_handler_ptr);
    order_book = depth_order_book;
  } else {
    order_book = std::make_shared<OrderBook>(symbol);
  }

  order_book->set_order_event_handler_ptr(m_order_event_handler_ptr);
  order_book->set_trade_event_handler_ptr(m_trade_event_handler_ptr);

  m_order_book_map[symbol] = order_book;
}