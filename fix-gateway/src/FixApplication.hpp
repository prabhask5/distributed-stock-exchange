#pragma once

#include <quickfix/Application.h>
#include <quickfix/MessageCracker.h>

class FixSocketConnection; // From FixSocketConnection.hpp.

class FixApplication : public FIX::Application, public FIX::MessageCracker {
public:
  bool insert_pending_logon_socket_connection(
      const FIX::Message &message, FixSocketConnection *socket_connection);

  void on_disconnect(const FIX::SessionID &sessionID,
                     FixSocketConnection *pSocketConnection);
};