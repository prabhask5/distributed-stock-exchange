#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>

template <typename T> class ThreadSafeQueue {
public:
  void push(T const &v) {
    std::unique_lock<std::mutex> lock(m_mutex);

    m_inner_queue.push(v);

    lock.unlock();
    m_cond_var.notify_one();
  }

  bool empty() const {
    std::unique_lock<std::mutex> lock(m_mutex);

    return m_inner_queue.empty();
  }

  bool try_pop(T &popped_value) {
    std::unique_lock<std::mutex> lock(m_mutex);

    if (m_inner_queue.empty())
      return false;

    popped_value = m_inner_queue.front();
    m_inner_queue.pop();

    return true;
  }

  void wait_and_pop(T &popped_value) {
    std::unique_lock<std::mutex> lock(m_mutex);

    while (m_inner_queue.empty())
      m_cond_var.wait(lock);

    popped_value = m_inner_queue.front();
    m_inner_queue.pop();
  }

private:
  std::queue<T> m_inner_queue;
  std::mutex m_mutex;
  std::condition_variable m_cond_var;
};