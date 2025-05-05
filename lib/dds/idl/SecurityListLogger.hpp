#pragma once
#include "HeaderLogger.hpp"
;

class SecurityListLogger {
public:
  static void log(std::ostream &out,
                  DistributedStockExchange_SecurityList::SecurityList &ddsMsg)
      __attribute__((visibility("default"))) {
    ;
    out << "Message : SecurityList { " << std::endl;

    ;
    out << "ddsMsg.Source : " << ddsMsg.Source() << std::endl
        << "ddsMsg.Destination : " << ddsMsg.Destination() << std::endl
        << "ddsMsg.SourceUser : " << ddsMsg.SourceUser() << std::endl
        << "ddsMsg.DestinationUser : " << ddsMsg.DestinationUser() << std::endl;

    HeaderLogger::log(out, ddsMsg.fix_header());

    ;
    out << "ddsMsg.SecurityReqID : " << ddsMsg.SecurityReqID() << std::endl
        << "ddsMsg.SecurityResponseID : " << ddsMsg.SecurityResponseID()
        << std::endl
        << "ddsMsg.SecurityRequestResult : " << ddsMsg.SecurityRequestResult()
        << std::endl

        ;
    out << "ddsMsg.c_NoRelatedSym" << std::endl;
    out << "{" << std::endl;

    for (int tt = 0; tt < ddsMsg.c_NoRelatedSym().size(); tt++) {

      ;
      out << "ddsMsg.c_NoRelatedSym()[" << tt
          << "].Symbol : " << ddsMsg.c_NoRelatedSym()[tt].Symbol() << std::endl;
      ;
      out << "ddsMsg.c_NoRelatedSym()[" << tt << "].SecurityExchange : "
          << ddsMsg.c_NoRelatedSym()[tt].SecurityExchange() << std::endl;
      ;
      out << "ddsMsg.c_NoRelatedSym()[" << tt
          << "].Text : " << ddsMsg.c_NoRelatedSym()[tt].Text() << std::endl;
    };

    ;
    out << "}" << std::endl;
    ;
    out << "}";
    out << std::endl;
  };
};
