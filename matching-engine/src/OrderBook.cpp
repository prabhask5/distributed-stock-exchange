#include "OrderBook.hpp"

OrderBook::OrderBook(const std::string &symbol)
    : m_symbol(symbol), m_market_price(0) {}

const std::string &OrderBook::get_symbol() const { return m_symbol; }

Price OrderBook::get_market_price() const { return m_market_price; }

void OrderBook::set_symbol(const std::string &symbol) { m_symbol = symbol; }

void OrderBook::set_market_price(Price market_price) {
  m_market_price = market_price;
}
