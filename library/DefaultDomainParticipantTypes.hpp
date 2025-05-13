#pragma once

#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>
#include <memory>

using TypeSupport = eprosima::fastdds::dds::TypeSupport;
using Topic = eprosima::fastdds::dds::Topic;
using DomainParticipant = eprosima::fastdds::dds::DomainParticipant;
using Publisher = eprosima::fastdds::dds::Publisher;
using Subscriber = eprosima::fastdds::dds::Subscriber;
using DataReaderListener = eprosima::fastdds::dds::DataReaderListener;
using DataWriterListener = eprosima::fastdds::dds::DataWriterListener;
using DataReader = eprosima::fastdds::dds::DataReader;
using DataWriter = eprosima::fastdds::dds::DataWriter;
using ContentFilteredTopic = eprosima::fastdds::dds::ContentFilteredTopic;

using DomainParticipantPtr = std::shared_ptr<DomainParticipant>;
using PublisherPtr = std::unique_ptr<Publisher>;
using SubscriberPtr = std::unique_ptr<Subscriber>;
using DataReaderPtr = std::unique_ptr<DataReader>;
using DataWriterPtr = std::unique_ptr<DataWriter>;
using ContentFilteredTopicPtr = std::unique_ptr<ContentFilteredTopic>;
using DataReaderListenerPtr = std::unique_ptr<DataReaderListener>;

template <class TopicType>
using TopicTuple =
    std::tuple<std::unique_ptr<Topic>, std::unique_ptr<TopicType>,
               std::unique_ptr<TypeSupport>>;

template <class TopicType>
using TopicTuplePtr = std::unique_ptr<TopicTuple<TopicType>>;

template <class TopicType>
using DataReaderTuple =
    std::tuple<ContentFilteredTopicPtr, DataReaderListenerPtr, DataReaderPtr>;

template <class TopicType>
using DataReaderTuplePtr = std::unique_ptr<DataReaderTuple<TopicType>>;

class DefaultDomainParticipant; // From DefaultDomainParticipant.hpp.
using DefaultDomainParticipantPtr = std::shared_ptr<DefaultDomainParticipant>;