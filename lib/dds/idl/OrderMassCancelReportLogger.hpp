#pragma once
#include "HeaderLogger.hpp"
;


class OrderMassCancelReportLogger
{
	public:
		static void log(std::ostream & out, DistributedStockExchange_OrderMassCancelReport::OrderMassCancelReport& ddsMsg )  __attribute__ ((visibility ("default")))
		{
			;out<< "Message : OrderMassCancelReport { " << std::endl;

			;out 
			 << "ddsMsg.Source : " << ddsMsg.Source() << std::endl 
			 << "ddsMsg.Destination : " << ddsMsg.Destination() << std::endl
			 << "ddsMsg.SourceUser : " << ddsMsg.SourceUser() << std::endl
			 << "ddsMsg.DestinationUser : " << ddsMsg.DestinationUser() << std::endl;

			HeaderLogger::log(out, ddsMsg.fix_header());

			;out
			 << "ddsMsg.OrderID : " << ddsMsg.OrderID() << std::endl
			 << "ddsMsg.MassCancelRequestType : " << ddsMsg.MassCancelRequestType() << std::endl
			 << "ddsMsg.MassCancelResponse : " << ddsMsg.MassCancelResponse() << std::endl
			 << "ddsMsg.Symbol : " << ddsMsg.Symbol() << std::endl
			 << "ddsMsg.SecurityExchange : " << ddsMsg.SecurityExchange() << std::endl
			 << "ddsMsg.TransactTime : " << ddsMsg.TransactTime() << std::endl
			 << "ddsMsg.Text : " << ddsMsg.Text() << std::endl
;			out << "}";
		out << std::endl;};

};
