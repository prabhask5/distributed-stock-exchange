#pragma once

#include "MarketTypes.hpp"
#include <DefaultDomainParticipantTypes.hpp>
#include <fastdds/dds/publisher/DataWriterListener.hpp>

// This class sends a request to the data service to ask for a list of
// securities that the market can use for initialization and configuration.
class SecurityListRequestDataWriterListener : public DataWriterListener {
public:
  SecurityListRequestDataWriterListener(const MarketPtr &market_ptr);

  void on_publication_matched(
      DataWriter *dwr,
      const eprosima::fastdds::dds::PublicationMatchedStatus &info) override;

private:
  MarketPtr m_market_ptr;
  std::atomic_int m_matched;
};