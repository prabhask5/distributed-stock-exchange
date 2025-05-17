#pragma once

#include <boost/lockfree/spsc_queue.hpp>
#include <functional>

class FixApplication; // From FixApplication.hpp.

template <typename T>
using TranslatorFunc = std::function<void(FixApplication &, T &)>;

// This is a special type of queue called a Single-Producer Single-Consumer
// (SPSC) lock-free queue. This queue adds items from one thread and consumes
// items from a DIFFERENT thread. This queue is lock free and doesn't use atomic
// operations- this is because there is a buffer (head/tail) so the pointers for
// the consumer/producer never cross, leading to no concurrency issues. In the
// context of the distributed stock exchange, this queue takes in DDS messages
// from the data reader listener that uses the fix msg translator service, and
// is popped off by the fix msg translator service thread.
template <typename T>
using SPSCQueue =
    boost::lockfree::spsc_queue<T, boost::lockfree::capacity<1024>>;