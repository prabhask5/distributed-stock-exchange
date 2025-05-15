#include "FixMsgTranslatorService.hpp"
#include <DefaultDomainParticipantConstants.hpp>

template <typename T>
FixMsgTranslatorService<T>::FixMsgTranslatorService(
    FixApplication &app, ProcessorFunc<T> processor_func,
    const std::string &name, unsigned long wait_timeout_us = 1000)
    : m_processor_func(processor_func), m_name(name),
      m_wait_interval_us(wait_timeout_us) {
  LOG4CXX_INFO(logger, "Starting processor : [" << m_name << "]");

  std::atomic_init(&m_is_running, true);
  m_publisher_thread = std::thread(&FixMsgTranslatorService<T>::service, this);
}

template <typename T> FixMsgTranslatorService<T>::~FixMsgTranslatorService() {
  LOG4CXX_INFO(logger, "Stopping processor : [" << m_name << "]");

  m_is_running.store(false);
  m_publisher_thread.join();

  LOG4CXX_INFO(logger, "Stopped processor");
}

template <typename T>
void FixMsgTranslatorService<T>::enqueue_dds_message(const T &msg) {
  m_dds_msg_queue.push(msg);
}

template <typename T> void FixMsgTranslatorService<T>::service() {
  while (!m_is_running.load()) {
    T dds_message;
    while (m_dds_msg_queue.try_pop(dds_message)) {
      LOG4CXX_INFO(logger, "Processing: [" << m_name << "]");

      m_processor_func(application, dds_message);

      LOG4CXX_INFO(logger, "Processed: [" << m_name << "]");
    }

    std::this_thread::sleep_for(std::chrono::microseconds(m_wait_interval_us));
  }

  LOG4CXX_INFO(logger, "Exiting processes: [" << m_name << "]");
}