#pragma once
#include "HeaderLogger.hpp"
;


class MarketDataIncrementalRefreshLogger
{
	public:
		static void log(std::ostream & out, DistributedStockExchange_MarketDataIncrementalRefresh::MarketDataIncrementalRefresh& ddsMsg )  __attribute__ ((visibility ("default")))
		{
			;out<< "Message : MarketDataIncrementalRefresh { " << std::endl;

			;out 
			 << "ddsMsg.Source : " << ddsMsg.Source() << std::endl 
			 << "ddsMsg.Destination : " << ddsMsg.Destination() << std::endl
			 << "ddsMsg.SourceUser : " << ddsMsg.SourceUser() << std::endl
			 << "ddsMsg.DestinationUser : " << ddsMsg.DestinationUser() << std::endl;

			HeaderLogger::log(out, ddsMsg.fix_header());

			;out
			 << "ddsMsg.MDReqID : " << ddsMsg.MDReqID() << std::endl
			
		;out << "ddsMsg.c_NoMDEntries" << std::endl; 
	 	out << "{" << std::endl;

		for ( int tt = 0; tt < ddsMsg.c_NoMDEntries().size(); tt++)
		{

			 ;out << "ddsMsg.c_NoMDEntries()[" << tt << "].MDUpdateAction : " << ddsMsg.c_NoMDEntries()[tt].MDUpdateAction() << std::endl;
			 ;out << "ddsMsg.c_NoMDEntries()[" << tt << "].MDEntryType : " << ddsMsg.c_NoMDEntries()[tt].MDEntryType() << std::endl;
			 ;out << "ddsMsg.c_NoMDEntries()[" << tt << "].Symbol : " << ddsMsg.c_NoMDEntries()[tt].Symbol() << std::endl;
			 ;out << "ddsMsg.c_NoMDEntries()[" << tt << "].SecurityExchange : " << ddsMsg.c_NoMDEntries()[tt].SecurityExchange() << std::endl;
			 ;out << "ddsMsg.c_NoMDEntries()[" << tt << "].MDEntryPx : " << ddsMsg.c_NoMDEntries()[tt].MDEntryPx() << std::endl;
			 ;out << "ddsMsg.c_NoMDEntries()[" << tt << "].MDEntrySize : " << ddsMsg.c_NoMDEntries()[tt].MDEntrySize() << std::endl;
			 ;out << "ddsMsg.c_NoMDEntries()[" << tt << "].TimeInForce : " << ddsMsg.c_NoMDEntries()[tt].TimeInForce() << std::endl;
			 ;out << "ddsMsg.c_NoMDEntries()[" << tt << "].ExecInst : " << ddsMsg.c_NoMDEntries()[tt].ExecInst() << std::endl;
			 ;out << "ddsMsg.c_NoMDEntries()[" << tt << "].Text : " << ddsMsg.c_NoMDEntries()[tt].Text() << std::endl;
		};

		;out << "}" << std::endl;
;			out << "}";
		out << std::endl;};

};
