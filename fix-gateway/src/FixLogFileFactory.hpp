#pragma once

#include <quickfix/Log.h>

class FixLogFileFactory : public FIX::LogFactory {
public:
  FixLogFileFactory(const FIX::SessionSettings &settings,
                    std::string fix_prefix);

  FixLogFileFactory(const std::string &path);

  FixLogFileFactory(const std::string &path, const std::string &backup_path);

  FIX::Log *create();

  FIX::Log *create(const FIX::SessionID &session_id);

  void destroy(FIX::Log *log_ptr);

private:
  std::string m_fix_log_path;
  std::string m_fix_log_backup_path;
  FIX::SessionSettings m_session_settings;
  FIX::Log *m_global_log_ptr;
  int m_global_log_count;
  std::string m_fix_prefix;
};