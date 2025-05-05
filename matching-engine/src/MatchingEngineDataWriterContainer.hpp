#pragma once

#include <DefaultDomainParticipant.hpp>

struct DataWriterContainer {
    // Represents the execution report for an order sent by a user through the FIX gateway.
    // This report contains information about a order that is currently being executed by the matching engine.
    // This DW goes from matching engine -> FIX gateway.
    DataWriterPtr execReportDW;

    // Represents information related to canceling one order sent by a user through the FIX gatway.
    // This is basically the response to the order cancel FIX request.
    // This DW goes from matching engine -> FIX gateway.
    DataWriterPtr orderCancelRejectDW;

    // Represents information related to canceling multiple orders sent by a user through the FIX gatway.
    // This is basically the response to the order mass cancel FIX request.
    // This DW goes from matching engine -> FIX gateway.
    DataWriterPtr orderMassCancelRejectDW;

    // Represents information related to the incremental market data refresh sent from the matching engine to the data service.
    // An incremental market data refresh represents the changes in the market data during the duration from when the last refresh was sent to now.
    // These market data refreshes are periodically sent via a constant period duration (marketdata.data_pub_interval in config file).
    // This DW goes from matching engine -> data service.
    DataWriterPtr marketDataIncrementalRefreshDW;

    // Represents information related to the list of securities contained within this matching engine.
    // This is basically the response to a security list FIX request send by a user.
    // This DW goes from matching engine -> FIX gateway.
    DataWriterPtr securityListRequestDW;

    // Represents information related to the market data related to this matching engine.
    // Market data can contain a list of securities, prices, list of orders, etc.
    // This is basically the response to the market data FIX request.
    // This DW goes from matching engine -> FIX gateway.
    DataWriterPtr marketDataRequestDW;
};

using DataWriterContainerPtr = std::unique_ptr<DataWriterContainer>;