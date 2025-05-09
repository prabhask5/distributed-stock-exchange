#include "OrderPrice.hpp"
#include <cstdint>

OrderPrice::OrderPrice(Price price, bool is_buy)
    : m_price(price), m_is_buy(is_buy) {}

Price OrderPrice::get_price() const { return m_price; }

bool OrderPrice::is_buy() const { return m_is_buy; }

bool OrderPrice::is_market_order() const {
  return m_price == MARKET_ORDER_PRICE;
}

bool OrderPrice::matches(const OrderPrice &other) const {
  Price my_price = m_price;
  Price other_price = other.get_price();

  if (my_price == other_price) {
    return true;
  }
  if (m_is_buy) {
    return other_price < my_price || my_price == MARKET_ORDER_PRICE;
  }

  return my_price < other_price || other_price == MARKET_ORDER_PRICE;
}

bool OrderPrice::operator<(const OrderPrice &other) const {
  Price my_price = m_price;
  Price other_price = other.get_price();

  if (my_price == MARKET_ORDER_PRICE && other_price != MARKET_ORDER_PRICE) {
    return true;
  }
  if (my_price != MARKET_ORDER_PRICE && other_price == MARKET_ORDER_PRICE) {
    return false;
  }

  if (m_is_buy) {
    return my_price < other_price; // Buying: Highest prices first.
  } else {
    return my_price > other_price; // Selling: lowest prices first.
  }
}

bool OrderPrice::operator==(const OrderPrice &other) const {
  Price my_price = m_price;
  Price other_price = other.get_price();

  return my_price == other_price;
}

bool OrderPrice::operator!=(const OrderPrice &other) const {
  Price my_price = m_price;
  Price other_price = other.get_price();

  return my_price != other_price;
}

bool OrderPrice::operator>(const OrderPrice &other) const {
  Price my_price = m_price;
  Price other_price = other.get_price();

  if (my_price != MARKET_ORDER_PRICE && other_price == MARKET_ORDER_PRICE) {
    return true;
  }
  if (my_price == MARKET_ORDER_PRICE && other_price != MARKET_ORDER_PRICE) {
    return false;
  }

  if (m_is_buy) {
    return my_price > other_price; // Buying: Highest prices first.
  } else {
    return my_price < other_price; // Selling: lowest prices first.
  }
}