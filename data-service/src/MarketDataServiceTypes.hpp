#pragma once

#include <boost/lockfree/spsc_queue.hpp>
#include <list>
#include <map>
#include <memory>

namespace DistributedStockExchange_MarketDataIncrementalRefresh {
class NoMDEntries;
}

namespace DistributedStockExchange_MarketDataRequest {
class MarketDataRequest;
}

using MarketDataEntryList = std::list<
    DistributedStockExchange_MarketDataIncrementalRefresh::NoMDEntries>;

struct Instrument; // From Instrument.hpp.
using IncrementalRefreshMap =
    std::unordered_map<Instrument, MarketDataEntryList>;
using IncrementalRefreshMapPtr = std::shared_ptr<IncrementalRefreshMap>;

using MarketDataRequestPtr = std::unique_ptr<
    DistributedStockExchange_MarketDataRequest::MarketDataRequest>;

// This is a special type of queue called a Single-Producer Single-Consumer
// (SPSC) lock-free queue. This queue adds items from one thread and consumes
// items from a DIFFERENT thread. This queue is lock free and doesn't use atomic
// operations- this is because there is a buffer (head/tail) so the pointers for
// the consumer/producer never cross, leading to no concurrency issues. In the
// context of the distributed stock exchange, this queue takes in market data
// request DDS messages from the data reader listener, and the market date
// service thread processes each of the messages.
using MarketDataRequestQueue =
    boost::lockfree::spsc_queue<MarketDataRequestPtr,
                                boost::lockfree::capacity<1024>>;
using MarketDataRequestQueuePtr = std::shared_ptr<MarketDataRequestQueue>;