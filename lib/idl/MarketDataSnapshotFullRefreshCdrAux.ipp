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
 * @file MarketDataSnapshotFullRefreshCdrAux.ipp
 * This source file contains some declarations of CDR related functions.
 *
 * This file was generated by the tool fastddsgen.
 */

#ifndef FAST_DDS_GENERATED__MARKETDATASNAPSHOTFULLREFRESH_MARKETDATASNAPSHOTFULLREFRESHCDRAUX_IPP
#define FAST_DDS_GENERATED__MARKETDATASNAPSHOTFULLREFRESH_MARKETDATASNAPSHOTFULLREFRESHCDRAUX_IPP

#include "MarketDataSnapshotFullRefreshCdrAux.hpp"

#include <fastcdr/Cdr.h>
#include <fastcdr/CdrSizeCalculator.hpp>


#include <fastcdr/exceptions/BadParamException.h>
using namespace eprosima::fastcdr::exception;

namespace eprosima {
namespace fastcdr {

template<>
eProsima_user_DllExport size_t calculate_serialized_size(
        eprosima::fastcdr::CdrSizeCalculator& calculator,
        const MarketDataSnapshotFullRefresh::NoMDEntries& data,
        size_t& current_alignment)
{
    using namespace MarketDataSnapshotFullRefresh;

    static_cast<void>(data);

    eprosima::fastcdr::EncodingAlgorithmFlag previous_encoding = calculator.get_encoding();
    size_t calculated_size {calculator.begin_calculate_type_serialized_size(
                                eprosima::fastcdr::CdrVersion::XCDRv2 == calculator.get_cdr_version() ?
                                eprosima::fastcdr::EncodingAlgorithmFlag::DELIMIT_CDR2 :
                                eprosima::fastcdr::EncodingAlgorithmFlag::PLAIN_CDR,
                                current_alignment)};


        calculated_size += calculator.calculate_member_serialized_size(eprosima::fastcdr::MemberId(0),
                data.MDEntryType(), current_alignment);

        calculated_size += calculator.calculate_member_serialized_size(eprosima::fastcdr::MemberId(1),
                data.MDEntryPx(), current_alignment);

        calculated_size += calculator.calculate_member_serialized_size(eprosima::fastcdr::MemberId(2),
                data.MDEntrySize(), current_alignment);

        calculated_size += calculator.calculate_member_serialized_size(eprosima::fastcdr::MemberId(3),
                data.TimeInForce(), current_alignment);

        calculated_size += calculator.calculate_member_serialized_size(eprosima::fastcdr::MemberId(4),
                data.ExecInst(), current_alignment);

        calculated_size += calculator.calculate_member_serialized_size(eprosima::fastcdr::MemberId(5),
                data.Text(), current_alignment);


    calculated_size += calculator.end_calculate_type_serialized_size(previous_encoding, current_alignment);

    return calculated_size;
}

template<>
eProsima_user_DllExport void serialize(
        eprosima::fastcdr::Cdr& scdr,
        const MarketDataSnapshotFullRefresh::NoMDEntries& data)
{
    using namespace MarketDataSnapshotFullRefresh;

    eprosima::fastcdr::Cdr::state current_state(scdr);
    scdr.begin_serialize_type(current_state,
            eprosima::fastcdr::CdrVersion::XCDRv2 == scdr.get_cdr_version() ?
            eprosima::fastcdr::EncodingAlgorithmFlag::DELIMIT_CDR2 :
            eprosima::fastcdr::EncodingAlgorithmFlag::PLAIN_CDR);

    scdr
        << eprosima::fastcdr::MemberId(0) << data.MDEntryType()
        << eprosima::fastcdr::MemberId(1) << data.MDEntryPx()
        << eprosima::fastcdr::MemberId(2) << data.MDEntrySize()
        << eprosima::fastcdr::MemberId(3) << data.TimeInForce()
        << eprosima::fastcdr::MemberId(4) << data.ExecInst()
        << eprosima::fastcdr::MemberId(5) << data.Text()
;
    scdr.end_serialize_type(current_state);
}

template<>
eProsima_user_DllExport void deserialize(
        eprosima::fastcdr::Cdr& cdr,
        MarketDataSnapshotFullRefresh::NoMDEntries& data)
{
    using namespace MarketDataSnapshotFullRefresh;

    cdr.deserialize_type(eprosima::fastcdr::CdrVersion::XCDRv2 == cdr.get_cdr_version() ?
            eprosima::fastcdr::EncodingAlgorithmFlag::DELIMIT_CDR2 :
            eprosima::fastcdr::EncodingAlgorithmFlag::PLAIN_CDR,
            [&data](eprosima::fastcdr::Cdr& dcdr, const eprosima::fastcdr::MemberId& mid) -> bool
            {
                bool ret_value = true;
                switch (mid.id)
                {
                                        case 0:
                                                dcdr >> data.MDEntryType();
                                            break;

                                        case 1:
                                                dcdr >> data.MDEntryPx();
                                            break;

                                        case 2:
                                                dcdr >> data.MDEntrySize();
                                            break;

                                        case 3:
                                                dcdr >> data.TimeInForce();
                                            break;

                                        case 4:
                                                dcdr >> data.ExecInst();
                                            break;

                                        case 5:
                                                dcdr >> data.Text();
                                            break;

                    default:
                        ret_value = false;
                        break;
                }
                return ret_value;
            });
}

void serialize_key(
        eprosima::fastcdr::Cdr& scdr,
        const MarketDataSnapshotFullRefresh::NoMDEntries& data)
{
    using namespace MarketDataSnapshotFullRefresh;

    static_cast<void>(scdr);
    static_cast<void>(data);
                        scdr << data.MDEntryType();

                        scdr << data.MDEntryPx();

                        scdr << data.MDEntrySize();

                        scdr << data.TimeInForce();

                        scdr << data.ExecInst();

                        scdr << data.Text();

}



template<>
eProsima_user_DllExport size_t calculate_serialized_size(
        eprosima::fastcdr::CdrSizeCalculator& calculator,
        const MarketDataSnapshotFullRefresh::MarketDataSnapshotFullRefresh& data,
        size_t& current_alignment)
{
    using namespace MarketDataSnapshotFullRefresh;

    static_cast<void>(data);

    eprosima::fastcdr::EncodingAlgorithmFlag previous_encoding = calculator.get_encoding();
    size_t calculated_size {calculator.begin_calculate_type_serialized_size(
                                eprosima::fastcdr::CdrVersion::XCDRv2 == calculator.get_cdr_version() ?
                                eprosima::fastcdr::EncodingAlgorithmFlag::DELIMIT_CDR2 :
                                eprosima::fastcdr::EncodingAlgorithmFlag::PLAIN_CDR,
                                current_alignment)};


        calculated_size += calculator.calculate_member_serialized_size(eprosima::fastcdr::MemberId(0),
                data.header(), current_alignment);

        calculated_size += calculator.calculate_member_serialized_size(eprosima::fastcdr::MemberId(1),
                data.MDReqID(), current_alignment);

        calculated_size += calculator.calculate_member_serialized_size(eprosima::fastcdr::MemberId(2),
                data.Symbol(), current_alignment);

        calculated_size += calculator.calculate_member_serialized_size(eprosima::fastcdr::MemberId(3),
                data.SecurityExchange(), current_alignment);

        calculated_size += calculator.calculate_member_serialized_size(eprosima::fastcdr::MemberId(4),
                data.c_NoMDEntries(), current_alignment);


    calculated_size += calculator.end_calculate_type_serialized_size(previous_encoding, current_alignment);

    return calculated_size;
}

template<>
eProsima_user_DllExport void serialize(
        eprosima::fastcdr::Cdr& scdr,
        const MarketDataSnapshotFullRefresh::MarketDataSnapshotFullRefresh& data)
{
    using namespace MarketDataSnapshotFullRefresh;

    eprosima::fastcdr::Cdr::state current_state(scdr);
    scdr.begin_serialize_type(current_state,
            eprosima::fastcdr::CdrVersion::XCDRv2 == scdr.get_cdr_version() ?
            eprosima::fastcdr::EncodingAlgorithmFlag::DELIMIT_CDR2 :
            eprosima::fastcdr::EncodingAlgorithmFlag::PLAIN_CDR);

    scdr
        << eprosima::fastcdr::MemberId(0) << data.header()
        << eprosima::fastcdr::MemberId(1) << data.MDReqID()
        << eprosima::fastcdr::MemberId(2) << data.Symbol()
        << eprosima::fastcdr::MemberId(3) << data.SecurityExchange()
        << eprosima::fastcdr::MemberId(4) << data.c_NoMDEntries()
;
    scdr.end_serialize_type(current_state);
}

template<>
eProsima_user_DllExport void deserialize(
        eprosima::fastcdr::Cdr& cdr,
        MarketDataSnapshotFullRefresh::MarketDataSnapshotFullRefresh& data)
{
    using namespace MarketDataSnapshotFullRefresh;

    cdr.deserialize_type(eprosima::fastcdr::CdrVersion::XCDRv2 == cdr.get_cdr_version() ?
            eprosima::fastcdr::EncodingAlgorithmFlag::DELIMIT_CDR2 :
            eprosima::fastcdr::EncodingAlgorithmFlag::PLAIN_CDR,
            [&data](eprosima::fastcdr::Cdr& dcdr, const eprosima::fastcdr::MemberId& mid) -> bool
            {
                bool ret_value = true;
                switch (mid.id)
                {
                                        case 0:
                                                dcdr >> data.header();
                                            break;

                                        case 1:
                                                dcdr >> data.MDReqID();
                                            break;

                                        case 2:
                                                dcdr >> data.Symbol();
                                            break;

                                        case 3:
                                                dcdr >> data.SecurityExchange();
                                            break;

                                        case 4:
                                                dcdr >> data.c_NoMDEntries();
                                            break;

                    default:
                        ret_value = false;
                        break;
                }
                return ret_value;
            });
}

void serialize_key(
        eprosima::fastcdr::Cdr& scdr,
        const MarketDataSnapshotFullRefresh::MarketDataSnapshotFullRefresh& data)
{
    using namespace MarketDataSnapshotFullRefresh;
            extern void serialize_key(
                    Cdr& scdr,
                    const Header& data);






    static_cast<void>(scdr);
    static_cast<void>(data);
                        serialize_key(scdr, data.header());

                        scdr << data.MDReqID();

                        scdr << data.Symbol();

                        scdr << data.SecurityExchange();

                        scdr << data.c_NoMDEntries();

}



} // namespace fastcdr
} // namespace eprosima

#endif // FAST_DDS_GENERATED__MARKETDATASNAPSHOTFULLREFRESH_MARKETDATASNAPSHOTFULLREFRESHCDRAUX_IPP

