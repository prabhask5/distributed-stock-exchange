#!/bin/bash


fastddsgen Heartbeat.idl 
fastddsgen Logout.idl 
fastddsgen ExecutionReport.idl 
fastddsgen OrderCancelReject.idl 
fastddsgen Logon.idl 
fastddsgen NewOrderSingle.idl 
fastddsgen OrderCancelRequest.idl 
fastddsgen OrderCancelReplaceRequest.idl 
fastddsgen MarketDataRequest.idl 
fastddsgen MarketDataSnapshotFullRefresh.idl
fastddsgen MarketDataIncrementalRefresh.idl
fastddsgen BusinessMessageReject.idl 
fastddsgen OrderMassCancelRequest.idl 
fastddsgen OrderMassCancelReport.idl 
fastddsgen SecurityListRequest.idl 
fastddsgen SecurityList.idl
fastddsgen OrderMassStatusRequest.idl 
fastddsgen Header.idl 
