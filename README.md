# Distributed Stock Exchange

An implementation of a FIX protocol-based distributed stock exchange trading system that uses FastDDS for inter-node communication. This project creates three modular component systems to an overall high level distributed system, the matching engine, the FIX gateway, and the data service. This project makes it easy to configure your own custom distributed stock exchange by customizing the different amounts of each system to have in the overall distributed system, and with the input of your own custom data into the in-memory SQLite database.

## Components
- [FIX Gateway](https://github.com/prabhask5/distributed-stock-exchange/tree/main/fix-gateway): The portal to external users to take stock exchange requests through the FIX protocol. It is responsible for handling the frontend of authentication requests, creating new FIX sessions and socket connections via QuickFIX, accepting different kinds of requests related to the stock exchange from external users and converting them into DDS messages and sending them to the matching engine/data service via FastDDS for processing.
- [Matching Engine](https://github.com/prabhask5/distributed-stock-exchange/tree/main/matching-engine): Handles all of the order and order book logic within the distributed stock exchange. It is responsible for efficiently pairing buy and sell orders to facilitate trades. In the context of the distributed stock exchange project, the matching engine serves as the core mechanism that ensures fair and swift trade execution, maintaining market integrity and liquidity.
- [Data Service](https://github.com/prabhask5/distributed-stock-exchange/tree/main/data-service): Manages a connection to an in-memory SQLite database to manage persistent data from the matching engine, alongside additional hard coded information related to the overall metadata of the stock exchange, including authentication information, financial security information (i.e what lists of stocks are in the stock exchange, which users can access each stock).

## High Level Data Flow Throughout the Different Component Systems

|FIX Message|DDS Topic|Source|Destination|Content Description|
|-----------|---|------|--------------------------|-----------|
|Login Request|LOGON_TOPIC|fix_gateway|data_service|External FIX Login Request (New FIX Session)|
|Login Response|LOGON_TOPIC|data_service|fix_gateway|Successful Authentication of FIX Session by Data Service|
|Logout Response|LOGOUT_TOPIC|data_service|fix_gateway|Unsuccessful Authentication of FIX Session by Data Service|
|NewOrderSingle Request|NEW_ORDER_SINGLE_TOPIC|fix_gateway|matching_engine|External FIX New Order Single Request|
|OrderCancel Request|ORDER_CANCEL_REQUEST_TOPIC|fix_gateway|matching_engine|External FIX Order Cancel Request|
|OrderCancelReject Response|ORDER_CANCEL_REJECT_TOPIC|matching_engine|fix_gateway|If Order Can't Be Cancelled For Some Reason|
|OrderMassCancel Request|ORDER_MASS_CANCEL_REQUEST_TOPIC|fix_gateway|matching_engine|External FIX OrderMassCancel Request or FIX Client Logout/Disconnect|
|OrderMassCancel Response|ORDER_MASS_CANCEL_REPORT_TOPIC|matching_engine|fix_gateway|Results of Mass Cancel Request|
|SecurityList Request|SECURITY_LIST_REQUEST_TOPIC|fix_gateway|data_service|External FIX SecurityList Request|
|SecurityList Response|SECURITY_LIST_TOPIC|data_service|fix_gateway|Contains List of Financial Securities and Related Metadata for Stock Exchange|
|MarketData Request|MARKET_DATA_REQUEST_TOPIC|fix_gateway|data_service|External FIX Market Data Request|
|MarketDataSnapshotFullRefresh Response|MARKET_DATA_SNAPSHOT_FULL_REFRESH_TOPIC|data_service|fix_gateway|Current Market Data Snapshot (Top 5 levels)|
|OrderMassStatus Request|ORDER_MASS_STATUS_REQUEST_TOPIC|fix_gateway|data_service|External FIX Order Mass Status Request|
|ExecutionReport Response|EXECUTION_REPORT_TOPIC|data_service|fix_gateway|All Execution Reports for Orders Tied to Client (Response to Order Mass Status Request)|
|SecurityList Request|SECURITY_LIST_REQUEST_TOPIC|matching_engine|data_service|Security List Request for Matching Engine Initialization|
|SecurityList Response|SECURITY_LIST_TOPIC|data_service|matching_engine|Contains List of Financial Securities and Related Metadata for Stock Exchange|
|MarketData Request|MARKET_DATA_REQUEST_TOPIC|matching_engine|data_service|Market Data Request for Matching Engine Initialization|
|MarketDataSnapshotFullRefresh Response|MARKET_DATA_SNAPSHOT_FULL_REFRESH_TOPIC|data_service|matching_engine|Contains Opening Prices|
|ExecutionReport Data|EXECUTION_REPORT_TOPIC|matching_engine|data_service|Execution Report to Be Stored to Service Order Mass Status Requests|
|MarketDataIncrementalRefresh Data|MARKET_DATA_SNAPSHOT_FULL_REFRESH_TOPIC|matching_engine|fix_gateway|Incremental Market Data to Surface to FIX Client|
|MarketDataIncrementalRefresh Data|MARKET_DATA_SNAPSHOT_FULL_REFRESH_TOPIC|matching_engine|data_service|Incremental Refresh Data to Be Stored to Service Market Data Requests|

## Getting Started

To download and build all necessary dependencies and build the binaries for each of the components of the distributed stock exchange, use the provided script:

[build.sh](https://github.com/prabhask5/distributed-stock-exchange/blob/main/build.sh)

To initialize a new SQLite database file, use the provided script:

[create_database.sh](https://github.com/prabhask5/distributed-stock-exchange/blob/main/create_database.sh)

### Development

This project uses custom githooks to format .cpp/.hpp files, to change the folder used to run githook scripts from use the following commands:

```bash
# Install dependency clang-format
brew install clang-format

chmod +x .githooks/pre-commit
git config --local core.hooksPath .githooks/
```