#pragma once

#include "FixMsgTranslatorService.hpp"
#include <DefaultDomainParticipantTypes.hpp>
#include <OrderMassCancelReport.hpp>

// This class takes in an order mass cancel report DDS message from the matching
// engine (in the case that an order mass cancel request has been accepted and
// executed, the matching engine sends a response message to the FIX gateway to
// confirm that the request has been completed, alongside additional data). This
// class takes in that DDS message, translates it into FIX, and surfaces the
// message to the end user.
class OrderMassCancelReportDataReaderListener : public DataReaderListener {
public:
  OrderMassCancelReportDataReaderListener(FixApplication &app);

  virtual void on_data_available(DataReader *reader) override;

private:
  FixMsgTranslatorService<
      DistributedStockExchange_OrderMassCancelReport::OrderMassCancelReport>
      m_translator_service;
};