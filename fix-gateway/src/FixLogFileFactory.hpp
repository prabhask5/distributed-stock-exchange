#pragma once

#include <quickfix/Log.h>

// This class manages instances of FixLogFiles.
class FixLogFileFactory : public FIX::LogFactory {
public:
  FixLogFileFactory(const FIX::SessionSettings &settings,
                    std::string fix_prefix);

  FixLogFileFactory(const std::string &path);

  FixLogFileFactory(const std::string &path, const std::string &backup_path);

  // Create a new global FIX log pointer.
  FIX::Log *create();

  // Create a new FIX log pointer for a specific FIX session.
  FIX::Log *create(const FIX::SessionID &session_id);

  // Destroy the passed in pointer, may be either the global pointer or a
  // specific session log pointer.
  void destroy(FIX::Log *log_ptr);

private:
  std::string m_fix_log_path;
  std::string m_fix_log_backup_path;
  FIX::SessionSettings m_session_settings;
  FIX::Log *m_global_log_ptr;
  int m_global_log_count;
  std::string m_fix_prefix;
};