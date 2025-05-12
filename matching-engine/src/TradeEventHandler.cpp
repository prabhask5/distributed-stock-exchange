#include "TradeEventHandler.hpp"
#include "OrderBook.hpp"
#include "OrderBookStockStatistics.hpp"

TradeEventHandler::TradeEventHandler(
    const OrderBookStockStatsMapPtr &order_book_stats_map_ptr)
    : m_order_book_stats_map_ptr(order_book_stats_map_ptr) {}

void TradeEventHandler::on_trade(const OrderBook *order_book, Quantity quantity,
                                 Cost cost) {
  Price market_price = order_book->get_market_price();

  update_symbol_stats(m_order_book_stats_map_ptr, *order_book, quantity,
                      market_price);
}