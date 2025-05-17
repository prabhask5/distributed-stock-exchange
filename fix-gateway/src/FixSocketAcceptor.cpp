#include "FixSocketAcceptor.hpp"
#include "FixApplication.hpp"
#include "FixSocketAcceptorConstants.hpp"
#include "FixSocketConnection.hpp"

FixSocketAcceptor::FixSocketAcceptor(
    FixApplication &app, FIX::MessageStoreFactory &msg_store_factory,
    const FIX::SessionSettings &settings)
    : Acceptor(app, msg_store_factory, settings), m_server_ptr(nullptr) {}

FixSocketAcceptor::FixSocketAcceptor(
    FixApplication &app, FIX::MessageStoreFactory &msg_store_factory,
    const FIX::SessionSettings &settings, FIX::LogFactory &log_factory)
    : Acceptor(app, msg_store_factory, settings, log_factory),
      m_server_ptr(nullptr) {}

FixSocketAcceptor::~FixSocketAcceptor() {
  // Delete all the socket connections.
  for (auto iter = m_socket_fd_connection_map.begin();
       iter != m_socket_fd_connection_map.end(); ++iter)
    delete iter->second;

  // Delete the server pointer if it still exists unexpectedly (not deleted in
  // onStart).
  if (m_server_ptr != nullptr) {
    m_server_ptr->close();
    delete m_server_ptr;
  }
}

void FixSocketAcceptor::onConfigure(const FIX::SessionSettings &settings) throw(
    FIX::ConfigError) {
  SessionSet sessions = settings.getSessions();
  for (auto iter = sessions.begin(); iter != sessions.end(); ++iter) {
    const FIX::Dictionary &session_settings = settings.get(*iter);

    // We're basically just seeing if the following expected settings on the
    // settings for the socket exist, we're not modifying any state here, just
    // throwing a ConfigError if any of these settings are not available/not the
    // expected type.
    session_settings.getInt(FIX::SOCKET_ACCEPT_PORT);
    if (session_settings.has(FIX::SOCKET_REUSE_ADDRESS))
      session_settings.getBool(FIX::SOCKET_REUSE_ADDRESS);
    if (session_settings.has(FIX::SOCKET_NODELAY))
      session_settings.getBool(FIX::SOCKET_NODELAY);
  }
}

void FixSocketAcceptor::onInitialize(
    const FIX::SessionSettings &settings) throw(FIX::RuntimeError) {
  short port = 0;
  try {
    m_server_ptr = new FIX::SocketServer(SOCKET_SERVER_TIMEOUT);

    SessionSet sessions = settings.getSessions();
    for (auto iter = sessions.begin(); iter != sessions.end(); ++iter) {
      const FIX::SessionID &session = *iter;
      const FIX::Dictionary &session_settings = settings.get(session);

      port = (short)session_settings.getInt(FIX::SOCKET_ACCEPT_PORT);
      const bool reuse_address =
          session_settings.has(FIX::SOCKET_REUSE_ADDRESS)
              ? session_settings.getBool(FIX::SOCKET_REUSE_ADDRESS)
              : true;
      const bool no_delay = session_settings.has(FIX::SOCKET_NODELAY)
                                ? session_settings.getBool(FIX::SOCKET_NODELAY)
                                : false;
      const int send_buf_size =
          session_settings.has(FIX::SOCKET_SEND_BUFFER_SIZE)
              ? session_settings.getInt(FIX::SOCKET_SEND_BUFFER_SIZE)
              : 0;
      const int rcv_buffer_size =
          session_settings.has(FIX::SOCKET_RECEIVE_BUFFER_SIZE)
              ? session_settings.getInt(FIX::SOCKET_RECEIVE_BUFFER_SIZE)
              : 0;

      // Add this already existing session to the port to sessions map and the
      // socket server.
      m_port_session_set_map[port].insert(session);
      m_server_ptr->add(port, reuse_address, no_delay, send_buf_size,
                        rcv_buffer_size);
    }
  } catch (FIX::SocketException &e) {
    throw FIX::RuntimeError("Unable to create, bind, or listen to port " +
                            std::to_string((unsigned short)port) + " (" +
                            e.what() + ")");
  }
}

