#include "OrderPrice.hpp"
#include "OrderConstants.hpp"
#include "OrderTypes.hpp"
#include <cstdint>
#include <ostream>

OrderPrice::OrderPrice(Price price, bool is_buy)
    : m_price(price), m_is_buy(is_buy) {}

Price OrderPrice::get_price() const { return m_price; }

bool OrderPrice::is_buy() const { return m_is_buy; }

bool OrderPrice::is_market_order() const {
  return m_price == MARKET_ORDER_PRICE;
}

bool OrderPrice::matches(Price other_price) const {
  Price my_price = m_price;

  if (my_price == other_price) {
    return true;
  }
  if (m_is_buy) {
    return other_price < my_price || my_price == MARKET_ORDER_PRICE;
  }

  return my_price < other_price || other_price == MARKET_ORDER_PRICE;
}

bool OrderPrice::matches(const OrderPrice &other) const {
  return this->matches(other.get_price());
}

bool OrderPrice::operator<(Price other_price) const {
  Price my_price = m_price;

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

bool OrderPrice::operator<(const OrderPrice &other) const {
  return *this < other.get_price();
}

bool OrderPrice::operator==(Price other_price) const {
  Price my_price = m_price;

  return my_price == other_price;
}

bool OrderPrice::operator==(const OrderPrice &other) const {
  return *this == other.get_price();
}

bool OrderPrice::operator!=(Price other_price) const {
  Price my_price = m_price;

  return my_price != other_price;
}

bool OrderPrice::operator!=(const OrderPrice &other) const {
  return *this == other.get_price();
}

bool OrderPrice::operator>(Price other_price) const {
  Price my_price = m_price;

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

bool OrderPrice::operator>(const OrderPrice &other) const {
  return *this > other.get_price();
}

bool OrderPrice::operator<=(Price other_price) const {
  return *this < other_price || *this == other_price;
}

bool OrderPrice::operator>=(Price other_price) const {
  return *this > other_price || *this == other_price;
}

bool operator<(Price price, const OrderPrice &key) { return key > price; }

bool operator>(Price price, const OrderPrice &key) { return key < price; }

bool operator==(Price price, const OrderPrice &key) { return key == price; }

bool operator!=(Price price, const OrderPrice &key) { return key != price; }

bool operator<=(Price price, const OrderPrice &key) { return key >= price; }

bool operator>=(Price price, const OrderPrice &key) { return key <= price; }

std::ostream &operator<<(std::ostream &out, const OrderPrice &key) {
  out << (key.is_buy() ? "Buy at " : "Sell at ");
  if (key.is_market_order()) {
    out << "Market";
  } else {
    out << key.get_price();
  }
  return out;
}