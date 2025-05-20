#include "AuthService.hpp"
#include "DataServiceDataWriterContainer.hpp"
#include "SQLiteConnection.hpp"
#include "SQLiteQuery.hpp"
#include <DefaultDomainParticipantConstants.hpp>
#include <Logon.hpp>
#include <LogonLogger.hpp>
#include <Logout.hpp>
#include <LogoutLogger.hpp>
#include <quickfix/FixValues.h>

AuthService::AuthService(
    const FIX::DatabaseConnectionID &database_connection_id,
    const DataWriterContainerPtr &data_writer_container_ptr,
    LogonQueuePtr logon_request_queue_ptr)
    : m_data_writer_container_ptr(data_writer_container_ptr),
      m_logon_request_queue_ptr(logon_request_queue_ptr),
      m_credentials_cache([this](const std::string &username) {
        return this->get_password(username);
      }),
      m_auth_query("SELECT password FROM user_groups WHERE name = ?;", true,
                   {}) {
  m_sqlite_connection_ptr =
      std::make_unique<SQLiteConnection>(database_connection_id);

  std::atomic_init(&m_is_running, true);
  m_service_thread = std::thread(&AuthService::service, this);
}

AuthService::~AuthService() {
  m_is_running.store(false);
  m_service_thread.join();
}

void AuthService::service() {
  while (m_is_running.load()) {
    if (!m_sqlite_connection_ptr->is_connected()) {
      LOG4CXX_ERROR(logger, "Auth Service is not connected to the database.\n");
      return;
    }

    while (!m_logon_request_queue_ptr->empty()) {
      LogonPtr logon_ptr;
      m_logon_request_queue_ptr->pop(logon_ptr);
      authenticate(std::move(logon_ptr));
    }

    std::this_thread::sleep_for(
        std::chrono::duration<long double, std::milli>(1000));
  }
}

void AuthService::authenticate(LogonPtr logon_ptr) {
  std::stringstream ss_logon;
  LogonLogger::log(ss_logon, *logon_ptr);

  const std::string &username_input = logon_ptr->Username();
  const std::string &password_input = logon_ptr->Password();
  const std::string &db_password = m_credentials_cache.get(username_input);

  if ((password_input != "" && db_password != "") &&
      (password_input == db_password)) {
    logon_ptr->Source(logon_ptr->Destination());
    logon_ptr->Destination(logon_ptr->Source());
    logon_ptr->SourceUser("AUTH");
    logon_ptr->DestinationUser(logon_ptr->Username());

    LOG4CXX_INFO(logger,
                 "Auth Service Logon Success: [" << ss_logon.str() << "]");

    eprosima::fastdds::dds::ReturnCode_t code =
        m_data_writer_container_ptr->logOnDW->write(logon_ptr.get());

    if (code != eprosima::fastdds::dds::RETCODE_OK)
      LOG4CXX_ERROR(logger, "Error Publishing to DDS :" << ss_logon.str());

  } else {

    LOG4CXX_INFO(logger, "Authentication failed: Publishing logout for : ["
                             << ss_logon.str() << "]");

    DistributedStockExchange_Logout::Logout logout;

    logout.fix_header().BeginString(logon_ptr->fix_header().BeginString());
    logout.fix_header().MsgType(FIX::MsgType_Logout);
    logout.fix_header().SendingTime(0);

    logout.Source(logon_ptr->Destination());
    logout.Destination(logon_ptr->Source());
    logout.DestinationUser(logon_ptr->RawData());

    logout.Text("Invalid username or password.");

    std::stringstream ss_logout;
    LogoutLogger::log(ss_logout, logout);

    LOG4CXX_INFO(logger, "Auth Service Logout : %s\n" << ss_logout.str());

    eprosima::fastdds::dds::ReturnCode_t code =
        m_data_writer_container_ptr->logOutDW->write(&logout);

    if (code != eprosima::fastdds::dds::RETCODE_OK)
      LOG4CXX_ERROR(logger, "Logout write returned error : " << code);
  }
}

std::string AuthService::get_password(const std::string &username) {
  std::vector<std::string> parameters = {username};
  m_auth_query.set_parameters(std::move(parameters));
  m_sqlite_connection_ptr->execute(m_auth_query);

  if (m_auth_query.get_num_rows() == 0)
    return "";
  else
    return m_auth_query.get_value(0, 0);
}