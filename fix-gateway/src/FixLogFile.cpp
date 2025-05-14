#include "FixLogFile.hpp"

FixLogFile::FixLogFile(const std::string &path, const std::string &prefix) {
  init(path, path, prefix + "-GLOBAL");
}

FixLogFile::FixLogFile(const std::string &path, const std::string &backup_path,
                       const std::string &prefix) {
  init(path, backup_path, prefix + "-GLOBAL");
}

FixLogFile::FixLogFile(const std::string &path,
                       const FIX::SessionID &session_id,
                       const std::string &prefix) {
  init(path, path, generate_prefix(prefix, session_id));
}

FixLogFile::FixLogFile(const std::string &path, const std::string &backup_path,
                       const FIX::SessionID &session_id,
                       const std::string &prefix) {
  init(path, backup_path, generate_prefix(prefix, session_id));
}

FixLogFile::~FixLogFile() {
  m_messages_fstream.close();
  m_events_fstream.close();
}

void FixLogFile::clear() {
  m_messages_fstream.close();
  m_events_fstream.close();

  m_messages_fstream.open(m_messages_file_name.c_str(),
                          std::ios::out | std::ios::trunc);
  m_events_fstream.open(m_events_file_name.c_str(),
                        std::ios::out | std::ios::trunc);
}

void FixLogFile::backup() {
  m_messages_fstream.close();
  m_events_fstream.close();

  // Loop through backup file names in the format
  // ".backup.{backup_file_num}.log" until we find a suffix that has not been
  // used yet.
  int backup_file_num = 0;
  FILE *messages_log_file = nullptr;
  FILE *event_log_file = nullptr;
  std::string messages_file_name;
  std::string events_file_name;

  do {
    // If the previous iteration led to ONE of messages_log_file/event_log_file
    // being non nullptr, close that first.
    if (messages_log_file != nullptr)
      FIX::file_fclose(messages_log_file);
    if (event_log_file != nullptr)
      FIX::file_fclose(event_log_file);

    messages_file_name = m_fix_log_backup_prefix + "messages.backup." +
                         std::to_string(++backup_file_num) + ".log";
    events_file_name = m_fix_log_backup_prefix + "events.backup." +
                       std::to_string(backup_file_num) + ".log";

    messages_log_file = FIX::file_fopen(messages_file_name.c_str(), "r");
    event_log_file = FIX::file_fopen(events_file_name.c_str(), "r");
  } while (messages_log_file != nullptr || event_log_file != nullptr);

  // NOTE: There's no memory leaks for the FILE pointers, since the only way we
  // reach the code below is if BOTH files are nullptr.

  // Store the new messages/events backup by just renaming the existing
  // messages/events file into the backup file name.
  FIX::file_rename(m_messages_file_name.c_str(), messages_file_name.c_str());
  FIX::file_rename(m_events_file_name.c_str(), events_file_name.c_str());

  // Reopen the messages/events file streams to maintain behavior, this also
  // clears the messages/events files.
  m_messages_fstream.open(m_messages_file_name.c_str(),
                          std::ios::out | std::ios::trunc);
  m_events_fstream.open(m_events_file_name.c_str(),
                        std::ios::out | std::ios::trunc);
}

void FixLogFile::onIncoming(const std::string &value) {
  m_messages_fstream << FIX::UtcTimeStampConvertor::convert(FIX::UtcTimeStamp(),
                                                            9)
                     << " : " << value << std::endl;
}

void FixLogFile::onOutgoing(const std::string &value) {
  m_messages_fstream << FIX::UtcTimeStampConvertor::convert(FIX::UtcTimeStamp(),
                                                            9)
                     << " : " << value << std::endl;
}

void FixLogFile::onEvent(const std::string &value) {
  m_events_fstream << FIX::UtcTimeStampConvertor::convert(FIX::UtcTimeStamp(),
                                                          9)
                   << " : " << value << std::endl;
}

std::string FixLogFile::generate_prefix(const std::string &file_prefix,
                                        const FIX::SessionID &session_id) {
  const std::string &begin = session_id.getBeginString().getString();
  const std::string &sender = session_id.getSenderCompID().getString();
  const std::string &target = session_id.getTargetCompID().getString();
  const std::string &qualifier = session_id.getSessionQualifier();

  std::string prefix = file_prefix + "-" + begin + "-" + sender + "-" + target;
  if (qualifier.size())
    prefix += "-" + qualifier;

  return prefix;
}

void FixLogFile::init(std::string path, std::string backup_path,
                      const std::string &prefix) {
  // Make the overall path/backup_path directories to store the messages/events
  // log files.
  FIX::file_mkdir(path.c_str());
  FIX::file_mkdir(backup_path.c_str());

  if (path.empty())
    path = ".";
  if (backup_path.empty())
    backup_path = path;

  m_fix_log_prefix = FIX::file_appendpath(path, prefix + ".");
  m_fix_log_backup_prefix = FIX::file_appendpath(backup_path, prefix + ".");

  m_messages_file_name = m_fix_log_prefix + "messages.current.log";
  m_events_file_name = m_fix_log_prefix + "event.current.log";

  m_messages_fstream.open(m_messages_file_name.c_str(),
                          std::ios::out | std::ios::app);
  if (!m_messages_fstream.is_open())
    throw FIX::ConfigError("Could not open messages file: " +
                           m_messages_file_name);

  m_events_fstream.open(m_events_file_name.c_str(),
                        std::ios::out | std::ios::app);
  if (!m_events_fstream.is_open())
    throw FIX::ConfigError("Could not open event file: " + m_events_file_name);
}