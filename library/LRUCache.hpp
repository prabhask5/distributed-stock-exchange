#pragma once

#include "LRUCacheTypes.hpp"
#include <list>
#include <map>

const int DEFAULT_CAPACITY = 256;

template <typename K, typename V> class LRUCache {
public:
  LRUCache(RefreshFunc<K, V> refresh_func, size_t capacity = DEFAULT_CAPACITY)
      : m_refresh_func(refresh_func), m_capacity(capacity) {}

  // Get should either get the key in the cache if present (and update the state
  // of the LRU cache) or use the refresh function included to get that value.
  V get(const K &key) {
    auto iter = m_key_to_node.find(key);

    if (iter != m_key_to_node.end()) {
      // Move to front (most recently used)
      m_nodes.splice(m_nodes.begin(), m_nodes, it->second.second);
      return it->second.first;
    }

    // If not in cache, get it using the refresh function.
    V value = m_refresh_func(key);

    // Evict if necessary.
    if (m_key_to_node.size() >= m_capacity) {
      K lru_key = m_nodes.back();
      m_nodes.pop_back();
      m_key_to_node.erase(lru_key);
    }

    // Insert the new item.
    m_nodes.push_front(key);
    m_key_to_node[key] = {value, m_nodes.begin()};

    return value;
  }

private:
  size_t m_capacity;
  RefreshFunc<K, V> m_refresh_func;
  std::list<K> m_nodes;
  std::unordered_map<K, std::pair<V, Node>> m_key_to_node;
};