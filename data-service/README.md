# Data Service

The data service manages a connection to an in-memory SQLite database to manage persistent data from the matching engine, alongside additional hard coded information related to the overall metadata of the stock exchange, including authentication information, financial security information (i.e what lists of stocks are in the stock exchange, which users can access each stock).

## Features
- Abstracts connections to the SQLite database and SQLite queries to easily fetch data.

- Implements a separate service thread for authentication (AuthService) to take in authentication requests from the FIX gateway, and process them one by one using authentication information from the database.
    - Additionally implemented a LRU cache layer to allow in memory fetching of authentication data to streamline the authentication workflow of frequently logged in users.

- Implements a separate service thread for instrument reference data (InstrumentReferenceDataService) to query all financial security information from the database and respond to security list data requests from both the FIX gateway and matching engine (for initialization).
- Implements a separate service thread for market data (MarketDataService) to query initial market incremental refresh data from the matching engine from the database and accept additional market data refreshes from the matching engine. This service thread responds to market data snapshot full refresh requests from the FIX gateway using this information. This service also persists all in-memory data gained from the matching engine while running to the SQLite database on shutdown.
- Implements a separate service thread for order mass status data (OrderMassStatusService) to accept execution reports from the matching engine (which represents order execution information) and store in memory. This service thread responds to order mass status requests from the FIX gateway using this information. This service also persists all in-memory data gained from the matching engine while running to the SQLite database on shutdown.