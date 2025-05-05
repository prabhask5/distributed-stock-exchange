#pragma once
#include "HeaderLogger.hpp"
;

class NewOrderSingleLogger {
public:
  static void
  log(std::ostream &out,
      DistributedStockExchange_NewOrderSingle::NewOrderSingle &ddsMsg)
      __attribute__((visibility("default"))) {
    ;
    out << "Message : NewOrderSingle { " << std::endl;

    ;
    out << "ddsMsg.Source : " << ddsMsg.Source() << std::endl
        << "ddsMsg.Destination : " << ddsMsg.Destination() << std::endl
        << "ddsMsg.SourceUser : " << ddsMsg.SourceUser() << std::endl
        << "ddsMsg.DestinationUser : " << ddsMsg.DestinationUser() << std::endl;

    HeaderLogger::log(out, ddsMsg.fix_header());

    ;
    out << "ddsMsg.ClOrdID : " << ddsMsg.ClOrdID() << std::endl
        << "ddsMsg.ExecInst : " << ddsMsg.ExecInst() << std::endl
        << "ddsMsg.Symbol : " << ddsMsg.Symbol() << std::endl
        << "ddsMsg.SecurityExchange : " << ddsMsg.SecurityExchange()
        << std::endl
        << "ddsMsg.Side : " << ddsMsg.Side() << std::endl
        << "ddsMsg.TransactTime : " << ddsMsg.TransactTime() << std::endl
        << "ddsMsg.OrderQty : " << ddsMsg.OrderQty() << std::endl
        << "ddsMsg.OrdType : " << ddsMsg.OrdType() << std::endl
        << "ddsMsg.Price : " << ddsMsg.Price() << std::endl
        << "ddsMsg.StopPx : " << ddsMsg.StopPx() << std::endl
        << "ddsMsg.TimeInForce : " << ddsMsg.TimeInForce() << std::endl
        << "ddsMsg.Text : " << ddsMsg.Text() << std::endl;
    out << "}";
    out << std::endl;
  };
};
