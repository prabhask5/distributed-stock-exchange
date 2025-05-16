#pragma once

#include "AuthServiceTypes.hpp"
#include <quickfix/Mutex.h>

namespace FIX {
class Message;
class Session;
}; // namespace FIX

// FixApplication helper class to handle pending logon requests, manage active
// users/sessions, process logon/logout requests from outside sources.
class AuthService {
public:
  AuthService(FixSessionSettingsPtr settings_ptr,
              FixSessionFactoryPtr session_factory_ptr,
              FixDictionaryPtr default_dictionary_ptr, std::string sender_id);

  // Static methods.

  // Extracts the active session id from FIX message by parsing the sender id
  // from it and searching through the active user map- stored in the session_id
  // param. Returns whether the sender id it parsed from the message is within
  // the active user map, and successfully retrieved the active session id.
  static bool ActiveSessionIDFromMessage(const FIX::Message &message,
                                         FIX::SessionID &session_id);

  // Action methods.

  // Inserts a new pending socket connection to the map, also updates the socket
  // connection pending connection token.
  void insert_pending_connection(const std::string &connection_token,
                                 FixSocketConnection *sc_ptr);

  // This function handles an external logon request sent via DDS and translated
  // into a FIX message. The purpose of this function is to turn pending logon
  // requests from the socket connections to active sessions.
  void process_dds_logon(FIX::Message &message);

  // This function handles an external logout request sent via DDS and
  // translated into a FIX message. The purpose of this function is to reject
  // any authentication from pending sessions that have been rejected by the
  // data service. This is a mirror to process_dds_logon, but the fail version.
  // NOTE: This function and process_dds_logon don't deal with already active
  // sessions, they deal with pending logon requests that have been deferred to
  // the data service to authenticate them. The logout message sent is the
  // message in the parameter.
  void process_dds_logout(std::string &connection_token,
                          FIX::Message &logout_message);

  // Disconnect a socket connection, either pending or active.
  void process_disconnect(const FIX::SessionID &session_id,
                          FixSocketConnection *sc_ptr);

private:
  FIX::Session *
  create_session_from_auth_message(const FIX::SessionID new_session_id,
                                   FIX::Message &message,
                                   const std::string &session_qualifier);

  // Parse the connection token from a DDS FIX message raw data.
  std::string get_connection_token(const FIX::Message &message);

  // Helper function to tie an active session to a pending socket connection via
  // the connection token.
  void login_session(FIX::Session *session, std::string &connection_token);

private:
  // Maps the user sender id to the FIX session id, for all the current active
  // sessions.
  static UserMap m_active_user_map;

  // Dependencies.

  FixSessionSettingsPtr m_settings_ptr;
  FixSessionFactoryPtr m_session_factory_ptr;
  FixDictionaryPtr m_default_dictionary_ptr;

  // Metadata.

  // ID representing the FIX gatway destination requests come to, this is
  // different than the sender id, which represents each user who sends the
  // request.
  std::string m_comp_id;

  // State management.

  PendingLogonSocketConnectionMap m_pending_logon_socket_connection_map;
  FIX::Mutex m_pending_session_mutex;
};