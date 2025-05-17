#include "FixApplication.hpp"
#include "AuthService.hpp"
#include "FixGatewayDataWriterContainer.hpp"
#include "FixSocketConnection.hpp"
#include <DefaultDomainParticipantConstants.hpp>
#include <Logon.hpp>
#include <LogonAdapter.hpp>
#include <LogonLogger.hpp>
#include <MarketDataRequest.hpp>
#include <MarketDataRequestAdapter.hpp>
#include <MarketDataRequestLogger.hpp>
#include <NewOrderSingle.hpp>
#include <NewOrderSingleAdapter.hpp>
#include <NewOrderSingleLogger.hpp>
#include <OrderCancelRequest.hpp>
#include <OrderCancelRequestAdapter.hpp>
#include <OrderCancelRequestLogger.hpp>
#include <OrderMassCancelRequest.hpp>
#include <OrderMassCancelRequestAdapter.hpp>
#include <OrderMassCancelRequestLogger.hpp>
#include <OrderMassStatusRequest.hpp>
#include <OrderMassStatusRequestAdapter.hpp>
#include <OrderMassStatusRequestLogger.hpp>
#include <SecurityListRequest.hpp>
#include <SecurityListRequestAdapter.hpp>
#include <SecurityListRequestLogger.hpp>
#include <quickfix/fix44/MarketDataRequest.h>
#include <quickfix/fix44/NewOrderSingle.h>
#include <quickfix/fix44/OrderCancelRequest.h>
#include <quickfix/fix44/OrderMassCancelRequest.h>
#include <quickfix/fix44/OrderMassStatusRequest.h>
#include <quickfix/fix44/SecurityListRequest.h>

FixApplication::FixApplication(std::string data_service,
                               std::string fix_gateway_name,
                               DataWriterContainerPtr data_writer_container_ptr)
    : m_data_service(data_service), m_fix_gateway_name(fix_gateway_name),
      m_data_writer_container_ptr(std::move(data_writer_container_ptr)),
      m_connection_id(0) {}

void FixApplication::onCreate(const FIX::SessionID &session_id) {
  LOG4CXX_INFO(logger, "Session Created :" << session_id.toString());
}

void FixApplication::onLogon(const FIX::SessionID &session_id) {
  LOG4CXX_INFO(logger, "Session Logon :" << session_id.toString());
}

void FixApplication::onLogout(const FIX::SessionID &session_id) {
  LOG4CXX_INFO(logger, "Session Logout :" << session_id.toString());

  // We're logging out and closing this session, so we need to cancel all orders
  // related to this session. Send a DDS order mass cancel request to the
  // matching engine via DDS.

  FIX::ClOrdID clOrdID;
  FIX::MassCancelRequestType request_type(
      FIX::MassCancelRequestType_CANCEL_ALL_ORDERS);
  FIX::TransactTime transactType;

  FIX44::OrderMassCancelRequest message(clOrdID, request_type, transactType);

  // Call the specific order mass cancel request function handler.
  onMessage(message, session_id);
}

void FixApplication::toAdmin(FIX::Message &message,
                             const FIX::SessionID &session_id) {}

void FixApplication::toApp(
    FIX::Message &message,
    const FIX::SessionID &session_id) throw(FIX::DoNotSend) {}

void FixApplication::fromAdmin(
    const FIX::Message &message,
    const FIX::SessionID &session_id) throw(FIX::FieldNotFound,
                                            FIX::IncorrectDataFormat,
                                            FIX::IncorrectTagValue,
                                            FIX::RejectLogon) {}

void FixApplication::fromApp(
    const FIX::Message &message,
    const FIX::SessionID &session_id) throw(FIX::FieldNotFound,
                                            FIX::IncorrectDataFormat,
                                            FIX::IncorrectTagValue,
                                            FIX::UnsupportedMessageType) {
  // This function accepts a generic message from the application. We use the
  // message cracker to route this message to the right FIX message handler
  // function.
  crack(message, session_id);
}

