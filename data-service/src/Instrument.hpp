#pragma once

#include <boost/functional/hash.hpp>
#include <string>

// This struct is a simple representation of a financial instrument (i.e stock)
// in the context of the stock exchange.
struct Instrument {
  Instrument() = default;

  Instrument(Instrument const &) = default;

  Instrument(Instrument &&) = default;

  Instrument &operator=(Instrument &) = default;

  Instrument &operator=(Instrument &&) = default;

  ~Instrument() = default;

  Instrument(const char *market_name, const char *symbol,
             const char *properties = "")
      : market_name(market_name), symbol(symbol), properties(properties) {};

  Instrument(const std::string &market_name, const std::string &symbol,
             const std::string properties = "")
      : market_name(market_name), symbol(symbol), properties(properties) {};

  std::string symbol;
  std::string market_name;
  std::string properties;

  friend bool operator<(const Instrument &i1, const Instrument &i2) {
    return (i1.symbol + i1.market_name < i2.symbol + i2.market_name);
  };

  friend bool operator==(const Instrument &i1, const Instrument &i2) {
    return (i1.symbol == i2.symbol && i1.market_name == i2.market_name);
  };
};

template <> struct std::hash<Instrument> {
  size_t operator()(const Instrument &instr) const {
    std::size_t seed = 0;
    boost::hash_combine(seed, instr.symbol);
    boost::hash_combine(seed, instr.market_name);
    boost::hash_combine(seed, instr.properties);

    return seed;
  }
};