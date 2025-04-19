#pragma once

#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>
#include <fastdds/dds/subscriber/qos/DataReaderQos.hpp>
#include <fastdds/dds/subscriber/SampleInfo.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/publisher/DataWriterListener.hpp>
#include <fastdds/dds/publisher/PublisherListener.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>

#include <memory>
#include <log4cxx/logger.h>

static auto logger = log4cxx::Logger::getRootLogger();

// Wrapper around FastDDS API.
using domain_participant = eprosima::fastdds::dds::DomainParticipant;
using publisher = eprosima::fastdds::dds::Publisher;
using subscriber = eprosima::fastdds::dds::Subscriber;
using data_writer = eprosima::fastdds::dds::DataWriter;
using data_reader = eprosima::fastdds::dds::DataReader;
using content_filtered_topic = eprosima::fastdds::dds::ContentFilteredTopic;
using data_reader_listener = eprosima::fastdds::dds::DataReaderListener;

using domain_participant_ptr = std::shared_ptr<domain_participant>;
using publisher_ptr = std::unique_ptr<publisher>;
using subscriber_ptr = std::unique_ptr<subscriber>;
using data_writer_ptr = std::unique_ptr<data_writer>;
using data_reader_ptr = std::unique_ptr<data_reader>;
using content_filtered_topic_ptr = std::unique_ptr<content_filtered_topic>;
using data_reader_listener_ptr = std::unique_ptr<data_reader_listener>;

template <class TopicType>
using topic_tuple = std::tuple<
    std::unique_ptr<eprosima::fastdds::dds::Topic>,
    std::unique_ptr<TopicType>,
    std::unique_ptr<eprosima::fastdds::dds::TypeSupport>>;

template <class TopicType>
using topic_tuple_ptr = std::unique_ptr<topic_tuple<TopicType>>;

using data_reader_tuple = std::tuple<
    content_filtered_topic_ptr,
    data_reader_listener_ptr,
    data_reader_ptr>;

template <class TopicType>
using data_reader_tuple_ptr = std::unique_ptr<data_reader_tuple<TopicType>>;

class basic_domain_participant
{
    class pub_listener : public eprosima::fastdds::dds::DataWriterListener
    {
    public:
        pub_listener() : m_matched(0), m_topic_description("Undefined") {};
        pub_listener(const eprosima::fastdds::dds::Topic *topic)
            : m_matched(0)
        {
            std::stringstream desc_stream;
            desc_stream << "Type : " << topic->get_type_name() << " - " << "Name : " << topic->get_name() << std::endl;
            m_topic_description = desc_stream.str();

            LOG4CXX_INFO(logger, "PubListener Description : " << m_topic_description);
        }

        ~pub_listener() override = default;

        void on_publication_matched(
            data_writer *,
            const eprosima::fastdds::dds::PublicationMatchedStatus &info) override
        {
            if (info.current_count_change == 1)
            {
                m_matched = info.total_count;
                LOG4CXX_INFO(logger, "Publisher matched:" << m_topic_description << " Count : " << m_matched);
            }
            else if (info.current_count_change == -1)
            {
                m_matched = info.total_count;
                LOG4CXX_INFO(logger, "Publisher unmatched:" << m_topic_description << " Count : " << m_matched);
            }
            else
            {
                LOG4CXX_ERROR(logger, "" << info.current_count_change << " is not a valid value for PublicationMatchedStatus current count change");
            }
        }

        std::atomic_int m_matched;
        std::string m_topic_description;
    };

public:
    // Instantiate a domain participant for a provided domain id
    basic_domain_participant(eprosima::fastdds::dds::DomainId_t domain_id, const std::string &participant_name)
    {
        eprosima::fastdds::dds::DomainParticipantQos participantQos;
        participantQos.name(participant_name);
        participantQos.setup_transports(eprosima::fastdds::rtps::BuiltinTransports::LARGE_DATA);

        m_participant = std::make_shared<domain_participant>(eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->create_participant(domain_id, participantQos));

        if (m_participant == nullptr)
        {
            throw std::runtime_error("Failed to create a participant : " + participant_name);
        }
    }

    // Deletes domain participant
    virtual ~basic_domain_participant()
    {
        eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->delete_participant(m_participant.get());
    }

    bool create_publisher()
    {
        m_publisher = std::make_unique<publisher>(m_participant->create_publisher(eprosima::fastdds::dds::PUBLISHER_QOS_DEFAULT, nullptr));
        return !(m_publisher == nullptr);
    }

    bool create_subscriber()
    {
        m_subscriber = std::make_unique<subscriber>(m_participant->create_subscriber(eprosima::fastdds::dds::SUBSCRIBER_QOS_DEFAULT, nullptr));
        return !(m_subscriber == nullptr);
    }

