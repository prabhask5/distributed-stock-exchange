/* Don't modify, automatically generated file by QuickFIX2FastDDS.py*/
#pragma once
#include "HeaderLogger.hpp"
;

class MarketDataRequestLogger
{
public:
	static void log(std::ostream &out, MarketDataRequest::MarketDataRequest &ddsMsg) __attribute__((visibility("default")))
	{
		;
		out << "Message : MarketDataRequest { " << std::endl;
		HeaderLogger::log(out, ddsMsg.header());

		;
		out
			<< "ddsMsg.MDReqID : " << ddsMsg.MDReqID() << std::endl
			<< "ddsMsg.SubscriptionRequestType : " << ddsMsg.SubscriptionRequestType() << std::endl
			<< "ddsMsg.MarketDepth : " << ddsMsg.MarketDepth() << std::endl

			;
		out << "ddsMsg.c_NoMDEntryTypes" << std::endl;
		out << "{" << std::endl;

		for (int tt = 0; tt < ddsMsg.c_NoMDEntryTypes().size(); tt++)
		{

			;
			out << "ddsMsg.c_NoMDEntryTypes()[" << tt << "].MDEntryType : " << ddsMsg.c_NoMDEntryTypes()[tt].MDEntryType() << std::endl;
		};

		;
		out << "}" << std::endl;

		;
		out << "ddsMsg.c_NoRelatedSym" << std::endl;
		out << "{" << std::endl;

		for (int tt = 0; tt < ddsMsg.c_NoRelatedSym().size(); tt++)
		{

			;
			out << "ddsMsg.c_NoRelatedSym()[" << tt << "].Symbol : " << ddsMsg.c_NoRelatedSym()[tt].Symbol() << std::endl;
			;
			out << "ddsMsg.c_NoRelatedSym()[" << tt << "].SecurityExchange : " << ddsMsg.c_NoRelatedSym()[tt].SecurityExchange() << std::endl;
		};

		;
		out << "}" << std::endl;
		;
		out << "}";
		out << std::endl;
	};
};
