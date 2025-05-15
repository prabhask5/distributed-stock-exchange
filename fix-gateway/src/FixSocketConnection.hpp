#pragma once

#include "FixSocketConnectionTypes.hpp"
#include <quickfix/Mutex.h>
#include <quickfix/Parser.h>
#include <quickfix/Responder.h>

class FixSocketAcceptor; // From FixSocketAcceptor.hpp.

namespace FIX {
class SocketServer;
class SocketConnector;
class Session;
class SocketInitiator;
class SocketAcceptor;
class SocketMonitor;
} // namespace FIX

// Abstracts a FIX socket file descriptor.
class FixSocketConnection : public FIX::Responder {
public:
  FixSocketConnection(int socket_fd, SessionSet sessions,
                      FIX::SocketMonitor *monitor_ptr);

  virtual ~FixSocketConnection();

  // Getter const functions.

  int get_socket() const;

  FIX::Session *get_session() const;

  std::string get_pending_login();

  std::string get_pending_connection_token() const;

  // Returns whether that the currently active session (in m_session_ptr) is not
  // null, registered with FIX, and in the session set.
  bool is_valid_session() const;

  // Setter functions.

  void set_session(FIX::Session *session_ptr);

  void
  set_pending_connection_token(const std::string &pending_connection_token);

  // Action functions.

  // Reads raw data from the socket, assembles complete FIX messages, matches
  // them to sessions, and routes them accordingly: handling logon,
  // reconnections, and session assignment along the way. Returns the socket
  // connection is healthy after the execution of this function, if = false,
  // then the connection went wrong and should be dropped.
  bool read(FixSocketAcceptor &acceptor, FIX::SocketServer &server);

  // Add a message to the message queue, signal the socket monitor, and process
  // the messages in the queue. Returns whether successful (this is dumb but
  // required by the interface).
  bool send(const std::string &message);

  // In the case that the message queue is non-empty, we wait for this socket to
  // be ready, then we send the first message in the message queue via the
  // socket FD. There's a chance we don't send the complete message, so we
  // update the send_length accordingly. We only pop off the message if we
  // completely send it. Returns whether the message queue is empty after
  // execution.
  bool process_queue();

  // Signal to the socket monitor provided by quickfix that this socket is ready
  // to write. This occurs after the message queue has one member. This ONLY
  // happens when the message queue has one member since in the case of > 1,
  // signal() already occurred.
  void signal();

  // Unsignal to the socket monitor provided by quickfix. This occurs when the
  // message queue is empty, and we don't want the socket monitor to do more
  // work than necessary.
  void unsignal();

  // Drops this socket connection from the socket monitor.
  void disconnect();

private:
  // Simple helper function to read from socket using socket_recv().
  void read_from_socket();

  // Extracts a complete FIX message from the parser dependency. Returns whether
  // it was successful.
  bool read_message(std::string &message);

  // Continously calls read_message and routes to the active session. Catches an
  // error if the session is not logged in, then drops the socket from the
  // socket monitor if so.
  void read_messages(FIX::SocketMonitor &monitor);

private:
  // Socket connection metadata.

  // Represents the underlying socket file descriptor.
  int m_socket_fd;

  // Bitmap used to track the status of multiple file descriptors, used in
  // system calls like select() to wait for sockets to be ready for I/O.
  fd_set m_fd_set;

  // Socket connection high level state management.

  // Contains messages to be sent through the underlying socket FD.
  MessageQueue m_send_queue;

  // Tracks the state of FIX sessions associated with this socket connection
  // class.
  SessionSet m_sessions;

  // Represents a pointer to a currently active session that we're currently
  // reading/writing data from via the underlying socket FD.
  FIX::Session *m_session_ptr;

  // Socket message output state management.

  // Tracks the amount of the first message in the message queue already sent.
  // Useful in the case we can only send a portion of the message at a time.
  size_t m_sent_message_portion_length;

  // Socket message input state management.

  // Contains the message data read from the socket FD.
  char m_incoming_message_buffer[BUFSIZ];

  // Stores pending FIX message related to login.
  std::string m_pending_login;

  // Stores pending FIX message related to a session connection.
  std::string m_pending_connection_token;

  // Quickfix dependencies.

  // Dependency used to parse incoming byte streams into FIX messages.
  FIX::Parser m_parser;

  // Dependency used to monitor different sockets through quickfix.
  FIX::SocketMonitor *m_monitor_ptr;

  // Dependency used to make this socket connection thread-safe, provided by
  // quickfix.
  FIX::Mutex m_mutex;
};