void FixApplication::onMessage(const FIX44::NewOrderSingle &message,
                               const FIX::SessionID &session_id) {
  publish_to_dds<NewOrderSingleAdapter,
                 DistributedStockExchange_NewOrderSingle::NewOrderSingle,
                 NewOrderSingleLogger>(
      message, m_data_writer_container_ptr->newOrderSingleDW,
      session_id.getSenderCompID(), "MATCHING_ENGINE",
      session_id.getTargetCompID());
}

void FixApplication::onMessage(const FIX44::OrderCancelRequest &message,
                               const FIX::SessionID &session_id) {
  publish_to_dds<
      OrderCancelRequestAdapter,
      DistributedStockExchange_OrderCancelRequest::OrderCancelRequest,
      OrderCancelRequestLogger>(
      message, m_data_writer_container_ptr->orderCancelRequestDW,
      session_id.getSenderCompID(), "MATCHING_ENGINE",
      session_id.getTargetCompID());
}

void FixApplication::onMessage(const FIX44::OrderMassCancelRequest &message,
                               const FIX::SessionID &session_id) {
  publish_to_dds<
      OrderMassCancelRequestAdapter,
      DistributedStockExchange_OrderMassCancelRequest::OrderMassCancelRequest,
      OrderMassCancelRequestLogger>(
      message, m_data_writer_container_ptr->orderMassCancelRequestDW,
      session_id.getSenderCompID(), "MATCHING_ENGINE",
      session_id.getTargetCompID());
}

void FixApplication::onMessage(const FIX44::SecurityListRequest &message,
                               const FIX::SessionID &session_id) {
  publish_to_dds<
      SecurityListRequestAdapter,
      DistributedStockExchange_SecurityListRequest::SecurityListRequest,
      SecurityListRequestLogger>(
      message, m_data_writer_container_ptr->securityListRequestDW,
      session_id.getSenderCompID(), "DATA_SERVICE",
      session_id.getTargetCompID());
}

void FixApplication::onMessage(const FIX44::MarketDataRequest &message,
                               const FIX::SessionID &session_id) {
  publish_to_dds<MarketDataRequestAdapter,
                 DistributedStockExchange_MarketDataRequest::MarketDataRequest,
                 MarketDataRequestLogger>(
      message, m_data_writer_container_ptr->marketDataRequestDW,
      session_id.getSenderCompID(), "DATA_SERVICE",
      session_id.getTargetCompID());
}

void FixApplication::onMessage(const FIX44::OrderMassStatusRequest &message,
                               const FIX::SessionID &session_id) {
  publish_to_dds<
      OrderMassStatusRequestAdapter,
      DistributedStockExchange_OrderMassStatusRequest::OrderMassStatusRequest,
      OrderMassStatusRequestLogger>(
      message, m_data_writer_container_ptr->orderMassStatusRequestDW,
      session_id.getSenderCompID(), "DATA_SERVICE",
      session_id.getTargetCompID());
}

const DataWriterContainerPtr &
FixApplication::get_data_writer_container() const {
  return m_data_writer_container_ptr;
}

const std::string &FixApplication::get_fix_gateway_name() const {
  return m_fix_gateway_name;
}

void FixApplication::set_auth_service_ptr(AuthServicePtr auth_service_ptr) {
  m_auth_service_ptr = std::move(auth_service_ptr);
}

bool FixApplication::insert_pending_logon_socket_connection(
    const FIX::Message &logon_message, FixSocketConnection *socket_connection) {
  FIX::TargetCompID client_target_comp_id;
  logon_message.getHeader().getField(client_target_comp_id);

  // If this logon message was not meant for this FIX gateway, we return early.
  if (client_target_comp_id.getValue().compare(m_fix_gateway_name) != 0) {
    socket_connection->disconnect();
    return false;
  }

  std::string connection_token = make_connection_token(logon_message);
  m_auth_service_ptr->insert_pending_connection(connection_token,
                                                socket_connection);

  FIX::SenderCompID client_sender_comp_id;
  logon_message.getHeader().getField(client_sender_comp_id);

  FIX::RawData raw_token_data(connection_token);
  const_cast<FIX::Message &>(logon_message).setField(raw_token_data);

  publish_to_dds<LogonAdapter, DistributedStockExchange_Logon::Logon,
                 LogonLogger>(
      logon_message, m_data_writer_container_ptr->logOnDW,
      client_target_comp_id, "DATA_SERVICE", client_sender_comp_id);

  return true;
}

