#include "InstrumentReferenceDataService.hpp"
#include "DataServiceDataWriterContainer.hpp"
#include "Instrument.hpp"
#include "SQLiteConnection.hpp"
#include "SQLiteQuery.hpp"
#include <DefaultDomainParticipantConstants.hpp>
#include <SecurityList.hpp>
#include <SecurityListLogger.hpp>
#include <SecurityListRequest.hpp>
#include <quickfix/FixValues.h>

InstrumentReferenceDataService::InstrumentReferenceDataService(
    const FIX::DatabaseConnectionID &database_connection_id,
    const DataWriterContainerPtr &data_writer_container_ptr,
    SecurityListRequestQueuePtr security_list_request_queue_ptr)
    : m_data_writer_container_ptr(data_writer_container_ptr),
      m_security_list_request_queue_ptr(security_list_request_queue_ptr) {
  m_sqlite_connection_ptr =
      std::make_unique<SQLiteConnection>(database_connection_id);

  std::atomic_init(&m_is_running, true);
  m_service_thread =
      std::thread(&InstrumentReferenceDataService::service, this);

  initialize();
}

InstrumentReferenceDataService::~InstrumentReferenceDataService() {
  m_is_running.store(false);
  m_service_thread.join();
}

void InstrumentReferenceDataService::initialize() {
  // This query returns a list of instruments that each user has access to,
  // based on their user group and the markets mapped to those groups. This is
  // used to populate both the instrument map and the user instrument list map.
  std::string instrument_ref_data_query_str =
      "SELECT i.name, i.properties, u.username, m.name FROM users u, "
      "instruments i, markets m, instrument_markets im, user_group_markets ugm "
      "WHERE im.instrument_name = i.name AND u.user_group = ugm.user_group AND "
      "m.name = im.market_name AND im.market_name = ugm.market_name;";
  SQLiteQuery instrument_ref_data_query(instrument_ref_data_query_str, true,
                                        {});
  m_sqlite_connection_ptr->execute(instrument_ref_data_query);

  // Populate the user instrument list map and the instrument map using the
  // database info.
  for (int row = 0; row < instrument_ref_data_query.get_num_rows(); ++row) {
    std::string symbol = instrument_ref_data_query.get_value(row, 0);
    std::string properties = instrument_ref_data_query.get_value(row, 1);
    std::string username = instrument_ref_data_query.get_value(row, 2);
    std::string market = instrument_ref_data_query.get_value(row, 3);

    auto instr_iter = m_instrument_map.find(symbol);
    if (instr_iter == m_instrument_map.end())
      instr_iter =
          m_instrument_map
              .emplace(symbol,
                       std::make_unique<Instrument>(market, symbol, properties))
              .first;

    auto instr_list_iter = m_user_instrument_list_map.find(username);
    if (instr_list_iter == m_user_instrument_list_map.end())
      instr_list_iter =
          m_user_instrument_list_map
              .emplace(username,
                       std::make_shared<InstrumentList>(InstrumentList()))
              .first;

    instr_list_iter->second->emplace_back(instr_iter->second);
  }
}

void InstrumentReferenceDataService::service() {
  while (m_is_running.load()) {
    if (!m_sqlite_connection_ptr->is_connected()) {
      LOG4CXX_ERROR(logger, "Auth Service is not connected to the database.\n");
      return;
    }

    while (!m_security_list_request_queue_ptr->empty()) {
      SecurityListRequestPtr security_list_request_ptr;
      m_security_list_request_queue_ptr->pop(security_list_request_ptr);
      process_ref_data_request(std::move(security_list_request_ptr));
    }

    std::this_thread::sleep_for(
        std::chrono::duration<long double, std::milli>(1000));
  }
}

void InstrumentReferenceDataService::process_ref_data_request(
    SecurityListRequestPtr security_list_request_ptr) {
  InstrumentListPtr instrument_list_ptr =
      std::make_shared<InstrumentList>(InstrumentList());

  std::string username = security_list_request_ptr->SourceUser();
  if (username.empty())
    username = security_list_request_ptr->Source();

  // If there's an instrument list attached to a username already, populate it.
  auto instr_list_iter = m_user_instrument_list_map.find(username);
  if (instr_list_iter != m_user_instrument_list_map.end())
    instrument_list_ptr->assign(instr_list_iter->second->begin(),
                                instr_list_iter->second->end());

  DistributedStockExchange_SecurityList::SecurityList security_list;

  security_list.fix_header().MsgType(FIX::MsgType_SecurityList);
  security_list.Source(security_list_request_ptr->Destination());
  security_list.Destination(security_list_request_ptr->Source());
  security_list.DestinationUser(security_list_request_ptr->SourceUser());
  security_list.SecurityReqID(security_list_request_ptr->SecurityReqID());
  security_list.SecurityResponseID("1");
  security_list.SecurityRequestResult(0);

  // Copy all the instruments that the user can see into the security list DDS
  // response for the user request sent by FIX gateway.

  security_list.c_NoRelatedSym().resize(instrument_list_ptr->size());
  int instrument_index = 0;

  for (const auto &instrument : *instrument_list_ptr) {
    security_list.c_NoRelatedSym()[instrument_index].Symbol(instrument->symbol);
    security_list.c_NoRelatedSym()[instrument_index].SecurityExchange(
        instrument->marketName);
    security_list.c_NoRelatedSym()[instrument_index].Text(
        instrument->properties);

    instrument_index++;
  };

  std::stringstream ss;
  SecurityListLogger::log(ss, security_list);
  LOG4CXX_INFO(logger, "Publishing SecurityList : [" << ss.str() << "]");

  eprosima::fastdds::dds::ReturnCode_t code =
      m_data_writer_container_ptr->securityListDW->write(&security_list);

  if (code != eprosima::fastdds::dds::RETCODE_OK)
    LOG4CXX_ERROR(logger, "Security List write returned :" << code);
}