#pragma once
#include "HeaderLogger.hpp"
;

class HeartbeatLogger {
public:
  static void log(std::ostream &out,
                  DistributedStockExchange_Heartbeat::Heartbeat &ddsMsg)
      __attribute__((visibility("default"))) {
    ;
    out << "Message : Heartbeat { " << std::endl;

    ;
    out << "ddsMsg.Source : " << ddsMsg.Source() << std::endl
        << "ddsMsg.Destination : " << ddsMsg.Destination() << std::endl
        << "ddsMsg.SourceUser : " << ddsMsg.SourceUser() << std::endl
        << "ddsMsg.DestinationUser : " << ddsMsg.DestinationUser() << std::endl;

    HeaderLogger::log(out, ddsMsg.fix_header());

    ;
    out;
    out << "}";
    out << std::endl;
  };
};
