#pragma once

#include "AuthHelperTypes.hpp"
#include "FixGatewayDataWriterContainerTypes.hpp"
#include "FixSocketAcceptor.hpp"
#include <DefaultDomainParticipantTypes.hpp>
#include <quickfix/Application.h>
#include <quickfix/MessageCracker.h>

class FixSocketConnection; // From FixSocketConnection.hpp.

class FixApplication : public FIX::Application, public FIX::MessageCracker {
public:
  FixApplication(std::string data_service, std::string fix_gateway_name,
                 DataWriterContainerPtr data_writer_container_ptr);

  // Interface methods from FIX::Application.

  /// Notification of a session begin created.
  virtual void onCreate(const FIX::SessionID &session_id);

  /// Notification of a session successfully logging on.
  // NOTE: This happens AFTER all the pending logic and session creation.
  virtual void onLogon(const FIX::SessionID &session_id);

  /// Notification of a session logging off or disconnecting.
  virtual void onLogout(const FIX::SessionID &session_id);

  /// Notification of admin message being sent to target.
  virtual void toAdmin(FIX::Message &message, const FIX::SessionID &session_id);

  /// Notification of app message being sent to target.
  virtual void toApp(FIX::Message &message,
                     const FIX::SessionID &session_id) throw(FIX::DoNotSend);

  /// Notification of admin message being received from target.
  virtual void
  fromAdmin(const FIX::Message &message,
            const FIX::SessionID &session_id) throw(FIX::FieldNotFound,
                                                    FIX::IncorrectDataFormat,
                                                    FIX::IncorrectTagValue,
                                                    FIX::RejectLogon);

  /// Notification of app message being received from target.
  virtual void
  fromApp(const FIX::Message &message,
          const FIX::SessionID &session_id) throw(FIX::FieldNotFound,
                                                  FIX::IncorrectDataFormat,
                                                  FIX::IncorrectTagValue,
                                                  FIX::UnsupportedMessageType);

  // Interface methods from FIX::MessageCracker (to accept all the types of
  // messages that the FIX gateway handles).

  void onMessage(const FIX44::NewOrderSingle &message,
                 const FIX::SessionID &session_id);

  void onMessage(const FIX44::OrderCancelRequest &message,
                 const FIX::SessionID &session_id);

  void onMessage(const FIX44::OrderMassCancelRequest &message,
                 const FIX::SessionID &session_id);

  void onMessage(const FIX44::SecurityListRequest &message,
                 const FIX::SessionID &session_id);

  void onMessage(const FIX44::MarketDataRequest &message,
                 const FIX::SessionID &session_id);

  void onMessage(const FIX44::OrderMassStatusRequest &message,
                 const FIX::SessionID &session_id);

  // Getter const functions.

  const DataWriterContainerPtr &get_data_writer_container() const;

  const std::string &get_fix_gateway_name() const;

  // Setter functions.

  void set_auth_service_ptr(AuthHelperPtr auth_service_ptr);

  // Action functions to manage socket connections.

  // Handles a new pending logon socket connection by handling errors, using the
  // auth service to insert into the pending connection map, and sends an
  // authentication request to the data service. Returns whether successful.
  bool insert_pending_logon_socket_connection(
      const FIX::Message &logon_message,
      FixSocketConnection *socket_connection);

  // Forwards processing the disconnect to the auth service.
  void on_disconnect(const FIX::SessionID &session_id,
                     FixSocketConnection *socket_connection);

  // Make a connection token using the class state, and information from the
  // incoming logon message, and stores it in the raw data of the message.
  std::string make_connection_token(const FIX::Message &logon_message);

  // Methods to handle DDS -> FIX communication.

  // Publish a FIX message to one client session, with the session id found
  // through parsing the message.
  void publish_to_client(FIX::Message &message);

  // Broadcast the message to all client sessions.
  void broadcast_to_clients(FIX::Message &message);

  // Uses the Auth Service to process a logon message from the data service.
  // This means that the authentication was successfull, and a new FIX session
  // can be made.
  void process_dds_logon(FIX::Message &message);

  // Uses the Auth Service to process a logout message from the data service.
  // This means that authentication failed, so the FIX gateway must gracefully
  // handle that error.
  void process_dds_logout(const std::string &connection_token,
                          FIX::Message &message);

  // Generic helper function to take an incoming FIX message, translate it into
  // DDS, then send to the target service.
  template <class ADAPTER, class DATA, class LOGGER>
  void publish_to_dds(const FIX::Message &message,
                      const DataWriterPtr &data_writer_ptr,
                      const std::string &sender, const std::string &target,
                      const std::string &sender_sub_id);

private:
  // Dependencies.

  FixSocketAcceptor *m_acceptor_ptr;
  AuthHelperPtr m_auth_service_ptr;
  DataWriterContainerPtr m_data_writer_container_ptr;

  // Metadata.

  std::string m_data_service;
  std::string m_fix_gateway_name;

  // State management.

  // Used in making connection tokens.
  unsigned int m_connection_id;

  // Must lock whenever we interact with FIX sessions.
  FIX::Mutex m_session_mutex;
};