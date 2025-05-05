#include "LogoutAdapter.hpp"
#include <ConvertUtils.h>

void LogoutAdapter::FIX2DDS(const FIX::Message &fixMsg,
                            DistributedStockExchange_Logout::Logout &ddsMsg) {
  HeaderAdapter::FIX2DDS(fixMsg.getHeader(), ddsMsg.fix_header());

  if (fixMsg.isSetField(FIX::FIELD::Text))
    ddsMsg.Text(
        ((FIX::Text)fixMsg.getField(FIX::FIELD::Text)).getString().c_str());
};

void LogoutAdapter::DDS2FIX(
    const DistributedStockExchange_Logout::Logout &ddsMsg,
    FIX::Message &fixMsg) {
  HeaderAdapter::DDS2FIX(ddsMsg.fix_header(), fixMsg.getHeader());

  convert_dds_string_to_fix(ddsMsg.Text(), FIX::FIELD::Text, fixMsg);
};
