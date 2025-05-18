#pragma once

#include "AuthServiceTypes.hpp"
#include "DataServiceDataWriterContainerTypes.hpp"
#include "SQLiteConnectionTypes.hpp"
#include <LRUCache.hpp>
#include <quickfix/DatabaseConnectionID.h>
#include <thread>

// This service spins a separate thread to handle all incoming authentication
// requests to the data service. It manages a queue of incoming logon requests
// from the FIX gateway, interacts with the database to authenticate, and sends
// back logon/logout responses depending on authentication success.
class AuthService {
public:
  AuthService(const FIX::DatabaseConnectionID &database_connection_id,
              const DataWriterContainerPtr &data_writer_container_ptr,
              LogonQueuePtr logon_request_queue_ptr);

  ~AuthService();

  void service();

private:
  void authenticate(LogonPtr logon_ptr);

  std::string get_password(const std::string &username);

private:
  SQLiteConnectionPtr m_sqlite_connection_ptr;
  DataWriterContainerPtr m_data_writer_container_ptr;

  // This cache stores usernames and passwords from the database so we don't
  // have go all the way down to the DB layer for every auth request.
  AuthCache m_credentials_cache;

  LogonQueuePtr m_logon_request_queue_ptr;
  std::atomic<bool> m_is_running;
  std::thread m_service_thread;
};