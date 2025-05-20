# Distributed Stock Exchange

An implementation of a FIX protocol-based distributed stock exchange trading system that uses FastDDS for inter-node communication. This project creates three modular component systems to an overall high level distributed system, the matching engine, the FIX gateway, and the data service. This project makes it easy to configure your own custom distributed stock exchange by customizing the different amounts of each system to have in the overall distributed system, and with the input of your own custom data into the in-memory SQLite database.

## Components
- [FIX Gateway](https://github.com/prabhask5/distributed-stock-exchange/tree/main/fix-gateway): The portal to external users to take stock exchange requests through the FIX protocol. It is responsible for handling the frontend of authentication requests, creating new FIX sessions and socket connections via QuickFIX, accepting different kinds of requests related to the stock exchange from external users and converting them into DDS messages and sending them to the matching engine/data service via FastDDS for processing.
- [Matching Engine](https://github.com/prabhask5/distributed-stock-exchange/tree/main/matching-engine): Handles all of the order and order book logic within the distributed stock exchange. It is responsible for efficiently pairing buy and sell orders to facilitate trades. In the context of the distributed stock exchange project, the matching engine serves as the core mechanism that ensures fair and swift trade execution, maintaining market integrity and liquidity.
- [Data Service](https://github.com/prabhask5/distributed-stock-exchange/tree/main/data-service): Manages a connection to an in-memory SQLite database to manage persistent data from the matching engine, alongside additional hard coded information related to the overall metadata of the stock exchange, including authentication information, financial security information (i.e what lists of stocks are in the stock exchange, which users can access each stock).

## High Level Data Flow Throughout the Different Component Systems

|FIX Message|DDS Topic|Source|Destination|Content Description|
|-----------|---|------|--------------------------|-----------|
|Login(A)|LOGON_TOPIC|FIXGateway|DataService|Converted to IDL FIX Login|
|Login(A)|LOGON_TOPIC|DataService|FIXGateway|Successfully Authenticated FIX Session by a Data Service |
|Logout(5)|LOGOUT_TOPIC|DataService|FIXGateway|Unsuccessfully Authenticate FIX Session by Data Service|
|NewOrderSingle(D)|NEW_ORDER_SINGLE_TOPIC|FIXGateway|MatchingEngine|New order single|
|ExecutionReport(8)|EXECUTION_REPORT_TOPIC|MatchingEngine|FIXGateway|Execution Report to be sent to a client|
|ExecutionReport(8)|EXECUTION_REPORT_TOPIC|MatchingEngine|DataService|Execution Report to be store to service Order Mass Status Requests|
|OrderCancel Request(F)|ORDER_CANCEL_  REQUEST_TOPIC|FIXGateway|MatchingEngine|Converted to IDL Cancel Request from FIX Client|
|OrderCancel Reject(9)|ORDER_CANCEL_REJECT_ TOPIC|MatchingEngine|FIXGateway|IDL Cancel Reject, if order can't be cancelled|
|OrderCancelReplace Request(G)|ORDER_CANCEL_ REPLACE_REQUEST_TOPIC_NAME|FIXGateway|MatchingEngine|Converted to IDL Cancel Replace Request from FIX Client|
|MassCancel Request(q)|ORDER_MASS_CANCEL_ REQUEST_TOPIC|FIXGateway|MatchingEngines|Mass Cancel requested by FIX Client or FIX Client Logout/Disconnect|
|MassCancel Report(r)|ORDER_MASS_CANCEL_ REPORT_TOPIC|MatchingEngines|FIXGateway|Results of Mass Cancel Request|
|MarketData Request(V)|MARKET_DATA_REQUEST _TOPIC|FIXGateway|DataService|Request for the current state of Order Book(Top 5 levels)|
|MarketDataSnapshotFull Refresh(W)|MARKET_DATA_SNAPSHOT _FULL_REFRESH_TOPIC|DataService|FIXGateway|Current Market Data Snapshot(Top 5 levels)|
|MarketDataSnapshotFull Refresh(W)|MARKET_DATA_SNAPSHOT _FULL_REFRESH_TOPIC|Data Service|MatchingEngine|OpeningPrices|
|MarketDataIncremental Refresh(X)|MARKET_DATA_SNAPSHOT _FULL_REFRESH_TOPIC|MatchingEngine|FIXGateway|Incremental Market Data Request(Top 5 levels)|
|MarketDataIncremental Refresh(X)|MARKET_DATA_SNAPSHOT _FULL_REFRESH_TOPIC|MatchingEngine|FIXGateway|Incremental Market Data Request(Top 5 levels)|
|Security ListRequest(x)|SECURITY_LIST_REQUEST _TOPIC|FIXGateway|DataService|FIX Client initiated Security List Request|
|SecurityList(y)|SECURITY_LIST_TOPIC|DataService|FIXGateway|Security list reply for FIX Client Request|
|SecuritList Request(x)|SECURITY_LIST_REQUEST _TOPIC|MatchingEngine|DataService|Security List Request for Order Book setup|
|SecurityList(y)|SECURITY_LIST_TOPIC|DataService|MatchingEngine|Security List for OrderBook Setup|
|OrderMassStatus Request(AF)|ORDER_MASS_STATUS_ REQUEST_TOPIC|FIXGateway|DataService|Initiated by FIX Client Mass Status Request|

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