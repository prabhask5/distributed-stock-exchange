#pragma once

#include "FixMsgTranslatorService.hpp"
#include <DefaultDomainParticipantTypes.hpp>
#include <OrderCancelReject.hpp>

// This class listens to an incoming order cancel reject DDS message from the
// matching engine (this represents an order cancel request not working for some
// reason). This class takes in that DDS message, translates it into FIX, and
// surfaces it to the end user.
class OrderCancelRejectDataReaderListener : public DataReaderListener {
public:
  OrderCancelRejectDataReaderListener(FixApplication &app);

  virtual void on_data_available(DataReader *reader) override;

private:
  FixMsgTranslatorService<
      DistributedStockExchange_OrderCancelReject::OrderCancelReject>
      m_translator_service;
};