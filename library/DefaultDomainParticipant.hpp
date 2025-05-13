#pragma once

#include "DefaultDomainParticipantConstants.hpp"
#include "DefaultDomainParticipantTypes.hpp"
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/publisher/DataWriterListener.hpp>

class DefaultDomainParticipant {
  class PubListener : public DataWriterListener {
  public:
    PubListener() : m_matched(0), m_topic_description("Undefined") {};
    PubListener(const Topic *topic) : m_matched(0) {
      std::stringstream desc_stream;
      desc_stream << "Type : " << topic->get_type_name() << " - "
                  << "Name : " << topic->get_name() << std::endl;
      m_topic_description = desc_stream.str();

      LOG4CXX_INFO(logger, "PubListener Description : " << m_topic_description);
    }

    ~PubListener() override {}

    void on_publication_matched(
        DataWriter *,
        const eprosima::fastdds::dds::PublicationMatchedStatus &info) override {
      if (info.current_count_change == 1) {
        m_matched = info.total_count;
        LOG4CXX_INFO(logger, "Publisher matched:" << m_topic_description
                                                  << " Count : " << m_matched);
      } else if (info.current_count_change == -1) {
        m_matched = info.total_count;
        LOG4CXX_INFO(logger,
                     "Publisher unmatched:" << m_topic_description
                                            << " Count : " << m_matched);
      } else {
        LOG4CXX_ERROR(logger,
                      "" << info.current_count_change
                         << " is not a valid value for "
                            "PublicationMatchedStatus current count change");
      }
    }

    std::atomic_int m_matched;
    std::string m_topic_description;
  };

public:
  // Instantiate a domain participant for a provided domain id.
  DefaultDomainParticipant(eprosima::fastdds::dds::DomainId_t domain_id,
                           const std::string &participant_name)
      : m_participant_name(participant_name) {
    eprosima::fastdds::dds::DomainParticipantQos participantQos;

    participantQos.name(m_participant_name);
    participantQos.setup_transports(
        eprosima::fastdds::rtps::BuiltinTransports::LARGE_DATA);

    m_participant = DomainParticipantPtr(
        eprosima::fastdds::dds::DomainParticipantFactory::get_instance()
            ->create_participant(domain_id, participantQos));

    if (m_participant == nullptr) {
      throw std::runtime_error("Failed to create a participant : " +
                               m_participant_name);
    }
  }

  // Deletes domain participant.
  virtual ~DefaultDomainParticipant() {
    eprosima::fastdds::dds::DomainParticipantFactory::get_instance()
        ->delete_participant(m_participant.get());
  }

  bool create_publisher() {
    m_publisher = PublisherPtr(m_participant->create_publisher(
        eprosima::fastdds::dds::PUBLISHER_QOS_DEFAULT, nullptr));
    return !(m_publisher == nullptr);
  }

  bool create_subscriber() {
    m_subscriber = SubscriberPtr(m_participant->create_subscriber(
        eprosima::fastdds::dds::SUBSCRIBER_QOS_DEFAULT, nullptr));
    return !(m_subscriber == nullptr);
  }

  template <class TOPIC_SUPPORT_TYPE, class TOPIC_TYPE>
  TopicTuplePtr<TOPIC_TYPE> make_topic(const std::string &topic_name) {
    std::unique_ptr<TOPIC_TYPE> topic_type_ptr = std::make_unique<TOPIC_TYPE>();
    std::unique_ptr<TypeSupport> type_support_ptr =
        std::make_unique<TypeSupport>(new TOPIC_SUPPORT_TYPE);

    type_support_ptr->register_type(m_participant.get());

    std::unique_ptr<Topic> topic_ptr(m_participant->create_topic(
        topic_name, type_support_ptr->get_type_name(),
        eprosima::fastdds::dds::TOPIC_QOS_DEFAULT));

    return std::make_unique<TopicTuple<TOPIC_TYPE>>(
        std::move(topic_ptr), std::move(topic_type_ptr),
        std::move(type_support_ptr));
  }

