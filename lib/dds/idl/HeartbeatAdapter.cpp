#include "HeartbeatAdapter.hpp"
#include <QuickFixDDSConvertUtils.hpp>

void HeartbeatAdapter::FIX2DDS(
    const FIX::Message &fixMsg,
    DistributedStockExchange_Heartbeat::Heartbeat &ddsMsg) {
  HeaderAdapter::FIX2DDS(fixMsg.getHeader(), ddsMsg.fix_header());
};

void HeartbeatAdapter::DDS2FIX(
    const DistributedStockExchange_Heartbeat::Heartbeat &ddsMsg,
    FIX::Message &fixMsg) {
  HeaderAdapter::DDS2FIX(ddsMsg.fix_header(), fixMsg.getHeader());
};
