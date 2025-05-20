#!/bin/sh

source ./env.sh

sqlite3 ${DSE_HOME}${EXCHANGE_BASEDIR}/data/dse.db <<EOF

PRAGMA foreign_keys = ON;

-- Stores user group information and passwords for authentication.
CREATE TABLE IF NOT EXISTS user_groups (
    name TEXT PRIMARY KEY,
    password TEXT,
    last_update_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Represents individual users and associates them with a user group.
CREATE TABLE IF NOT EXISTS users (
    username TEXT PRIMARY KEY,
    user_group TEXT,
    last_update_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY(user_group) REFERENCES user_groups(name)
);

-- Stores instruments (e.g., stocks) and their JSON metadata.
CREATE TABLE IF NOT EXISTS instruments (
    name TEXT PRIMARY KEY,
    properties JSON,
    last_update_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Historical last price data per instrument/market combination.
CREATE TABLE IF NOT EXISTS historical_prices (
    instrument_name TEXT,
    market_name TEXT,
    last_price TEXT,
    last_update_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY(instrument_name) REFERENCES instruments(name),
    FOREIGN KEY(market_name) REFERENCES markets(name),
    UNIQUE(instrument_name, market_name)
);

-- Lists available markets (e.g., NASDAQ, NYSE).
CREATE TABLE IF NOT EXISTS markets (
    name TEXT PRIMARY KEY,
    last_update_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Maps instruments to the markets they trade on (many-to-many).
CREATE TABLE IF NOT EXISTS instrument_markets (
    instrument_name TEXT,
    market_name TEXT,
    last_update_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY(instrument_name) REFERENCES instruments(name),
    FOREIGN KEY(market_name) REFERENCES markets(name),
    UNIQUE(instrument_name, market_name)
);

-- Maps user groups to the markets they can access (many-to-many).
CREATE TABLE IF NOT EXISTS user_group_markets (
    user_group TEXT,
    market_name TEXT,
    last_update_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY(user_group) REFERENCES user_groups(name),
    FOREIGN KEY(market_name) REFERENCES markets(name),
    UNIQUE(user_group, market_name)
);

-- Stores order execution reports.
CREATE TABLE IF NOT EXISTS execution_reports (
    user_group TEXT,
    order_id TEXT,
    begin_string TEXT,
    body_length INTEGER,
    msg_type TEXT,
    sender_comp_id TEXT,
    target_comp_id TEXT,
    msg_seq_num INTEGER,
    sender_sub_id TEXT,
    target_sub_id TEXT,
    sending_time INTEGER,
    orig_cl_ord_id TEXT,
    exec_id TEXT,
    exec_type TEXT,
    ord_status TEXT,
    ord_rej_reason INTEGER,
    symbol TEXT,
    security_exchange TEXT,
    side TEXT,
    order_qty INTEGER,
    ord_type TEXT,
    price REAL,
    stop_px REAL,
    time_in_force TEXT,
    exec_inst TEXT,
    last_qty INTEGER,
    last_px REAL,
    leaves_qty INTEGER,
    cum_qty INTEGER,
    avg_px REAL,
    transact_time INTEGER,
    text TEXT
);

EOF
