#pragma once

#include <quickfix/DatabaseConnectionID.h>
#include <sqlite3.h>

class SQLiteQuery; // From SQLiteQuery.hpp.

// This class encapsulates a connection to a SQLite database.
class SQLiteConnection {
public:
  SQLiteConnection(const FIX::DatabaseConnectionID &database_connection_id);

  ~SQLiteConnection();

  // Encapsulates the execution of a SQLite query by passing in the database
  // pointer to it. Returns whether the query execution was successful.
  bool execute(SQLiteQuery &query);

  // Just returns whether the database connection pointer is not NULL.
  bool is_connected();

private:
  // Helper method called from the constructor to create a new database
  // connection, and set the database connection pointer.
  void connect();

private:
  sqlite3 *m_db_connection_ptr;
  FIX::DatabaseConnectionID m_database_connection_id;
};