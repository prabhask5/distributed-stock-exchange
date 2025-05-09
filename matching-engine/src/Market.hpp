#pragma once

// This class represents the top level abstraction of the matching engine.
// An instance of the market class will contain one or more order books/depth
// order books, and provides all the functionality to take orders in and match
// them with corresponding orders. The market class also manages stats related
// to the matching engine to be sent to the user via reports or to the data
// service.
class Market {

public:
  Market(MarketDataPublisherQueuePtr market_data_publisher_queue_ptr);

  // Getter const functions.

  std::string get_market_name() const;

  OrderBookStockStatsMapPtr get_order_book_stats_ptr() const;

  MarketDataPublisherQueuePtr get_market_data_publisher_queue_ptr() const;

private:
  std::string m_market_name;
  OrderBookStockStatsMapPtr m_order_book_stats_ptr;
  MarketDataPublisherQueuePtr m_market_data_publisher_queue_ptr;
};

using MarketPtr = std::shared_ptr<Market>;