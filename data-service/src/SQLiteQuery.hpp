#pragma once

#include "SQLiteQueryTypes.hpp"
#include <sqlite3.h>
#include <string>

// This class encapsulates a SQL query sent to the SQLite database.
class SQLiteQuery {
public:
  SQLiteQuery(const std::string &sql_query,
              const std::vector<std::string> &parameters);

  ~SQLiteQuery();

  // Executes the prepared sql query using the provided db connection pointer.
  // Returns whether successful.
  bool execute(sqlite3 *db_connection_ptr);

  // Steps to handle a fatal error from SQLite, we close the database and
  // destroy the prepared statement.
  void handle_fatal(sqlite3 *db_connection_ptr);

  // Getter const methods.

  // Returns the number of rows in the output returned from the SQLite database
  // after executing the query.
  unsigned long get_num_rows() const;

  // Returns the element at (row, col) in the output returned from the SQLite
  // database after executing the query.
  std::string get_value(int row, int col) const;

private:
  sqlite3_stmt *m_prepared_statement;
  std::string m_raw_sql_query;
  std::vector<std::string> m_parameters;
  StringTable m_output_table;
};