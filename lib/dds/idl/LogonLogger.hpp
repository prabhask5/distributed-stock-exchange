#pragma once
#include "HeaderLogger.hpp"
;

class LogonLogger {
public:
  static void log(std::ostream &out,
                  DistributedStockExchange_Logon::Logon &ddsMsg)
      __attribute__((visibility("default"))) {
    ;
    out << "Message : Logon { " << std::endl;

    ;
    out << "ddsMsg.Source : " << ddsMsg.Source() << std::endl
        << "ddsMsg.Destination : " << ddsMsg.Destination() << std::endl
        << "ddsMsg.SourceUser : " << ddsMsg.SourceUser() << std::endl
        << "ddsMsg.DestinationUser : " << ddsMsg.DestinationUser() << std::endl;

    HeaderLogger::log(out, ddsMsg.fix_header());

    ;
    out << "ddsMsg.EncryptMethod : " << ddsMsg.EncryptMethod() << std::endl
        << "ddsMsg.HeartBtInt : " << ddsMsg.HeartBtInt() << std::endl
        << "ddsMsg.RawData : " << ddsMsg.RawData() << std::endl
        << "ddsMsg.Username : " << ddsMsg.Username() << std::endl
        << "ddsMsg.Password : " << ddsMsg.Password() << std::endl;
    out << "}";
    out << std::endl;
  };
};
