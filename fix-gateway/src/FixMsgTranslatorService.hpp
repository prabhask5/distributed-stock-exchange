#pragma once

#include "FixMsgTranslatorServiceTypes.hpp"
#include <DefaultDomainParticipantConstants.hpp>
#include <atomic>
#include <thread>

class FixApplication; // From FixApplication.hpp.

// Service thread dedicated to translating dds messages to FIX messages.
// This is a generic service thread because it is tied to individual FastDDS
// data reader listeners.
template <typename T> class FixMsgTranslatorService {
public:
  FixMsgTranslatorService(FixApplication &app, TranslatorFunc<T> processor_func,
                          const std::string &name,
                          unsigned long wait_timeout_us = 1000)
      : m_processor_func(processor_func), m_name(name),
        m_wait_interval_us(wait_timeout_us) {
    LOG4CXX_INFO(logger, "Starting processor : [" << m_name << "]");

    std::atomic_init(&m_is_running, true);
    m_publisher_thread = std::thread([&]() {
      while (m_is_running.load()) {
        T dds_message;
        while (!m_dds_msg_queue.empty()) {
          m_dds_msg_queue.pop(dds_message);

          LOG4CXX_INFO(logger, "Processing: [" << m_name << "]");

          m_processor_func(app, dds_message);

          LOG4CXX_INFO(logger, "Processed: [" << m_name << "]");
        }

        std::this_thread::sleep_for(
            std::chrono::microseconds(m_wait_interval_us));
      }

      LOG4CXX_INFO(logger, "Exiting processes: [" << m_name << "]");
    });
  }

  ~FixMsgTranslatorService() {
    LOG4CXX_INFO(logger, "Stopping processor : [" << m_name << "]");

    m_is_running.store(false);
    m_publisher_thread.join();

    LOG4CXX_INFO(logger, "Stopped processor");
  }

  void enqueue_dds_message(const T &msg) { m_dds_msg_queue.push(msg); }

private:
  // Service thread metadata.
  std::string m_name;
  std::atomic<bool> m_is_running;
  std::thread m_publisher_thread;
  unsigned long m_wait_interval_us;
  TranslatorFunc<T> m_processor_func;

  // Service state management.
  SPSCQueue<T> m_dds_msg_queue;
};