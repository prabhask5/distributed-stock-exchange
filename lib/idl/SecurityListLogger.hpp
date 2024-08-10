/* Don't modify, automatically generated file by QuickFIX2FastDDS.py*/
#pragma once
#include "HeaderLogger.hpp"
;

class SecurityListLogger
{
public:
	static void log(std::ostream &out, SecurityList::SecurityList &ddsMsg) __attribute__((visibility("default")))
	{
		;
		out << "Message : SecurityList { " << std::endl;
		HeaderLogger::log(out, ddsMsg.header());

		;
		out
			<< "ddsMsg.SecurityReqID : " << ddsMsg.SecurityReqID() << std::endl
			<< "ddsMsg.SecurityResponseID : " << ddsMsg.SecurityResponseID() << std::endl
			<< "ddsMsg.SecurityRequestResult : " << ddsMsg.SecurityRequestResult() << std::endl

			;
		out << "ddsMsg.c_NoRelatedSym" << std::endl;
		out << "{" << std::endl;

		for (int tt = 0; tt < ddsMsg.c_NoRelatedSym().size(); tt++)
		{

			;
			out << "ddsMsg.c_NoRelatedSym()[" << tt << "].Symbol : " << ddsMsg.c_NoRelatedSym()[tt].Symbol() << std::endl;
			;
			out << "ddsMsg.c_NoRelatedSym()[" << tt << "].SecurityExchange : " << ddsMsg.c_NoRelatedSym()[tt].SecurityExchange() << std::endl;
			;
			out << "ddsMsg.c_NoRelatedSym()[" << tt << "].Text : " << ddsMsg.c_NoRelatedSym()[tt].Text() << std::endl;
		};

		;
		out << "}" << std::endl;
		;
		out << "}";
		out << std::endl;
	};
};
