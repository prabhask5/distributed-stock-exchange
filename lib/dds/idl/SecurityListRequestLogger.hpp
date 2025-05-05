#pragma once
#include "HeaderLogger.hpp"
;

class SecurityListRequestLogger {
public:
  static void
  log(std::ostream &out,
      DistributedStockExchange_SecurityListRequest::SecurityListRequest &ddsMsg)
      __attribute__((visibility("default"))) {
    ;
    out << "Message : SecurityListRequest { " << std::endl;

    ;
    out << "ddsMsg.Source : " << ddsMsg.Source() << std::endl
        << "ddsMsg.Destination : " << ddsMsg.Destination() << std::endl
        << "ddsMsg.SourceUser : " << ddsMsg.SourceUser() << std::endl
        << "ddsMsg.DestinationUser : " << ddsMsg.DestinationUser() << std::endl;

    HeaderLogger::log(out, ddsMsg.fix_header());

    ;
    out << "ddsMsg.SecurityReqID : " << ddsMsg.SecurityReqID() << std::endl
        << "ddsMsg.SecurityListRequestType : "
        << ddsMsg.SecurityListRequestType() << std::endl
        << "ddsMsg.Symbol : " << ddsMsg.Symbol() << std::endl
        << "ddsMsg.SecurityExchange : " << ddsMsg.SecurityExchange()
        << std::endl
        << "ddsMsg.Text : " << ddsMsg.Text() << std::endl;
    out << "}";
    out << std::endl;
  };
};
