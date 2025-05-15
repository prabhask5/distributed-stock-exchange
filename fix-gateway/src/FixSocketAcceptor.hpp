#pragma once

#include "FixSocketAcceptorTypes.hpp"
#include <quickfix/Acceptor.h>
#include <quickfix/SocketServer.h>

class FixApplication; // From FixApplication.hpp.

namespace FIX {
class MessageStoreFactory;
};

// Abstracts a socket acceptor, responsible for listening for incoming FIX
// connections over TCP and managing their lifecycles. Gateway to new FIX socket
// connections, FixSocketAcceptor manages FixSocketConnection's.
class FixSocketAcceptor : public FIX::Acceptor, FIX::SocketServer::Strategy {
  friend class FixSocketConnection;

public:
  FixSocketAcceptor(FixApplication &app,
                    FIX::MessageStoreFactory &msg_store_factory,
                    const FIX::SessionSettings &settings);

  FixSocketAcceptor(FixApplication &app,
                    FIX::MessageStoreFactory &msg_store_factory,
                    const FIX::SessionSettings &settings,
                    FIX::LogFactory &log_factory);

  virtual ~FixSocketAcceptor();

private:
  // Interface functions from FIX::Acceptor.

  // Hook to configure this socket acceptor using the settings param, called
  // before starting. This function validiates these settings, throwing a
  // ConfigError if something is invalid.
  void onConfigure(const FIX::SessionSettings &settings)
      EXCEPT(FIX::ConfigError);

  // Performs one-time setup actions (e.g., creating sockets, binding ports),
  // called after onConfigure.
  void onInitialize(const FIX::SessionSettings &settings)
      EXCEPT(FIX::RuntimeError);

  // Starts the socket acceptor.
  void onStart();

  // Polls the sockets for data, sleeping for the timeout. Called repeatedly.
  bool onPoll(double timeout);

  // Gracefully shuts down all sessions and closes sockets.
  void onStop();

  // Interface functions from FIX::SocketServer::Strategy.

  // Callback to handle a new TCP connection was accepted.
  void onConnect(FIX::SocketServer &server, int accepting_socket_fd,
                 int socket_fd);

  // Callback to handle a socket is ready for writing.
  void onWrite(FIX::SocketServer &server, int socket_fd);

  // Callback to handle a socket has data to be read.
  bool onData(FIX::SocketServer &server, int socket_fd);

  // Callback to handle a socket was closed by the peer.
  void onDisconnect(FIX::SocketServer &server, int socket_fd);

  // Callback to handle some socket error occurred.
  void onError(FIX::SocketServer &server);

  // Callback to handle no activity detected for a socket within a timeout
  // window.
  void onTimeout(FIX::SocketServer &server);

private:
  // Pointer to quickfix socket service dependency to manage/monitor sockets.
  FIX::SocketServer *m_server_ptr;

  // Maps each listening port to a group of sessions expected to connect on that
  // port.
  PortSessionSetMap m_port_session_set_map;

  // Active connection table: maps socket FDs to their corresponding
  // SocketConnection instances. Used to track who’s connected, route messages,
  // and clean up on disconnects.
  FDSocketConnectionMap m_socket_fd_connection_map;
};