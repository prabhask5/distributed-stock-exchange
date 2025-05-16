#include "AuthService.hpp"
#include "FixSocketConnection.hpp"
#include <DefaultDomainParticipantConstants.hpp>
#include <quickfix/Message.h>
#include <quickfix/Session.h>
#include <quickfix/SessionFactory.h>
#include <quickfix/fix44/Logout.h>

AuthService::AuthService(FixSessionSettingsPtr settings_ptr,
                         FixSessionFactoryPtr session_factory_ptr,
                         FixDictionaryPtr default_dictionary_ptr,
                         std::string comp_id)
    : m_settings_ptr(settings_ptr), m_session_factory_ptr(session_factory_ptr),
      m_default_dictionary_ptr(default_dictionary_ptr), m_comp_id(comp_id) {}

bool AuthService::ActiveSessionIDFromMessage(const FIX::Message &message,
                                             FIX::SessionID &session_id) {
  // First parse the sender id from the message.
  FIX::SenderCompID sender_id;
  message.getHeader().getField(sender_id);

  // Check whether the sender id that corresponds to the message is within the
  // active user map, this means that an active session id is tied to this
  // session id.
  auto aum_iter = m_active_user_map.find(sender_id.getValue());
  if (aum_iter == m_active_user_map.end())
    return false;

  session_id = aum_iter->second;
  return true;
}

void AuthService::insert_pending_connection(const std::string &connection_token,
                                            FixSocketConnection *sc_ptr) {
  // This is a lock guard, holds the lock for as long as this variable is in
  // scope.
  FIX::Locker lock(m_pending_session_mutex);

  LOG4CXX_INFO(logger, "Inserting pending connection : ["
                           << connection_token << "] Socket :["
                           << sc_ptr->get_socket() << "]");

  m_pending_logon_socket_connection_map[connection_token] = sc_ptr;
  sc_ptr->set_pending_connection_token(connection_token);
}

void AuthService::process_dds_logon(FIX::Message &message) {
  // Parse important pieces of info from the FIX DDS message.
  std::string connection_token = get_connection_token(
      message); // String that identifies the pending socket connection.
  std::string begin_string =
      message.getHeader().getField(FIX::FIELD::BeginString);
  std::string target_comp_id = message.getHeader().getField(
      FIX::FIELD::TargetCompID); // Represents the FIX gateway the message is
                                 // coming into.
  std::string target_sender_id = message.getHeader().getField(
      FIX::FIELD::TargetSubID); // Represents the sender id.
  std::string session_qualifier = message.getField(FIX::FIELD::RawData);

  LOG4CXX_INFO(logger, "Received Session ID for Logon : ["
                           << m_comp_id.c_str() << "] Session :["
                           << session_qualifier << "]");

  // Find the active session id using the target sender id.
  auto aum_iter = m_active_user_map.find(target_sender_id);
  if (aum_iter == m_active_user_map.end())
    return;

  FIX::SessionID active_session_id = aum_iter->second;

  // With the parsed session id, find the session.
  FIX::Session *active_session = nullptr;
  active_session = FIX::Session::lookupSession(active_session_id);
  if (active_session == nullptr)
    return;

  // Case 1: If this session is tied to a socket connection, and the comp id is
  // the same one as the one tied to this auth service instance, this session is
  // re-authenticating. This is a reconnection to the same gateway.
  if (active_session->isLoggedOn() && target_comp_id.compare(m_comp_id) == 0) {
    // Log out the old session to make way for the new one (only one session per
    // user as per FIX protocol).

    FIX44::Logout logout_message;

    FIX::Text logout_text("New session logged in with your credentials.");
    logout_message.set(logout_text);

    FIX::Session::sendToTarget(logout_message, active_session_id);
    m_active_user_map.erase(aum_iter);
  } else if (active_session->isLoggedOn()) {
    // Case 2: This session is logged on, so it's tied to a socket connection,
    // but the gateway doesn't match. We'll send a log out message saying that
    // the user is already logged in to another gateway.

    FIX44::Logout logout_message;

    FIX::Text logout_text(
        "New session logged in with your credentials from another gateway.");
    logout_message.set(logout_text);

    FIX::Session::sendToTarget(logout_message, active_session_id);
    m_active_user_map.erase(aum_iter);
    return; // End early, we don't want to log this session in.
  }

  // Now that we've processed already existing logged in sessions, we can safely
  // assume we log in all sessions past this point.

  // We can't log in sessions that were meant for another gateway.
  if (target_comp_id.compare(m_comp_id) != 0)
    return;

  // Create a new session id to mark the start of the new session, then create
  // that session using a helper method.
  FIX::SessionID new_session_id(begin_string, m_comp_id, target_sender_id,
                                session_qualifier);
  active_session =
      create_session_from_auth_message(new_session_id, message, "");

  // If active_session is null, something went wrong.
  if (active_session == nullptr)
    return;

  // Finally add this session to the active user map.
  m_active_user_map[target_sender_id] = new_session_id;
  login_session(active_session, connection_token);
}

