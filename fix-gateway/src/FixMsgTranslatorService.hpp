#pragma once

#include "FixMsgTranslatorServiceTypes.hpp"
#include <ThreadSafeQueue.hpp>
#include <atomic>
#include <thread>

class FixApplication; // From FixApplication.hpp.

// Service thread dedicated to translating dds messages to FIX messages.
// This is a generic service thread because it is tied to individual FastDDS
// data reader listeners.
template <typename T> class FixMsgTranslatorService {
public:
  FixMsgTranslatorService(FixApplication &app, ProcessorFunc<T> processor_func,
                          const std::string &name,
                          unsigned long wait_timeout_us = 1000);

  ~FixMsgTranslatorService();

  void enqueue_dds_message(const T &msg);

  void service();

private:
  // Service thread metadata.
  std::string m_name;
  std::atomic<bool> m_is_running;
  std::thread m_publisher_thread;
  unsigned long m_wait_interval_us;
  ProcessorFunc<T> m_processor_func;

  // Service state management.
  ThreadSafeQueue<T> m_dds_msg_queue;
};