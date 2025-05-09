#pragma once

#include <boost/lockfree/spsc_queue.hpp>
#include <memory>

struct MarketDataUpdate; // From MarketDataUpdate.hpp.
using MarketDataUpdatePtr = std::shared_ptr<MarketDataUpdate>;

// This is a special type of queue called a Single-Producer Single-Consumer
// (SPSC) lock-free queue. This queue adds items from one thread and consumes
// items from a DIFFERENT thread. This queue is lock free and doesn't use atomic
// operations- this is because there is a buffer (head/tail) so the pointers for
// the consumer/producer never cross, leading to no concurrency issues. In the
// context of the distributed stock exchange, this queue takes in market data
// updates from the matching engine (multi-threaded) and sends them to either
// the FIX gateway (if a user requests it), or the data service. The system does
// this through the MarketDataPublisher service thread within the matching
// engine.
using MarketDataPublisherQueue =
    boost::lockfree::spsc_queue<MarketDataUpdatePtr,
                                boost::lockfree::capacity<1024>>;
using MarketDataPublisherQueuePtr = std::shared_ptr<MarketDataPublisherQueue>;