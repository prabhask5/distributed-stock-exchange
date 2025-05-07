#include "DepthOrderBook.hpp"

const DepthLevels &DepthOrderBook::get_levels() const { return m_levels; }

const DepthLevelsSection &DepthOrderBook::get_buy_levels() const {
  return *reinterpret_cast<const DepthLevelsSection *>(&m_levels[0]);
}

const DepthLevelsSection &DepthOrderBook::get_sell_levels() const {
  return *reinterpret_cast<const DepthLevelsSection *>(
      &m_levels[MARKET_DATA_PRICE_DEPTH]);
}

const DepthLevel &DepthOrderBook::get_first_buy_level() const {
  return m_levels[0];
}

const DepthLevel &DepthOrderBook::get_last_buy_level() const {
  return m_levels[MARKET_DATA_PRICE_DEPTH - 1];
}

const DepthLevel &DepthOrderBook::get_first_sell_level() const {
  return m_levels[MARKET_DATA_PRICE_DEPTH];
}

const DepthLevel &DepthOrderBook::get_last_sell_level() const {
  return m_levels[2 * MARKET_DATA_PRICE_DEPTH - 1];
}

DepthLevels &DepthOrderBook::get_levels() { return m_levels; }

DepthLevelsSection &DepthOrderBook::get_buy_levels() {
  return *reinterpret_cast<DepthLevelsSection *>(&m_levels[0]);
}

DepthLevelsSection &DepthOrderBook::get_sell_levels() {
  return *reinterpret_cast<DepthLevelsSection *>(
      &m_levels[MARKET_DATA_PRICE_DEPTH]);
}

DepthLevel &DepthOrderBook::get_first_buy_level() { return m_levels[0]; }

DepthLevel &DepthOrderBook::get_last_buy_level() {
  return m_levels[MARKET_DATA_PRICE_DEPTH - 1];
}

DepthLevel &DepthOrderBook::get_first_sell_level() {
  return m_levels[MARKET_DATA_PRICE_DEPTH];
}

DepthLevel &DepthOrderBook::get_last_sell_level() {
  return m_levels[2 * MARKET_DATA_PRICE_DEPTH - 1];
}