void FixApplication::on_disconnect(const FIX::SessionID &session_id,
                                   FixSocketConnection *socket_connection) {
  // This is a lock guard, holds the lock for as long as this variable is in
  // scope.
  FIX::Locker lock(m_session_mutex);

  m_auth_service_ptr->process_disconnect(session_id, socket_connection);
}

std::string
FixApplication::make_connection_token(const FIX::Message &logon_message) {
  FIX::UtcTimeStamp time_stamp;
  FIX::SenderCompID client_sender_comp_id;
  FIX::TargetCompID client_target_comp_id;

  logon_message.getHeader().getField(client_sender_comp_id);
  logon_message.getHeader().getField(client_target_comp_id);

  std::stringstream connection_token_stream;
  connection_token_stream << client_sender_comp_id.getValue() << ":"
                          << client_target_comp_id.getValue() << ":"
                          << time_stamp.getTimeT() << ":" << m_connection_id++;

  return connection_token_stream.str();
}

void FixApplication::publish_to_client(FIX::Message &message) {
  // This is a lock guard, holds the lock for as long as this variable is in
  // scope.
  FIX::Locker lock(m_session_mutex);

  try {
    // Try to find the active_session_id parsed from the message.
    FIX::SessionID active_session_id;
    if (!AuthService::ActiveSessionIDFromMessage(message, active_session_id))
      return;

    FIX::Session::sendToTarget(message, active_session_id);
  } catch (FIX::FieldNotFound &field_not_found) {
    LOG4CXX_ERROR(logger, "Adapter message convertion : field "
                          "not found : Type - ["
                              << field_not_found.type << "%] : Field - ["
                              << field_not_found.field << "] - "
                              << message.toString() << "]");
  } catch (std::exception &e) {
    LOG4CXX_ERROR(logger, "unable to publish the message" << e.what());
  }
}

void FixApplication::broadcast_to_clients(FIX::Message &message) {
  // This is a lock guard, holds the lock for as long as this variable is in
  // scope.
  FIX::Locker lock(m_session_mutex);

  for (auto session_id : FIX::Session::getSessions())
    FIX::Session::sendToTarget(message, session_id);
}

void FixApplication::process_dds_logon(FIX::Message &message) {
  // This is a lock guard, holds the lock for as long as this variable is in
  // scope.
  FIX::Locker lock(m_session_mutex);

  m_auth_service_ptr->process_dds_logon(message);
}

void FixApplication::process_dds_logout(const std::string &connection_token,
                                        FIX::Message &message) {
  // This is a lock guard, holds the lock for as long as this variable is in
  // scope.
  FIX::Locker lock(m_session_mutex);

  m_auth_service_ptr->process_dds_logout(connection_token, message);
}

template <class ADAPTER, class DATA, class LOGGER>
void FixApplication::publish_to_dds(const FIX::Message &message,
                                    const DataWriterPtr &data_writer_ptr,
                                    const std::string &sender,
                                    const std::string &target,
                                    const std::string &sender_sub_id) {
  DATA dds_message;

  try {
    // Convert from FIX to DDS.
    ADAPTER::FIX2DDS(message, dds_message);
  } catch (FIX::FieldNotFound &field_not_found) {
    LOG4CXX_ERROR(logger,
                  "Adapter message convertion : field not found : Type - ["
                      << field_not_found.type << "] : Field - ["
                      << field_not_found.field << "]" << " - "
                      << message.toString());
    return;
  }

  // After converting the bulk of the information, add metdata to make sure the
  // DDS message goes to the right place.
  dds_message.Source(sender);
  dds_message.SourceUser(sender_sub_id);
  dds_message.Destination(target);
  dds_message.DestinationUser(target);

  std::stringstream ss;
  LOGGER::log(ss, dds_message);

  LOG4CXX_INFO(logger, "Publishing to DDS :" << ss.str());
  eprosima::fastdds::dds::ReturnCode_t code =
      data_writer_ptr->write(&dds_message);
  if (code != eprosima::fastdds::dds::RETCODE_OK)
    LOG4CXX_ERROR(logger, "Unable to publishing to DDS:" << ss.str());
}