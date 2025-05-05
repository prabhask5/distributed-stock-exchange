#pragma once
#include "HeaderLogger.hpp"
;

class LogoutLogger {
public:
  static void log(std::ostream &out,
                  DistributedStockExchange_Logout::Logout &ddsMsg)
      __attribute__((visibility("default"))) {
    ;
    out << "Message : Logout { " << std::endl;

    ;
    out << "ddsMsg.Source : " << ddsMsg.Source() << std::endl
        << "ddsMsg.Destination : " << ddsMsg.Destination() << std::endl
        << "ddsMsg.SourceUser : " << ddsMsg.SourceUser() << std::endl
        << "ddsMsg.DestinationUser : " << ddsMsg.DestinationUser() << std::endl;

    HeaderLogger::log(out, ddsMsg.fix_header());

    ;
    out << "ddsMsg.Text : " << ddsMsg.Text() << std::endl;
    out << "}";
    out << std::endl;
  };
};