void AuthService::process_dds_logout(std::string &connection_token,
                                     FIX::Message &logout_message) {
  // This is a lock guard, holds the lock for as long as this variable is in
  // scope.
  FIX::Locker lock(m_pending_session_mutex);

  // We reject a pending session, so we have to find that first.
  FixSocketConnection *pending_connection =
      m_pending_logon_socket_connection_map[connection_token];
  if (pending_connection == nullptr)
    return;

  std::string pending_login_str = pending_connection->get_pending_login();

  // Parse information in the pending login string into a pending login FIX
  // message to extract meaningful header information from.

  FIX::Message pending_login(pending_login_str);

  FIX::BeginString begin_string;
  FIX::SenderCompID cl_sender_comp_id;
  FIX::TargetCompID cl_target_comp_id;

  pending_login.getHeader().getField(begin_string);
  pending_login.getHeader().getField(cl_sender_comp_id);
  pending_login.getHeader().getField(cl_target_comp_id);

  // Build the logout message.
  FIX::MsgSeqNum msg_seq_num(1);
  FIX::SendingTime sending_time;
  FIX::TargetCompID target_comp_id(cl_sender_comp_id.getValue());
  FIX::SenderCompID sender_comp_id(cl_target_comp_id.getValue());

  logout_message.getHeader().setField(msg_seq_num);
  logout_message.getHeader().setField(target_comp_id);
  logout_message.getHeader().setField(sender_comp_id);
  logout_message.getHeader().setField(sending_time);

  LOG4CXX_INFO(logger, "Logout : Original SessionID Token [%s] : ["
                           << connection_token << "] Logout Message :["
                           << logout_message.toString() << "]");

  pending_connection->send(logout_message.toString());
  pending_connection->disconnect();
}

void AuthService::process_disconnect(const FIX::SessionID &session_id,
                                     FixSocketConnection *sc_ptr) {
  // This is a lock guard, holds the lock for as long as this variable is in
  // scope.
  FIX::Locker lock(m_pending_session_mutex);

  LOG4CXX_INFO(logger, "Disconnecting Pending Connection Token: ["
                           << sc_ptr->get_pending_connection_token());

  // Case 1: In the case that sc_ptr is a pending socket connection, we just
  // remove it from the map.
  auto psc_iter = m_pending_logon_socket_connection_map.find(
      sc_ptr->get_pending_connection_token());
  if (psc_iter != m_pending_logon_socket_connection_map.end()) {
    m_pending_logon_socket_connection_map.erase(psc_iter);
    return;
  }

  // Case 2: We disconnect an active session by logging it out, and destroying
  // it.
  LOG4CXX_INFO(logger, "Disconnecting : [" << session_id.toString());
  FIX::Session *session = FIX::Session::lookupSession(session_id);
  session->setResponder(NULL);
  session->logout("Disconnected");
  m_session_factory_ptr->destroy(session);
}

FIX::Session *AuthService::create_session_from_auth_message(
    const FIX::SessionID new_session_id, FIX::Message &message,
    const std::string &session_qualifier) { /* TODO */ }

std::string AuthService::get_connection_token(const FIX::Message &message) {
  FIX::RawData raw_data;
  message.getField(raw_data);

  return raw_data.getValue();
}

void AuthService::login_session(FIX::Session *session,
                                std::string &connection_token) {
  // This is a lock guard, holds the lock for as long as this variable is in
  // scope.
  FIX::Locker lock(m_pending_session_mutex);

  LOG4CXX_INFO(logger, "Login-in Session : [" << session->getSessionID()
                                              << "] Token :["
                                              << connection_token << "]");

  FixSocketConnection *pending_connection =
      m_pending_logon_socket_connection_map[connection_token];
  if (pending_connection == nullptr) {
    LOG4CXX_INFO(logger, "Responder not found : [" << session->getSessionID()
                                                   << "] Token :["
                                                   << connection_token << "]");
    return;
  }

  // Assign the session to the pending connection.
  pending_connection->set_session(session);
  session->setResponder(pending_connection);

  // This socket connection is not pending anymore (it's tied to an active
  // session), so we remove it from the pending map.
  m_pending_logon_socket_connection_map.erase(connection_token);

  // Send that pending logon FIX message!
  try {
    std::string pending_login_msg = pending_connection->get_pending_login();
    session->next(pending_login_msg, FIX::UtcTimeStamp());
  } catch (FIX::Exception &e) {
    LOG4CXX_ERROR(logger, "Exception in Logon : [" << session->getSessionID()
                                                   << "] Exception :["
                                                   << e.what() << "]");
  };
}