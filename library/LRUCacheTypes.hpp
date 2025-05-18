#pragma once

#include <functional>
#include <list>

template <typename K, typename V>
using RefreshFunc = std::function<V(const K &)>;

template <typename K> using Node = std::list<K>::iterator;