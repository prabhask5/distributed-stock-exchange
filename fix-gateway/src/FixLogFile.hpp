#pragma once

#include <fstream>
#include <quickfix/Log.h>

// This is a file representation of the fix logs. This is useful to store
// messages and events from FIX for debugging.
class FixLogFile : public FIX::Log {
public:
  FixLogFile(const std::string &path, const std::string &prefix);

  FixLogFile(const std::string &path, const std::string &backup_path,
             const std::string &prefix);

  FixLogFile(const std::string &path, const FIX::SessionID &session_id,
             const std::string &prefix);

  FixLogFile(const std::string &path, const std::string &backup_path,
             const FIX::SessionID &session_id, const std::string &prefix);

  virtual ~FixLogFile();

  // Clear the messages and events files, and refresh the messages and events
  // file streams.
  void clear();

  // Closes the messages and events file streams, and writes the contents of the
  // messages and events files into backup files by renaming them via the backup
  // path. We then close the original messages and events files.
  void backup();

  // Writes a value to the messages file stream, alongside the current
  // timestamp.
  void onIncoming(const std::string &value);

  // Writes a value to the messages file stream, alongside the current
  // timestamp.
  void onOutgoing(const std::string &value);

  // Writes a value ot the events file stream, alongside the current timestamp.
  void onEvent(const std::string &value);

private:
  // Generate the full messages/events file prefix using the common file prefix
  // and the FIX session id.
  std::string generate_prefix(const std::string &file_prefix,
                              const FIX::SessionID &session_id);

  // Initializes the messages/events files and file streams.
  void init(std::string path, std::string backup_path,
            const std::string &prefix);

private:
  std::ofstream m_messages_fstream;
  std::ofstream m_events_fstream;
  std::string m_messages_file_name;
  std::string m_events_file_name;
  std::string m_fix_log_prefix;
  std::string m_fix_log_backup_prefix;
};