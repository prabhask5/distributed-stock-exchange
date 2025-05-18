#pragma once

#include <boost/lockfree/spsc_queue.hpp>
#include <map>
#include <memory>

namespace DistributedStockExchange_Logon {
class Logon;
}
using LogonPtr = std::unique_ptr<DistributedStockExchange_Logon::Logon>;

// This is a special type of queue called a Single-Producer Single-Consumer
// (SPSC) lock-free queue. This queue adds items from one thread and consumes
// items from a DIFFERENT thread. This queue is lock free and doesn't use atomic
// operations- this is because there is a buffer (head/tail) so the pointers for
// the consumer/producer never cross, leading to no concurrency issues. In the
// context of the distributed stock exchange, this queue takes in logon DDS
// messages from the logon data reader listener service in the data service,
// then the auth service thread pops off those DDS messages to process
// authentication.
using LogonQueue =
    boost::lockfree::spsc_queue<LogonPtr, boost::lockfree::capacity<1024>>;
using LogonQueuePtr = std::shared_ptr<LogonQueue>;

template <typename K, typename V> class LRUCache; // From LRUCache.hpp.

using AuthCache = LRUCache<std::string, std::string>;