#pragma once
#include "HeaderLogger.hpp"
;


class OrderMassStatusRequestLogger
{
	public:
		static void log(std::ostream & out, DistributedStockExchange_OrderMassStatusRequest::OrderMassStatusRequest& ddsMsg )  __attribute__ ((visibility ("default")))
		{
			;out<< "Message : OrderMassStatusRequest { " << std::endl;

			;out 
			 << "ddsMsg.Source : " << ddsMsg.Source() << std::endl 
			 << "ddsMsg.Destination : " << ddsMsg.Destination() << std::endl
			 << "ddsMsg.SourceUser : " << ddsMsg.SourceUser() << std::endl
			 << "ddsMsg.DestinationUser : " << ddsMsg.DestinationUser() << std::endl;

			HeaderLogger::log(out, ddsMsg.fix_header());

			;out
			 << "ddsMsg.MassStatusReqID : " << ddsMsg.MassStatusReqID() << std::endl
			 << "ddsMsg.MassStatusReqType : " << ddsMsg.MassStatusReqType() << std::endl
			 << "ddsMsg.Symbol : " << ddsMsg.Symbol() << std::endl
			 << "ddsMsg.SecurityExchange : " << ddsMsg.SecurityExchange() << std::endl
;			out << "}";
		out << std::endl;};

};
