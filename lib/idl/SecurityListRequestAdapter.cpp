/* Don't modify, automatically generated file by QuickFIX2FastDDS.py*/
#include "SecurityListRequestAdapter.hpp"
#include <ConvertUtils.h>

void SecurityListRequestAdapter::FIX2DDS(const FIX::Message &fixMsg, SecurityListRequest::SecurityListRequest &ddsMsg)
{
	HeaderAdapter::FIX2DDS(fixMsg.getHeader(), ddsMsg.header());

	if (fixMsg.isSetField(FIX::FIELD::SecurityReqID))
		ddsMsg.SecurityReqID(((FIX::SecurityReqID)fixMsg.getField(FIX::FIELD::SecurityReqID)).getString().c_str());

	if (fixMsg.isSetField(FIX::FIELD::SecurityListRequestType))
		ddsMsg.SecurityListRequestType(FIELD_GET_REF(fixMsg, SecurityListRequestType));
	else
		ddsMsg.SecurityListRequestType(0);

	if (fixMsg.isSetField(FIX::FIELD::Symbol))
		ddsMsg.Symbol(((FIX::Symbol)fixMsg.getField(FIX::FIELD::Symbol)).getString().c_str());

	if (fixMsg.isSetField(FIX::FIELD::SecurityExchange))
		ddsMsg.SecurityExchange(((FIX::SecurityExchange)fixMsg.getField(FIX::FIELD::SecurityExchange)).getString().c_str());

	if (fixMsg.isSetField(FIX::FIELD::Text))
		ddsMsg.Text(((FIX::Text)fixMsg.getField(FIX::FIELD::Text)).getString().c_str());
};

void SecurityListRequestAdapter::DDS2FIX(const SecurityListRequest::SecurityListRequest &ddsMsg, FIX::Message &fixMsg)
{
	HeaderAdapter::DDS2FIX(ddsMsg.header(), fixMsg.getHeader());

	convert_dds_string_to_fix(ddsMsg.SecurityReqID(), FIX::FIELD::SecurityReqID, fixMsg);

	FIX::SecurityListRequestType fixSecurityListRequestType(ddsMsg.SecurityListRequestType());
	fixMsg.setField(fixSecurityListRequestType);

	convert_dds_string_to_fix(ddsMsg.Symbol(), FIX::FIELD::Symbol, fixMsg);

	convert_dds_string_to_fix(ddsMsg.SecurityExchange(), FIX::FIELD::SecurityExchange, fixMsg);

	convert_dds_string_to_fix(ddsMsg.Text(), FIX::FIELD::Text, fixMsg);
};
