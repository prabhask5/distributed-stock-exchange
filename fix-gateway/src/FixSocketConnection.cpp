#include "FixSocketConnection.hpp"
#include "FixApplication.hpp"
#include <quickfix/Session.h>
#include <quickfix/SocketAcceptor.h>
#include <quickfix/SocketMonitor.h>
#include <quickfix/SocketServer.h>

FixSocketConnection::FixSocketConnection(int socket_fd, SessionSet sessions,
                                         FIX::SocketMonitor *monitor_ptr)
    : m_socket_fd(socket_fd), m_sent_message_portion_length(0),
      m_sessions(sessions), m_session_ptr(nullptr), m_monitor_ptr(monitor_ptr) {
  FD_ZERO(&m_fd_set);
  FD_SET(m_socket_fd, &m_fd_set);
}

FixSocketConnection::~FixSocketConnection() {
  if (m_session_ptr)
    FIX::Session::unregisterSession(m_session_ptr->getSessionID());
}

int FixSocketConnection::get_socket() const { return m_socket_fd; }

FIX::Session *FixSocketConnection::get_session() const { return m_session_ptr; }

std::string FixSocketConnection::get_pending_login() { return m_pending_login; }

std::string FixSocketConnection::get_pending_connection_token() const {
  return m_pending_connection_token;
}

bool FixSocketConnection::is_valid_session() const {
  // Check 1: Does session pointer exist?
  if (m_session_ptr == nullptr)
    return false;

  FIX::SessionID session_id = m_session_ptr->getSessionID();

  // Check 2: Is the session ID registered?
  if (FIX::Session::isSessionRegistered(session_id))
    return false;

  // Check 3: Is the session within the session set?
  if (m_sessions.find(session_id) == m_sessions.end())
    return false;

  return true;
}

void FixSocketConnection::set_session(FIX::Session *session_ptr) {
  m_session_ptr = session_ptr;
}

void FixSocketConnection::set_pending_connection_token(
    const std::string &pending_connection_token) {
  m_pending_connection_token = pending_connection_token;
}

bool FixSocketConnection::read(FIX::SocketAcceptor &acceptor,
                               FIX::SocketServer &server) {
  try {
    // If we have an active session already, just read from the socket and
    // process FIX messages as usual.
    if (m_session_ptr) {
      read_from_socket();
      read_messages(server.getMonitor());
      return true;
    }

    // If we're here, then we are processing the first message from a
    // prospective active session.

    std::string msg_container;

    // While we've run out of internal input bytes to parse into FIX messages,
    // we poll the socket for new data using the select syscall. If there's an
    // error we immediately return, but if there's data, we read from socket and
    // parse again using the loop.
    while (!read_message(msg_container)) {
      // This timeout struct means that the select syscall waits for up to 1
      // second to listen for incoming data.
      struct timeval timeout = {1, 0};
      fd_set readset = m_fd_set;

      if (select(1 + m_socket_fd, &readset, 0, 0, &timeout) <= 0)
        return false;
      read_from_socket();
    }

    // Now, we have the FIX message in msg_container, we now lookup that session
    // using quickfix helper methods. At this point in time, m_session_ptr is
    // pointing to the session corresponding with the incoming message.
    m_session_ptr = FIX::Session::lookupSession(msg_container, true);

    // If something went wrong with look up session, we drop the socket and
    // throw can invalid message error.
    if (m_session_ptr == nullptr)
      throw FIX::InvalidMessage();

    // In this block, we handle authentication logic with the data service. This
    // can occur if the session is completely new (not authenticated on any
    // socket connection), or was active on another session and would like to
    // re-authenticate on this socket connection. This condition checks whether
    // the session is not allowed to use this session or it's logged in
    // somewhere else- we must handle authentication in this case. NOTE: If
    // we're at this block AND m_session_ptr->isLoggedOn() = true, this MUST
    // mean that the session is used by ANOTHER socket. If it was just used by
    // this socket, we wouldn't be here.
    if (!is_valid_session() || m_session_ptr->isLoggedOn()) {
      // First extract the header from the msg, if something goes wrong, we
      // return early.
      FIX::Message fix_message;
      if (!fix_message.setStringHeader(msg_container))
        return false;

      try {
        const FIX::MsgType &msg_type =
            FIELD_GET_REF(fix_message.getHeader(), MsgType);

        if (msg_type == FIX::MsgType_Logon) {
          // In the case that this message is a log on request, do not add the
          // active session yet (authentication might fail), and insert a
          // pending logon socket connection.

          m_session_ptr = nullptr;
          m_pending_login = msg_container;

          FixApplication &app =
              dynamic_cast<FixApplication &>(acceptor.getApplication());
          return app.insert_pending_logon_socket_connection(msg_container,
                                                            this);
        }

      } catch (FIX::FieldNotFound &) {
        return false;
      }

      // In the case that the message was not a log on request, we cannot use
      // this message in this current socket connection (it's invalidated), BUT
      // there might be a way to salvage this if the acceptor can find and
      // validate it, so we don't return false immediately.

      // Log this error.
      if (acceptor.getLog()) {
        acceptor.getLog()->onEvent("Session not found for incoming message: " +
                                   msg_container);
        acceptor.getLog()->onIncoming(msg_container);
      }
    }

    // TODO: There might be a security hole here allowing recently disconnected
    // clients to reconnect with a non-logon message and process. This is a
    // violation of the FIX protocol.

    // Try to see if we can find this session with the acceptor.
    m_session_ptr = acceptor.getSession(msg_container, *this);

    // If we're not successful, now this is unsalvagable- we'll throw an invalid
    // message error.
    if (m_session_ptr == nullptr)
      throw FIX::InvalidMessage();

    m_session_ptr->next(msg_container, FIX::UtcTimeStamp());
    FIX::Session::registerSession(m_session_ptr->getSessionID());
    return true;
  } catch (FIX::SocketRecvFailed &e) {
    if (m_session_ptr)
      m_session_ptr->getLog()->onEvent(e.what());
  } catch (FIX::InvalidMessage &) {
  }

  server.getMonitor().drop(m_socket_fd);
  return false;
}

