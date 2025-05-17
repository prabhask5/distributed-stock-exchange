#include "SQLiteQuery.hpp"
#include <DefaultDomainParticipantConstants.hpp>

SQLiteQuery::SQLiteQuery(const std::string &sql_query)
    : m_prepared_statement(nullptr), m_raw_sql_query(sql_query) {}

SQLiteQuery::~SQLiteQuery() {
  // This method destroyes a SQLite prepared statement.
  sqlite3_finalize(m_prepared_statement);
}

bool SQLiteQuery::execute(sqlite3 *db_connection_ptr) {
  // This function executes the raw sql query, and stores the prepared statement
  // in the m_prepared_statement pointer. A prepared statement pointer contains
  // the information from the execution.
  int code = sqlite3_prepare_v2(db_connection_ptr, m_raw_sql_query.c_str(), -1,
                                &m_prepared_statement, NULL);
  if (code != SQLITE_OK) {
    LOG4CXX_ERROR(logger, "FATAL ERROR: while compiling sql: "
                              << sqlite3_errmsg(db_connection_ptr));

    // Close the database, the only reason something went wrong is that there's
    // a problem with the database.
    sqlite3_close(db_connection_ptr);

    // This method destroyes a SQLite prepared statement.
    sqlite3_finalize(m_prepared_statement);
    return false;
  }

  int num_cols = sqlite3_column_count(m_prepared_statement);

  while ((code = sqlite3_step(m_prepared_statement)) == SQLITE_ROW) {
    std::vector<std::string> row(num_cols);
    for (int col = 0; col < num_cols; ++col)
      row[col] = reinterpret_cast<const char *>(
          sqlite3_column_text(m_prepared_statement, col));
    m_output_table.emplace_back(row);
  }

  return code = SQLITE_OK;
}

unsigned long SQLiteQuery::get_num_rows() const {
  return m_output_table.size();
}

std::string SQLiteQuery::get_value(int row, int col) const {
  return m_output_table[row][col];
}