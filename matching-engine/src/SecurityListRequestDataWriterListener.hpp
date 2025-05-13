#pragma once

#include "MarketTypes.hpp"
#include <DefaultDomainParticipantTypes.hpp>
#include <fastdds/dds/publisher/DataWriterListener.hpp>

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