    template <class TOPIC_SUPPORT_TYPE, class TOPIC_TYPE>
    topic_tuple_ptr<TOPIC_TYPE> make_topic(const std::string &topic_name)
    {
        std::unique_ptr<TOPIC_TYPE> topic_type_ptr = std::make_unique(new TOPIC_TYPE());
        std::unique_ptr<eprosima::fastdds::dds::TypeSupport> type_support_ptr = std::make_unique(new eprosima::fastdds::dds::TypeSupport(new TOPIC_SUPPORT_TYPE));

        type_support_ptr->register_type(m_participant.get());

        std::unique_ptr<eprosima::fastdds::dds::Topic> topic_ptr = std::make_unique(m_participant->create_topic(topic_name, type_support_ptr->get_type_name(), eprosima::fastdds::dds::TOPIC_QOS_DEFAULT));

        return topic_tuple_ptr<TOPIC_TYPE>(new topic_tuple<TOPIC_TYPE>(std::move(topic_ptr), std::move(topic_type_ptr), std::move(type_support_ptr)));
    }

    template <class TOPIC_TYPE>
    data_reader_tuple_ptr<TOPIC_TYPE> make_data_reader_tuple(const topic_tuple_ptr<TOPIC_TYPE> &topic_tuple,
                                                             data_reader_listener *data_reader_listener,
                                                             const std::string &filter_name, const std::string &filter)
    {
        auto filter_ptr = make_content_filtered_topic<TOPIC_TYPE>(filter_name, topic_tuple, filter);
        data_reader_listener_ptr drl_ptr(data_reader_listener);
        auto data_reader = make_datareader(filter_ptr, drl_ptr);

        return data_reader_tuple_ptr<TOPIC_TYPE>(new data_reader_tuple<TOPIC_TYPE>(std::move(filter_ptr), std::move(drl_ptr), std::move(data_reader)));
    };

    template <class TOPIC_TYPE>
    data_reader_tuple_ptr<TOPIC_TYPE> make_data_reader_tuple(const topic_tuple_ptr<TOPIC_TYPE> &topic_tuple,
                                                             data_reader_listener *data_reader_listener)
    {
        data_reader_listener_ptr drl_ptr(data_reader_listener);
        auto data_reader = make_datareader<TOPIC_TYPE>(topic_tuple, drl_ptr);

        return data_reader_tuple_ptr<TOPIC_TYPE>(new data_reader_tuple<TOPIC_TYPE>(nullptr, std::move(drl_ptr), std::move(data_reader)));
    };

    template <class TOPIC_TYPE>
    data_writer_ptr make_data_writer(const topic_tuple_ptr<TOPIC_TYPE> &topic_tuple)
    {
        auto topic = std::get<0>(*topic_tuple).get();
        return std::make_shared<data_writer>(_publisher->create_datawriter(topic,

                                                                           eprosima::fastdds::dds::DATAWRITER_QOS_DEFAULT, new pub_listener(topic)));
    }

    template <class TOPIC_TYPE>
    data_writer_ptr make_data_writer(const topic_tuple_ptr<TOPIC_TYPE> &topic_tuple, eprosima::fastdds::dds::DataWriterListener *listener)
    {
        auto topic = std::get<0>(*topic_tuple).get();

        return std::make_shared<data_writer>(_publisher->create_datawriter(topic,
                                                                           eprosima::fastdds::dds::DATAWRITER_QOS_DEFAULT, listener));
    }

    template <class TOPIC_TYPE>
    content_filtered_topic_ptr make_content_filtered_topic(
        const std::string &filter_name,
        const topic_tuple_ptr<TOPIC_TYPE> &topic_tuple,
        const std::string &filter_expression)
    {

        return std::make_shared<content_filtered_topic>(_participant->create_contentfilteredtopic(
            filter_name, std::get<0>(*topic_tuple).get(), filter_expression,
            std::vector<std::string>()));
    }

    template <class TOPIC_TYPE>
    data_reader_ptr
    make_datareader(const topic_tuple_ptr<TOPIC_TYPE> &topic_tuple, data_reader_listener_ptr &listener)
    {
        return std::make_shared<data_reader>(m_subscriber->create_datareader(
            std::get<0>(*topic_tuple).get(),
            eprosima::fastdds::dds::DATAREADER_QOS_DEFAULT, listener.get()));
    };

    data_reader_ptr
    make_datareader(const content_filtered_topic_ptr &content_filtered_topic,
                    const data_reader_listener_ptr &listener)
    {
        return std::make_unique<data_reader>(m_subscriber->create_datareader(content_filtered_topic.get(), eprosima::fastdds::dds::DATAREADER_QOS_DEFAULT, listener.get()));
    };

    const domain_participant_ptr &get_domain_participant()
    {
        return m_participant;
    }

private:
    domain_participant_ptr m_participant;
    publisher_ptr m_publisher;
    subscriber_ptr m_subscriber;
};

using basic_domain_participant_ptr = std::shared_ptr<basic_domain_participant>;