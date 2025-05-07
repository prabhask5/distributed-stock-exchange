#include "TradeEventHandler.hpp"

void TradeEventHandler::on_trade(
    const OrderBook *order_book,
    OrderBookStockStatsMapPtr &order_book_stats_map_ptr, Quantity quantity,
    Cost cost) {
  Price market_price = order_book->get_market_price();

  update_symbol_stats(order_book_stats_map_ptr, order_book, quantity,
                      market_price);
}