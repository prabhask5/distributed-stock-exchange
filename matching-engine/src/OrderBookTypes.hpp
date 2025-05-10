#pragma once

#include "OrderTypes.hpp"
#include <list>
#include <map>

class OrderPrice; // From OrderPrice.hpp.

// The reason we use a multimap instead of a priority queue to manage orders
// within the order book is because the multimap allows us the ability to both
// iterate through all the orders (in sorted order) and quickly find all the
// orders for a certain price within the order book. With the priority queue, we
// only have access to the topmost element, which can make some functionality of
// the order book difficult to implement (i.e cancelling orders, finding/sorting
// orders in terms of time priority, scaning multiple price levels).
// Additionally, a map is super helpful when dealing with deferred orders
// (orders that cannot be matched currently, but later), since we need to find
// the order from the map using the information from the list, this is not
// possible with the priority queue.
using OrderMap = std::multimap<OrderPrice, OrderPtr>;

using OrderVec = std::vector<OrderPtr>;

// This list keeps track of the orders that we skipped during this match run for
// any reason.
using DeferredMatchList = std::list<typename OrderMap::iterator>;