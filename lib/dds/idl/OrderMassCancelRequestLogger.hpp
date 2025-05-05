#pragma once
#include "HeaderLogger.hpp"
;

class OrderMassCancelRequestLogger {
public:
  static void
  log(std::ostream &out,
      DistributedStockExchange_OrderMassCancelRequest::OrderMassCancelRequest
          &ddsMsg) __attribute__((visibility("default"))) {
    ;
    out << "Message : OrderMassCancelRequest { " << std::endl;

    ;
    out << "ddsMsg.Source : " << ddsMsg.Source() << std::endl
        << "ddsMsg.Destination : " << ddsMsg.Destination() << std::endl
        << "ddsMsg.SourceUser : " << ddsMsg.SourceUser() << std::endl
        << "ddsMsg.DestinationUser : " << ddsMsg.DestinationUser() << std::endl;

    HeaderLogger::log(out, ddsMsg.fix_header());

    ;
    out << "ddsMsg.ClOrdID : " << ddsMsg.ClOrdID() << std::endl
        << "ddsMsg.MassCancelRequestType : " << ddsMsg.MassCancelRequestType()
        << std::endl
        << "ddsMsg.Symbol : " << ddsMsg.Symbol() << std::endl
        << "ddsMsg.SecurityExchange : " << ddsMsg.SecurityExchange()
        << std::endl
        << "ddsMsg.TransactTime : " << ddsMsg.TransactTime() << std::endl
        << "ddsMsg.Text : " << ddsMsg.Text() << std::endl;
    out << "}";
    out << std::endl;
  };
};
