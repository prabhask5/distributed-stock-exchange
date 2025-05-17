#include "SQLiteConnection.hpp"
#include "SQLiteQuery.hpp"
#include <DefaultDomainParticipantConstants.hpp>

SQLiteConnection::SQLiteConnection(
    const FIX::DatabaseConnectionID &database_connection_id)
    : m_database_connection_id(database_connection_id),
      m_db_connection_ptr(nullptr) {
  connect();
}

SQLiteConnection::~SQLiteConnection() {
  if (m_db_connection_ptr != nullptr)
    sqlite3_close(m_db_connection_ptr);
}

bool SQLiteConnection::execute(SQLiteQuery &query) {
  return query.execute(m_db_connection_ptr);
}

bool SQLiteConnection::is_connected() { return m_db_connection_ptr != nullptr; }

void SQLiteConnection::connect() {
  int code = sqlite3_open(m_database_connection_id.getDatabase().c_str(),
                          &m_db_connection_ptr);

  if (code != SQLITE_OK) {
    LOG4CXX_ERROR(logger, "Failed to fetch data: "
                              << sqlite3_errmsg(m_db_connection_ptr));
    sqlite3_close(m_db_connection_ptr);
  }
}