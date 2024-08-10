// Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/*!
 * @file MarketDataIncrementalRefreshPubSubTypes.cpp
 * This header file contains the implementation of the serialization functions.
 *
 * This file was generated by the tool fastddsgen.
 */

#include "MarketDataIncrementalRefreshPubSubTypes.hpp"

#include <fastdds/dds/log/Log.hpp>
#include <fastdds/rtps/common/CdrSerialization.hpp>

#include "MarketDataIncrementalRefreshCdrAux.hpp"
#include "MarketDataIncrementalRefreshTypeObjectSupport.hpp"

using SerializedPayload_t = eprosima::fastdds::rtps::SerializedPayload_t;
using InstanceHandle_t = eprosima::fastdds::rtps::InstanceHandle_t;
using DataRepresentationId_t = eprosima::fastdds::dds::DataRepresentationId_t;

namespace MarketDataIncrementalRefresh {
    NoMDEntriesPubSubType::NoMDEntriesPubSubType()
    {
        set_name("MarketDataIncrementalRefresh::NoMDEntries");
        uint32_t type_size = MarketDataIncrementalRefresh_NoMDEntries_max_cdr_typesize;
        type_size += static_cast<uint32_t>(eprosima::fastcdr::Cdr::alignment(type_size, 4)); /* possible submessage alignment */
        max_serialized_type_size = type_size + 4; /*encapsulation*/
        is_compute_key_provided = false;
        uint32_t key_length = MarketDataIncrementalRefresh_NoMDEntries_max_key_cdr_typesize > 16 ? MarketDataIncrementalRefresh_NoMDEntries_max_key_cdr_typesize : 16;
        key_buffer_ = reinterpret_cast<unsigned char*>(malloc(key_length));
        memset(key_buffer_, 0, key_length);
    }

    NoMDEntriesPubSubType::~NoMDEntriesPubSubType()
    {
        if (key_buffer_ != nullptr)
        {
            free(key_buffer_);
        }
    }

    bool NoMDEntriesPubSubType::serialize(
            const void* const data,
            SerializedPayload_t& payload,
            DataRepresentationId_t data_representation)
    {
        const NoMDEntries* p_type = static_cast<const NoMDEntries*>(data);

        // Object that manages the raw buffer.
        eprosima::fastcdr::FastBuffer fastbuffer(reinterpret_cast<char*>(payload.data), payload.max_size);
        // Object that serializes the data.
        eprosima::fastcdr::Cdr ser(fastbuffer, eprosima::fastcdr::Cdr::DEFAULT_ENDIAN,
                data_representation == DataRepresentationId_t::XCDR_DATA_REPRESENTATION ?
                eprosima::fastcdr::CdrVersion::XCDRv1 : eprosima::fastcdr::CdrVersion::XCDRv2);
        payload.encapsulation = ser.endianness() == eprosima::fastcdr::Cdr::BIG_ENDIANNESS ? CDR_BE : CDR_LE;
        ser.set_encoding_flag(
            data_representation == DataRepresentationId_t::XCDR_DATA_REPRESENTATION ?
            eprosima::fastcdr::EncodingAlgorithmFlag::PLAIN_CDR  :
            eprosima::fastcdr::EncodingAlgorithmFlag::DELIMIT_CDR2);

        try
        {
            // Serialize encapsulation
            ser.serialize_encapsulation();
            // Serialize the object.
            ser << *p_type;
        }
        catch (eprosima::fastcdr::exception::Exception& /*exception*/)
        {
            return false;
        }

        // Get the serialized length
        payload.length = static_cast<uint32_t>(ser.get_serialized_data_length());
        return true;
    }

    bool NoMDEntriesPubSubType::deserialize(
            SerializedPayload_t& payload,
            void* data)
    {
        try
        {
            // Convert DATA to pointer of your type
            NoMDEntries* p_type = static_cast<NoMDEntries*>(data);

            // Object that manages the raw buffer.
            eprosima::fastcdr::FastBuffer fastbuffer(reinterpret_cast<char*>(payload.data), payload.length);

            // Object that deserializes the data.
            eprosima::fastcdr::Cdr deser(fastbuffer, eprosima::fastcdr::Cdr::DEFAULT_ENDIAN);

            // Deserialize encapsulation.
            deser.read_encapsulation();
            payload.encapsulation = deser.endianness() == eprosima::fastcdr::Cdr::BIG_ENDIANNESS ? CDR_BE : CDR_LE;

            // Deserialize the object.
            deser >> *p_type;
        }
        catch (eprosima::fastcdr::exception::Exception& /*exception*/)
        {
            return false;
        }

        return true;
    }

    uint32_t NoMDEntriesPubSubType::calculate_serialized_size(
            const void* const data,
            DataRepresentationId_t data_representation)
    {
        try
        {
            eprosima::fastcdr::CdrSizeCalculator calculator(
                data_representation == DataRepresentationId_t::XCDR_DATA_REPRESENTATION ?
                eprosima::fastcdr::CdrVersion::XCDRv1 :eprosima::fastcdr::CdrVersion::XCDRv2);
            size_t current_alignment {0};
            return static_cast<uint32_t>(calculator.calculate_serialized_size(
                        *static_cast<const NoMDEntries*>(data), current_alignment)) +
                    4u /*encapsulation*/;
        }
        catch (eprosima::fastcdr::exception::Exception& /*exception*/)
        {
            return 0;
        }
    }