void FixSocketAcceptor::onStart() {
  // This is the main event loop for the socket acceptor. While this acceptor
  // isn't stopped with onStop, the socket server will block this acceptor until
  // an event happens. On an event, the related callback hook within this class
  // will execute and handle the logic.
  while (!isStopped() && m_server_ptr != nullptr &&
         m_server_ptr->block(*this)) {
  }

  // If we get here, this means that the acceptor has stopped, and we can do
  // some cleanup (mainly of the socket server).

  if (m_server_ptr == nullptr)
    return;

  // We allow 5 seconds to wait for all the sessions and their related socket
  // connections to exist gracefully before manually shutting everything down.

  time_t start = 0;
  time_t now = 0;

  ::time(&start);        // Timer start.
  while (isLoggedOn()) { // Checks whether any session is logged in (attached to
                         // a socket connection).
    m_server_ptr->block(*this);
    if (::time(&now) - GRACEFUL_STOP_WAIT_TIME >= start)
      break;
  }

  // We've waited enough, time to shut down the server.
  m_server_ptr->close();
  delete m_server_ptr;
  m_server_ptr =
      nullptr; // Don't want dangling pointers since the class is still active.
}

bool FixSocketAcceptor::onPoll(double timeout) {
  if (m_server_ptr == nullptr)
    return;

  time_t start = 0;
  time_t now = 0;

  ::time(&start);

  // Can't poll if there are no active sessions.
  // NOTE: We don't do this on stopped to allow for polls to happen while the
  // sessions are gracefully exiting.
  if (!isLoggedOn())
    return false;

  // In the crazy case that the duration of isLoggedOn takes more than the
  // graceful timeout time, we'll exit early here too.
  if (::time(&now) - GRACEFUL_STOP_WAIT_TIME >= start)
    return false;

  m_server_ptr->block(*this, true, timeout);
  return true;
}

void FixSocketAcceptor::onStop() {}

void FixSocketAcceptor::onConnect(FIX::SocketServer &server,
                                  int accepting_socket_fd, int socket_fd) {
  // Can't connect to an invalid receiving socket.
  if (!FIX::socket_isValid(socket_fd))
    return;

  // Can't have duplicate sockets.
  if (m_socket_fd_connection_map.find(socket_fd) !=
      m_socket_fd_connection_map.end())
    return;

  // Add the new socket connection to the connection map.
  int port = server.socketToPort(accepting_socket_fd);
  m_socket_fd_connection_map[socket_fd] = new FixSocketConnection(
      socket_fd, m_port_session_set_map[port], &server.getMonitor());

  // Log success.
  std::stringstream stream;
  stream << "Accepted connection from " << FIX::socket_peername(socket_fd)
         << " on port " << port;

  if (getLog())
    getLog()->onEvent(stream.str());
}

void FixSocketAcceptor::onWrite(FIX::SocketServer &server, int socket_fd) {
  auto sc_iter = m_socket_fd_connection_map.find(socket_fd);

  // Can't write on a socket connection that doesn't exist.
  if (sc_iter == m_socket_fd_connection_map.end())
    return;

  // onWrite assumes that writing on the socket FD already happened (i.e there's
  // messages in the send_queue). This means that one call to process_queue
  // already happened (and the signal call), so we call it again to make sure
  // we're done writing (this is the layer above the socket connection), and if
  // so call unsignal() in the socket connection.
  FixSocketConnection *sc_ptr = sc_iter->second;
  if (sc_ptr->process_queue())
    sc_ptr->unsignal();
}

bool FixSocketAcceptor::onData(FIX::SocketServer &server, int socket_fd) {
  auto sc_iter = m_socket_fd_connection_map.find(socket_fd);

  // Can't read on a socket connection that doesn't exist.
  if (sc_iter == m_socket_fd_connection_map.end())
    return false;

  FixSocketConnection *sc_ptr = sc_iter->second;
  return sc_ptr->read(*this, server);
}

void FixSocketAcceptor::onDisconnect(FIX::SocketServer &server, int socket_fd) {
  auto sc_iter = m_socket_fd_connection_map.find(socket_fd);

  // Can't disconnect a socket connection that doesn't exist.
  if (sc_iter == m_socket_fd_connection_map.end())
    return;

  FixSocketConnection *sc_ptr = sc_iter->second;

  FIX::Session *session_ptr = sc_ptr->get_session();
  if (session_ptr != nullptr) {
    session_ptr->disconnect();

    FixApplication &app = dynamic_cast<FixApplication &>(getApplication());
    app.on_disconnect(session_ptr->getSessionID(), sc_ptr);
  }

  // TODO: might leak?
  delete sc_ptr;
  m_socket_fd_connection_map.erase(socket_fd);
}

void FixSocketAcceptor::onError(FIX::SocketServer &server) {}

void FixSocketAcceptor::onTimeout(FIX::SocketServer &server) {}