  template <class TOPIC_TYPE>
  DataReaderTuplePtr<TOPIC_TYPE> make_data_reader_tuple(
      const TopicTuplePtr<TOPIC_TYPE> &topic_tuple,
      eprosima::fastdds::dds::DataReaderListener *data_reader_listener,
      const std::string &filter_name, const std::string &filter,
      const std::vector<std::string> &expression_parameters,
      bool quote_all_expressions = true) {
    std::vector<std::string> final_expression_parameters;

    if (quote_all_expressions) {
      final_expression_parameters.reserve(expression_parameters.size());
      for (const auto &v : expression_parameters) {
        final_expression_parameters.push_back("'" + v + "'");
      }
    } else {
      final_expression_parameters = expression_parameters;
    }

    auto filter_ptr = make_content_filtered_topic<TOPIC_TYPE>(
        filter_name, topic_tuple, filter, final_expression_parameters);
    if (filter_ptr == nullptr) {
      throw std::runtime_error("make_content_filtered_topic failed : " +
                               filter_name);
    }

    DataReaderListenerPtr drl_ptr = DataReaderListenerPtr(data_reader_listener);
    auto data_reader = make_datareader(filter_ptr, drl_ptr);

    return std::make_unique<DataReaderTuple<TOPIC_TYPE>>(
        std::move(filter_ptr), std::move(drl_ptr), std::move(data_reader));
  };

  template <class TOPIC_TYPE>
  DataReaderTuplePtr<TOPIC_TYPE> make_data_reader_tuple(
      const TopicTuplePtr<TOPIC_TYPE> &topic_tuple,
      eprosima::fastdds::dds::DataReaderListener *data_reader_listener) {
    DataReaderListenerPtr drl_ptr(data_reader_listener);
    auto data_reader = make_datareader<TOPIC_TYPE>(topic_tuple, drl_ptr);

    return std::make_unique<DataReaderTuple<TOPIC_TYPE>>(
        nullptr, std::move(drl_ptr), std::move(data_reader));
  };

  template <class TOPIC_TYPE>
  DataWriterPtr make_data_writer(const TopicTuplePtr<TOPIC_TYPE> &topic_tuple) {
    auto topic = std::get<0>(*topic_tuple).get();
    return DataWriterPtr(m_publisher->create_datawriter(
        topic, eprosima::fastdds::dds::DATAWRITER_QOS_DEFAULT,
        new PubListener(topic)));
  }

  template <class TOPIC_TYPE>
  DataWriterPtr
  make_data_writer(const TopicTuplePtr<TOPIC_TYPE> &topic_tuple,
                   eprosima::fastdds::dds::DataWriterListener *listener) {
    auto topic = std::get<0>(*topic_tuple).get();
    return DataWriterPtr(m_publisher->create_datawriter(
        topic, eprosima::fastdds::dds::DATAWRITER_QOS_DEFAULT, listener));
  }

  template <class TOPIC_TYPE>
  ContentFilteredTopicPtr make_content_filtered_topic(
      const std::string &filter_name,
      const TopicTuplePtr<TOPIC_TYPE> &topic_tuple,
      const std::string &filter_expression,
      const std::vector<std::string> &expression_parameters) {
    auto *topic = std::get<0>(*topic_tuple).get();
    if (topic == nullptr) {
      throw std::runtime_error(
          "Topic is null in make_content_filtered_topic : " + filter_name);
    }

    return ContentFilteredTopicPtr(m_participant->create_contentfilteredtopic(
        filter_name, topic, filter_expression, expression_parameters));
  }

  template <class TOPIC_TYPE>
  DataReaderPtr make_datareader(const TopicTuplePtr<TOPIC_TYPE> &topic_tuple,
                                const DataReaderListenerPtr &listener) {
    return DataReaderPtr(m_subscriber->create_datareader(
        std::get<0>(*topic_tuple).get(),
        eprosima::fastdds::dds::DATAREADER_QOS_DEFAULT, listener.get()));
  };

  DataReaderPtr
  make_datareader(const ContentFilteredTopicPtr &content_filtered_topic,
                  const DataReaderListenerPtr &listener) {
    return DataReaderPtr(m_subscriber->create_datareader(
        content_filtered_topic.get(),
        eprosima::fastdds::dds::DATAREADER_QOS_DEFAULT, listener.get()));
  };

  DomainParticipantPtr get_domain_participant() { return m_participant; }

  const std::string &get_participant_name() { return m_participant_name; };

private:
  DomainParticipantPtr m_participant;
  PublisherPtr m_publisher;
  SubscriberPtr m_subscriber;
  std::string m_participant_name;
};
