# FIX Gateway

The FIX Gateway is the portal to external users to take stock exchange requests through the FIX protocol. It is responsible for handling the frontend of authentication requests, creating new FIX sessions and socket connections via QuickFIX, accepting different kinds of requests related to the stock exchange from external users and converting them into DDS messages and sending them to the matching engine/data service via FastDDS for processing.

## Features
- Manages FIX socket connections to external users to allow fast FIX communication via QuickFIX, also ties socket connections to FIX sessions.
- Accepts log on requests from external users and defers to data service to manage authentication workflow, managing pending socket connections.
- Accepts new order single, order cancel, order mass cancel, security list, market data, order mass status requests from external users, translates to DDS messages via a separate translator service thread, and routes to either the matching engine or data service for processing via FastDDS.
- Accepts responses from the matching engine and data service, translates into FIX, and surfaces to the end user.