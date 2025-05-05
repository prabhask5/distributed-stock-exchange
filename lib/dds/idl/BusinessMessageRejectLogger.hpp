#pragma once
#include "HeaderLogger.hpp"
;


class BusinessMessageRejectLogger
{
	public:
		static void log(std::ostream & out, DistributedStockExchange_BusinessMessageReject::BusinessMessageReject& ddsMsg )  __attribute__ ((visibility ("default")))
		{
			;out<< "Message : BusinessMessageReject { " << std::endl;

			;out 
			 << "ddsMsg.Source : " << ddsMsg.Source() << std::endl 
			 << "ddsMsg.Destination : " << ddsMsg.Destination() << std::endl
			 << "ddsMsg.SourceUser : " << ddsMsg.SourceUser() << std::endl
			 << "ddsMsg.DestinationUser : " << ddsMsg.DestinationUser() << std::endl;

			HeaderLogger::log(out, ddsMsg.fix_header());

			;out
			 << "ddsMsg.RefMsgType : " << ddsMsg.RefMsgType() << std::endl
			 << "ddsMsg.BusinessRejectReason : " << ddsMsg.BusinessRejectReason() << std::endl
			 << "ddsMsg.Text : " << ddsMsg.Text() << std::endl
;			out << "}";
		out << std::endl;};

};
