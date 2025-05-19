#include "SQLiteQuery.hpp"
#include <DefaultDomainParticipantConstants.hpp>

SQLiteQuery::SQLiteQuery(const std::string &sql_query, bool in_read_mode,
                         const std::vector<std::string> &parameters)
    : m_prepared_statement(nullptr), m_raw_sql_query(sql_query),
      m_in_read_mode(in_read_mode), m_parameters(std::move(parameters)) {}

SQLiteQuery::~SQLiteQuery() {
  // This method destroyes a SQLite prepared statement.
  sqlite3_finalize(m_prepared_statement);
}

bool SQLiteQuery::execute(sqlite3 *db_connection_ptr) {
  // This function executes the raw sql query, and stores the prepared statement
  // in the m_prepared_statement pointer. A prepared statement pointer contains
  // the information from the execution.
  int code = sqlite3_prepare_v2(db_connection_ptr, m_raw_sql_query.c_str(), -1,
                                &m_prepared_statement, nullptr);
  if (code != SQLITE_OK) {
    handle_fatal(db_connection_ptr);
    return false;
  }

  for (int index = 0; index < m_parameters.size(); ++index) {
    // To take advantage of parameterization security, we feed in the parameters
    // into the prepared statement one by one.
    code = sqlite3_bind_text(m_prepared_statement, index + 1,
                             m_parameters[index].c_str(), -1, SQLITE_TRANSIENT);

    if (code != SQLITE_OK) {
      handle_fatal(db_connection_ptr);
      return false;
    }
  }

  int num_cols = sqlite3_column_count(m_prepared_statement);

  while ((code = sqlite3_step(m_prepared_statement)) == SQLITE_ROW) {
    std::vector<std::string> row(num_cols);
    for (int col = 0; col < num_cols; ++col) {
      const unsigned char *text =
          sqlite3_column_text(m_prepared_statement, col);
      row[col] = text ? reinterpret_cast<const char *>(text) : "";
    }

    m_output_table.emplace_back(row);
  }

  return code == SQLITE_DONE;
}

void SQLiteQuery::handle_fatal(sqlite3 *db_connection_ptr) {
  LOG4CXX_ERROR(logger, "FATAL ERROR: while compiling sql: "
                            << sqlite3_errmsg(db_connection_ptr));

  // This method destroyes a SQLite prepared statement.
  sqlite3_finalize(m_prepared_statement);
}

unsigned long SQLiteQuery::get_num_rows() const {
  return m_output_table.size();
}

std::string SQLiteQuery::get_value(int row, int col) const {
  return m_output_table[row][col];
}

bool SQLiteQuery::in_read_mode() const { return m_in_read_mode; }