    void* NoMDEntriesPubSubType::create_data()
    {
        return reinterpret_cast<void*>(new NoMDEntries());
    }

    void NoMDEntriesPubSubType::delete_data(
            void* data)
    {
        delete(reinterpret_cast<NoMDEntries*>(data));
    }

    bool NoMDEntriesPubSubType::compute_key(
            SerializedPayload_t& payload,
            InstanceHandle_t& handle,
            bool force_md5)
    {
        if (!is_compute_key_provided)
        {
            return false;
        }

        NoMDEntries data;
        if (deserialize(payload, static_cast<void*>(&data)))
        {
            return compute_key(static_cast<void*>(&data), handle, force_md5);
        }

        return false;
    }

    bool NoMDEntriesPubSubType::compute_key(
            const void* const data,
            InstanceHandle_t& handle,
            bool force_md5)
    {
        if (!is_compute_key_provided)
        {
            return false;
        }

        const NoMDEntries* p_type = static_cast<const NoMDEntries*>(data);

        // Object that manages the raw buffer.
        eprosima::fastcdr::FastBuffer fastbuffer(reinterpret_cast<char*>(key_buffer_),
                MarketDataIncrementalRefresh_NoMDEntries_max_key_cdr_typesize);

        // Object that serializes the data.
        eprosima::fastcdr::Cdr ser(fastbuffer, eprosima::fastcdr::Cdr::BIG_ENDIANNESS, eprosima::fastcdr::CdrVersion::XCDRv2);
        ser.set_encoding_flag(eprosima::fastcdr::EncodingAlgorithmFlag::PLAIN_CDR2);
        eprosima::fastcdr::serialize_key(ser, *p_type);
        if (force_md5 || MarketDataIncrementalRefresh_NoMDEntries_max_key_cdr_typesize > 16)
        {
            md5_.init();
            md5_.update(key_buffer_, static_cast<unsigned int>(ser.get_serialized_data_length()));
            md5_.finalize();
            for (uint8_t i = 0; i < 16; ++i)
            {
                handle.value[i] = md5_.digest[i];
            }
        }
        else
        {
            for (uint8_t i = 0; i < 16; ++i)
            {
                handle.value[i] = key_buffer_[i];
            }
        }
        return true;
    }

    void NoMDEntriesPubSubType::register_type_object_representation()
    {
        register_NoMDEntries_type_identifier(type_identifiers_);
    }


    MarketDataIncrementalRefreshPubSubType::MarketDataIncrementalRefreshPubSubType()
    {
        set_name("MarketDataIncrementalRefresh::MarketDataIncrementalRefresh");
        uint32_t type_size = MarketDataIncrementalRefresh_MarketDataIncrementalRefresh_max_cdr_typesize;
        type_size += static_cast<uint32_t>(eprosima::fastcdr::Cdr::alignment(type_size, 4)); /* possible submessage alignment */
        max_serialized_type_size = type_size + 4; /*encapsulation*/
        is_compute_key_provided = false;
        uint32_t key_length = MarketDataIncrementalRefresh_MarketDataIncrementalRefresh_max_key_cdr_typesize > 16 ? MarketDataIncrementalRefresh_MarketDataIncrementalRefresh_max_key_cdr_typesize : 16;
        key_buffer_ = reinterpret_cast<unsigned char*>(malloc(key_length));
        memset(key_buffer_, 0, key_length);
    }

    MarketDataIncrementalRefreshPubSubType::~MarketDataIncrementalRefreshPubSubType()
    {
        if (key_buffer_ != nullptr)
        {
            free(key_buffer_);
        }
    }

    bool MarketDataIncrementalRefreshPubSubType::serialize(
            const void* const data,
            SerializedPayload_t& payload,
            DataRepresentationId_t data_representation)
    {
        const MarketDataIncrementalRefresh* p_type = static_cast<const MarketDataIncrementalRefresh*>(data);

        // Object that manages the raw buffer.
        eprosima::fastcdr::FastBuffer fastbuffer(reinterpret_cast<char*>(payload.data), payload.max_size);
        // Object that serializes the data.
        eprosima::fastcdr::Cdr ser(fastbuffer, eprosima::fastcdr::Cdr::DEFAULT_ENDIAN,
                data_representation == DataRepresentationId_t::XCDR_DATA_REPRESENTATION ?
                eprosima::fastcdr::CdrVersion::XCDRv1 : eprosima::fastcdr::CdrVersion::XCDRv2);
        payload.encapsulation = ser.endianness() == eprosima::fastcdr::Cdr::BIG_ENDIANNESS ? CDR_BE : CDR_LE;
        ser.set_encoding_flag(
            data_representation == DataRepresentationId_t::XCDR_DATA_REPRESENTATION ?
            eprosima::fastcdr::EncodingAlgorithmFlag::PLAIN_CDR  :
            eprosima::fastcdr::EncodingAlgorithmFlag::DELIMIT_CDR2);

        try
        {
            // Serialize encapsulation
            ser.serialize_encapsulation();
            // Serialize the object.
            ser << *p_type;
        }
        catch (eprosima::fastcdr::exception::Exception& /*exception*/)
        {
            return false;
        }

        // Get the serialized length
        payload.length = static_cast<uint32_t>(ser.get_serialized_data_length());
        return true;
    }

