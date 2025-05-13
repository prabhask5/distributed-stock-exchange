# Matching Engine

This matching engine handles all of the order and order book logic within the distributed stock exchange. It is responsible for efficiently pairing buy and sell orders to facilitate trades. In the context of the distributed stock exchange project, the matching engine serves as the core mechanism that ensures fair and swift trade execution, maintaining market integrity and liquidity.

## Features
- Accepts requests from the FIX gateway about new orders, order cancels, order mass cancels, sent via the FastDDS protocol. These requests are the input data that the matching engine takes in.
    - Additionally sends reports to the FIX gateway (and the resulting user) about order execution, order cancel errors, and order mass cancel information.

- Accepts data from the data service about full refreshes of the market data (including market prices saved previously in the data service, used for market initialization), and security list data (used to determine which stock tickers are in this matching engine instance, used for market initialization).
    - Additionally sends requests to the data service about market data information (including previously saved market prices, and stock ticker symbols).

- Maintains a separate market data updates service thread to periodically send market data updates to the data service, used to get saved information on interrupt and matching engine restart.

- Maintains a dynamic record of all active buy and sell orders, updating it in real-time as new orders are received and trades are executed.
- Implements price time priority (time via FIFO) to determine the sequence in which orders are matched, ensuring fairness and transparency in trade execution.
- Handles multiple order types, including market orders, limit orders, stop orders, immediate or cancel orders, all or none orders, and fill or kill orders.
- Utilizes depth order books to maintain state over all price levels within each order book.