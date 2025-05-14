#include "FixLogFileFactory.hpp"
#include "FixLogFile.hpp"

FixLogFileFactory::FixLogFileFactory(const FIX::SessionSettings &settings,
                                     std::string fix_prefix)
    : m_session_settings(settings), m_fix_prefix(fix_prefix),
      m_global_log_ptr(nullptr), m_global_log_count(0) {}

FixLogFileFactory::FixLogFileFactory(const std::string &path)
    : m_fix_log_path(path), m_fix_log_backup_path(path),
      m_global_log_ptr(nullptr), m_global_log_count(0) {}

FixLogFileFactory::FixLogFileFactory(const std::string &path,
                                     const std::string &backup_path)
    : m_fix_log_path(path), m_fix_log_backup_path(backup_path),
      m_global_log_ptr(nullptr), m_global_log_count(0) {}

FIX::Log *FixLogFileFactory::create() {
  m_global_log_count++;
  if (m_global_log_count > 1)
    return m_global_log_ptr;

  try {
    if (m_fix_log_path.size())
      return new FixLogFile(m_fix_log_path, m_fix_prefix);

    std::string path;
    std::string backup_path;
    FIX::Dictionary settings = m_session_settings.get();

    path = settings.getString(FIX::FILE_LOG_PATH);
    backup_path = path;

    if (settings.has(FIX::FILE_LOG_BACKUP_PATH))
      backup_path = settings.getString(FIX::FILE_LOG_BACKUP_PATH);

    return m_global_log_ptr = new FixLogFile(path, backup_path, m_fix_prefix);
  } catch (FIX::ConfigError &) {
    m_global_log_count--;
    throw;
  }
}

FIX::Log *FixLogFileFactory::create(const FIX::SessionID &session_id) {
  if (m_fix_log_path.size() && m_fix_log_backup_path.size())
    return new FixLogFile(m_fix_log_path, m_fix_log_backup_path, session_id,
                          m_fix_prefix);
  if (m_fix_log_path.size())
    return new FixLogFile(m_fix_log_path, session_id, m_fix_prefix);

  std::string path;
  std::string backup_path;
  FIX::SessionID default_session("FIX.4.4", "DEFAULT", "DEFAULT");
  FIX::Dictionary settings = m_session_settings.get(default_session);

  path = settings.getString(FIX::FILE_LOG_PATH);
  backup_path = path;

  if (settings.has(FIX::FILE_LOG_BACKUP_PATH))
    backup_path = settings.getString(FIX::FILE_LOG_BACKUP_PATH);

  return new FixLogFile(path, backup_path, session_id, m_fix_prefix);
}

void FixLogFileFactory::destroy(FIX::Log *log_ptr) {
  if (log_ptr != m_global_log_ptr || --m_global_log_count == 0)
    delete log_ptr;
}