    bool MarketDataIncrementalRefreshPubSubType::deserialize(
            SerializedPayload_t& payload,
            void* data)
    {
        try
        {
            // Convert DATA to pointer of your type
            MarketDataIncrementalRefresh* p_type = static_cast<MarketDataIncrementalRefresh*>(data);

            // Object that manages the raw buffer.
            eprosima::fastcdr::FastBuffer fastbuffer(reinterpret_cast<char*>(payload.data), payload.length);

            // Object that deserializes the data.
            eprosima::fastcdr::Cdr deser(fastbuffer, eprosima::fastcdr::Cdr::DEFAULT_ENDIAN);

            // Deserialize encapsulation.
            deser.read_encapsulation();
            payload.encapsulation = deser.endianness() == eprosima::fastcdr::Cdr::BIG_ENDIANNESS ? CDR_BE : CDR_LE;

            // Deserialize the object.
            deser >> *p_type;
        }
        catch (eprosima::fastcdr::exception::Exception& /*exception*/)
        {
            return false;
        }

        return true;
    }

    uint32_t MarketDataIncrementalRefreshPubSubType::calculate_serialized_size(
            const void* const data,
            DataRepresentationId_t data_representation)
    {
        try
        {
            eprosima::fastcdr::CdrSizeCalculator calculator(
                data_representation == DataRepresentationId_t::XCDR_DATA_REPRESENTATION ?
                eprosima::fastcdr::CdrVersion::XCDRv1 :eprosima::fastcdr::CdrVersion::XCDRv2);
            size_t current_alignment {0};
            return static_cast<uint32_t>(calculator.calculate_serialized_size(
                        *static_cast<const MarketDataIncrementalRefresh*>(data), current_alignment)) +
                    4u /*encapsulation*/;
        }
        catch (eprosima::fastcdr::exception::Exception& /*exception*/)
        {
            return 0;
        }
    }

    void* MarketDataIncrementalRefreshPubSubType::create_data()
    {
        return reinterpret_cast<void*>(new MarketDataIncrementalRefresh());
    }

    void MarketDataIncrementalRefreshPubSubType::delete_data(
            void* data)
    {
        delete(reinterpret_cast<MarketDataIncrementalRefresh*>(data));
    }

    bool MarketDataIncrementalRefreshPubSubType::compute_key(
            SerializedPayload_t& payload,
            InstanceHandle_t& handle,
            bool force_md5)
    {
        if (!is_compute_key_provided)
        {
            return false;
        }

        MarketDataIncrementalRefresh data;
        if (deserialize(payload, static_cast<void*>(&data)))
        {
            return compute_key(static_cast<void*>(&data), handle, force_md5);
        }

        return false;
    }

    bool MarketDataIncrementalRefreshPubSubType::compute_key(
            const void* const data,
            InstanceHandle_t& handle,
            bool force_md5)
    {
        if (!is_compute_key_provided)
        {
            return false;
        }

        const MarketDataIncrementalRefresh* p_type = static_cast<const MarketDataIncrementalRefresh*>(data);

        // Object that manages the raw buffer.
        eprosima::fastcdr::FastBuffer fastbuffer(reinterpret_cast<char*>(key_buffer_),
                MarketDataIncrementalRefresh_MarketDataIncrementalRefresh_max_key_cdr_typesize);

        // Object that serializes the data.
        eprosima::fastcdr::Cdr ser(fastbuffer, eprosima::fastcdr::Cdr::BIG_ENDIANNESS, eprosima::fastcdr::CdrVersion::XCDRv2);
        ser.set_encoding_flag(eprosima::fastcdr::EncodingAlgorithmFlag::PLAIN_CDR2);
        eprosima::fastcdr::serialize_key(ser, *p_type);
        if (force_md5 || MarketDataIncrementalRefresh_MarketDataIncrementalRefresh_max_key_cdr_typesize > 16)
        {
            md5_.init();
            md5_.update(key_buffer_, static_cast<unsigned int>(ser.get_serialized_data_length()));
            md5_.finalize();
            for (uint8_t i = 0; i < 16; ++i)
            {
                handle.value[i] = md5_.digest[i];
            }
        }
        else
        {
            for (uint8_t i = 0; i < 16; ++i)
            {
                handle.value[i] = key_buffer_[i];
            }
        }
        return true;
    }

    void MarketDataIncrementalRefreshPubSubType::register_type_object_representation()
    {
        register_MarketDataIncrementalRefresh_type_identifier(type_identifiers_);
    }

} // namespace MarketDataIncrementalRefresh


// Include auxiliary functions like for serializing/deserializing.
#include "MarketDataIncrementalRefreshCdrAux.ipp"
