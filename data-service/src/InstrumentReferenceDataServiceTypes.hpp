#pragma once

#include <boost/lockfree/spsc_queue.hpp>
#include <list>
#include <map>
#include <memory>

class Instrument; // From Instrument.hpp.
using InstrumentPtr = std::shared_ptr<Instrument>;

// Maps a stock ticker symbol to an instrument representing the canonical stock
// ticker metadata. One instrument represents the metadata for the whole stock.
using InstrumentMap = std::unordered_map<std::string, InstrumentPtr>;

using InstrumentList = std::list<InstrumentPtr>;
using InstrumentListPtr = std::shared_ptr<InstrumentList>;

// Maps a list of instruments to a username. This represents the list of
// instruments that the user is allowed to access and trade on.
using UserInstrumentListMap =
    std::unordered_map<std::string, InstrumentListPtr>;

// Maps the instrument id to a ref data string, which is a JSON/encoded string
// containing all of the instrument reference data that the matching engine
// needs.
using InstrumentIdToRefDataMap =
    std::unordered_map<std::string, std::shared_ptr<std::string>>;

namespace DistributedStockExchange_SecurityListRequest {
class SecurityListRequest;
}

using SecurityListRequestPtr = std::unique_ptr<
    DistributedStockExchange_SecurityListRequest::SecurityListRequest>;

// This is a special type of queue called a Single-Producer Single-Consumer
// (SPSC) lock-free queue. This queue adds items from one thread and consumes
// items from a DIFFERENT thread. This queue is lock free and doesn't use atomic
// operations- this is because there is a buffer (head/tail) so the pointers for
// the consumer/producer never cross, leading to no concurrency issues. In the
// context of the distributed stock exchange, this queue takes in security list
// requests from either the FIX gateway or matching engine (for initialization)
// from the security list request data reader listener, and handles those,
// surfacing security list reference data to the user or the matching engine.
typedef boost::lockfree::spsc_queue<SecurityListRequestPtr,
                                    boost::lockfree::capacity<1024>>
    SecurityListRequestQueue;
typedef std::shared_ptr<SecurityListRequestQueue> SecurityListRequestQueuePtr;