bool FixSocketConnection::send(const std::string &message) {
  // This is a lock guard, holds the lock for as long as this variable is in
  // scope.
  FIX::Locker locker(m_mutex);

  m_send_queue.push_back(message);

  process_queue();
  signal();

  return true;
}

bool FixSocketConnection::process_queue() {
  // This is a lock guard, holds the lock for as long as this variable is in
  // scope.
  FIX::Locker locker(m_mutex);

  if (m_send_queue.empty())
    return false;

  // NOTE: this timeout struct tells select not to block and only check
  // immediately if the socket is ready to write.
  struct timeval timeout = {0, 0};
  fd_set writeset = m_fd_set;

  // This is a system call to check whether the socket_fd is ready to write (> 0
  // if so). NOTE: 1 + m_socket_fd represents that the select syscall checks all
  // socket FDs <= m_socket_fd (it's non-inclusive).
  if (select(1 + m_socket_fd, 0, &writeset, 0, &timeout) <= 0)
    return false;

  const std::string &latest_msg = m_send_queue.front();

  // NOTE: ssize_t is size_t that can be signed.
  ssize_t bytes_sent = FIX::socket_send(
      m_socket_fd, latest_msg.c_str() + m_sent_message_portion_length,
      latest_msg.length() - m_sent_message_portion_length);
  if (bytes_sent > 0)
    m_sent_message_portion_length += bytes_sent;

  // If we've sent all of the latest message, only now we can pop it.
  if (m_sent_message_portion_length == latest_msg.length()) {
    m_sent_message_portion_length = 0;
    m_send_queue.pop_front();
  }

  // We return whether the send queue is now empty.
  return !m_send_queue.size();
}

void FixSocketConnection::signal() {
  // This is a lock guard, holds the lock for as long as this variable is in
  // scope.
  FIX::Locker locker(m_mutex);

  if (m_send_queue.size() == 1)
    m_monitor_ptr->signal(m_socket_fd);
}

void FixSocketConnection::unsignal() {
  // This is a lock guard, holds the lock for as long as this variable is in
  // scope.
  FIX::Locker locker(m_mutex);

  if (m_send_queue.size() == 1)
    m_monitor_ptr->unsignal(m_socket_fd);
}

void FixSocketConnection::disconnect() {
  if (m_monitor_ptr)
    m_monitor_ptr->drop(m_socket_fd);
}

void FixSocketConnection::read_from_socket() {
  ssize_t bytes_sent = FIX::socket_recv(m_socket_fd, m_incoming_message_buffer,
                                        sizeof(m_incoming_message_buffer));

  // This is only negative if an unexpected error happened.
  if (bytes_sent <= 0)
    throw FIX::SocketRecvFailed(bytes_sent);

  m_parser.addToStream(m_incoming_message_buffer, bytes_sent);
}

bool FixSocketConnection::read_message(std::string &msg_container) {
  try {
    return m_parser.readFixMessage(msg_container);
  } catch (FIX::MessageParseError &) {
    return false;
  }

  return true;
}

void FixSocketConnection::read_messages(FIX::SocketMonitor &monitor) {
  if (!m_session_ptr)
    return;

  std::string msg_container;

  // Keep reading FIX messages from the parser.
  while (read_message(msg_container)) {
    try {
      // Sends the FIX message to the session for processing.
      m_session_ptr->next(msg_container, FIX::UtcTimeStamp());
    } catch (FIX::InvalidMessage &) {
      // Security: if the session isn't logged on, we drop it.
      if (!m_session_ptr->isLoggedOn())
        monitor.drop(m_socket_fd);
    }
  }
}
