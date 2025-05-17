#pragma once

#include <log4cxx/basicconfigurator.h>
#include <log4cxx/logger.h>

template <class STREAM, class DSE_LOGGER, class DSE_CLASS>
inline void log_debug(const LOG4CXX_NS::LoggerPtr &logger, DSE_CLASS &obj,
                      const std::string &context) {
  STREAM ss;
  DSE_LOGGER::log(ss, obj);
  LOG4CXX_DEBUG(logger, context << ":" << ss.str());
};

template <class STREAM, class DSE_LOGGER, class DSE_CLASS>
inline void log_info(const LOG4CXX_NS::LoggerPtr &logger, DSE_CLASS &obj,
                     const std::string &context) {
  STREAM ss;
  DSE_LOGGER::log(ss, obj);
  LOG4CXX_INFO(logger, context << ":" << ss.str());
};