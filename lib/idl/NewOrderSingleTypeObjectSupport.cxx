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
 * @file NewOrderSingleTypeObjectSupport.cxx
 * Source file containing the implementation to register the TypeObject representation of the described types in the IDL file
 *
 * This file was generated by the tool fastddsgen.
 */

#include "NewOrderSingleTypeObjectSupport.hpp"

#include <mutex>
#include <string>

#include <fastcdr/xcdr/external.hpp>
#include <fastcdr/xcdr/optional.hpp>
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/log/Log.hpp>
#include <fastdds/dds/xtypes/common.hpp>
#include <fastdds/dds/xtypes/type_representation/ITypeObjectRegistry.hpp>
#include <fastdds/dds/xtypes/type_representation/TypeObject.hpp>
#include <fastdds/dds/xtypes/type_representation/TypeObjectUtils.hpp>

#include "NewOrderSingle.hpp"

#include "Header.hpp"

using namespace eprosima::fastdds::dds::xtypes;

namespace NewOrderSingle {
// TypeIdentifier is returned by reference: dependent structures/unions are registered in this same method
void register_NewOrderSingle_type_identifier(
        TypeIdentifierPair& type_ids_NewOrderSingle)
{

    ReturnCode_t return_code_NewOrderSingle {eprosima::fastdds::dds::RETCODE_OK};
    return_code_NewOrderSingle =
        eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->type_object_registry().get_type_identifiers(
        "NewOrderSingle::NewOrderSingle", type_ids_NewOrderSingle);
    if (eprosima::fastdds::dds::RETCODE_OK != return_code_NewOrderSingle)
    {
        StructTypeFlag struct_flags_NewOrderSingle = TypeObjectUtils::build_struct_type_flag(eprosima::fastdds::dds::xtypes::ExtensibilityKind::APPENDABLE,
                false, false);
        QualifiedTypeName type_name_NewOrderSingle = "NewOrderSingle::NewOrderSingle";
        eprosima::fastcdr::optional<AppliedBuiltinTypeAnnotations> type_ann_builtin_NewOrderSingle;
        eprosima::fastcdr::optional<AppliedAnnotationSeq> ann_custom_NewOrderSingle;
        CompleteTypeDetail detail_NewOrderSingle = TypeObjectUtils::build_complete_type_detail(type_ann_builtin_NewOrderSingle, ann_custom_NewOrderSingle, type_name_NewOrderSingle.to_string());
        CompleteStructHeader header_NewOrderSingle;
        header_NewOrderSingle = TypeObjectUtils::build_complete_struct_header(TypeIdentifier(), detail_NewOrderSingle);
        CompleteStructMemberSeq member_seq_NewOrderSingle;
        {
            TypeIdentifierPair type_ids_header;
            ReturnCode_t return_code_header {eprosima::fastdds::dds::RETCODE_OK};
            return_code_header =
                eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->type_object_registry().get_type_identifiers(
                "Header", type_ids_header);

            if (eprosima::fastdds::dds::RETCODE_OK != return_code_header)
            {
            ::register_Header_type_identifier(type_ids_header);
            }
            StructMemberFlag member_flags_header = TypeObjectUtils::build_struct_member_flag(eprosima::fastdds::dds::xtypes::TryConstructFailAction::DISCARD,
                    false, false, false, false);
            MemberId member_id_header = 0x00000000;
            bool common_header_ec {false};
            CommonStructMember common_header {TypeObjectUtils::build_common_struct_member(member_id_header, member_flags_header, TypeObjectUtils::retrieve_complete_type_identifier(type_ids_header, common_header_ec))};
            if (!common_header_ec)
            {
                EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION, "Structure header member TypeIdentifier inconsistent.");
                return;
            }
            MemberName name_header = "header";
            eprosima::fastcdr::optional<AppliedBuiltinMemberAnnotations> member_ann_builtin_header;
            ann_custom_NewOrderSingle.reset();
            CompleteMemberDetail detail_header = TypeObjectUtils::build_complete_member_detail(name_header, member_ann_builtin_header, ann_custom_NewOrderSingle);
            CompleteStructMember member_header = TypeObjectUtils::build_complete_struct_member(common_header, detail_header);
            TypeObjectUtils::add_complete_struct_member(member_seq_NewOrderSingle, member_header);
        }
        {
            TypeIdentifierPair type_ids_ClOrdID;
            ReturnCode_t return_code_ClOrdID {eprosima::fastdds::dds::RETCODE_OK};
            return_code_ClOrdID =
                eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->type_object_registry().get_type_identifiers(
                "anonymous_string_unbounded", type_ids_ClOrdID);

            if (eprosima::fastdds::dds::RETCODE_OK != return_code_ClOrdID)
            {
                {
                    SBound bound = 0;
                    StringSTypeDefn string_sdefn = TypeObjectUtils::build_string_s_type_defn(bound);
                    if (eprosima::fastdds::dds::RETCODE_BAD_PARAMETER ==
                            TypeObjectUtils::build_and_register_s_string_type_identifier(string_sdefn,
                            "anonymous_string_unbounded", type_ids_ClOrdID))
                    {
                        EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION,
                            "anonymous_string_unbounded already registered in TypeObjectRegistry for a different type.");
                    }
                }
            }
            StructMemberFlag member_flags_ClOrdID = TypeObjectUtils::build_struct_member_flag(eprosima::fastdds::dds::xtypes::TryConstructFailAction::DISCARD,
                    false, false, false, false);
            MemberId member_id_ClOrdID = 0x00000001;
            bool common_ClOrdID_ec {false};
            CommonStructMember common_ClOrdID {TypeObjectUtils::build_common_struct_member(member_id_ClOrdID, member_flags_ClOrdID, TypeObjectUtils::retrieve_complete_type_identifier(type_ids_ClOrdID, common_ClOrdID_ec))};
            if (!common_ClOrdID_ec)
            {
                EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION, "Structure ClOrdID member TypeIdentifier inconsistent.");
                return;
            }
            MemberName name_ClOrdID = "ClOrdID";
            eprosima::fastcdr::optional<AppliedBuiltinMemberAnnotations> member_ann_builtin_ClOrdID;
            ann_custom_NewOrderSingle.reset();
            CompleteMemberDetail detail_ClOrdID = TypeObjectUtils::build_complete_member_detail(name_ClOrdID, member_ann_builtin_ClOrdID, ann_custom_NewOrderSingle);
            CompleteStructMember member_ClOrdID = TypeObjectUtils::build_complete_struct_member(common_ClOrdID, detail_ClOrdID);
            TypeObjectUtils::add_complete_struct_member(member_seq_NewOrderSingle, member_ClOrdID);
        }
        {
            TypeIdentifierPair type_ids_ExecInst;
            ReturnCode_t return_code_ExecInst {eprosima::fastdds::dds::RETCODE_OK};
            return_code_ExecInst =
                eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->type_object_registry().get_type_identifiers(
                "anonymous_string_unbounded", type_ids_ExecInst);

            if (eprosima::fastdds::dds::RETCODE_OK != return_code_ExecInst)
            {
                {
                    SBound bound = 0;
                    StringSTypeDefn string_sdefn = TypeObjectUtils::build_string_s_type_defn(bound);
                    if (eprosima::fastdds::dds::RETCODE_BAD_PARAMETER ==
                            TypeObjectUtils::build_and_register_s_string_type_identifier(string_sdefn,
                            "anonymous_string_unbounded", type_ids_ExecInst))
                    {
                        EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION,
                            "anonymous_string_unbounded already registered in TypeObjectRegistry for a different type.");
                    }
                }
            }
            StructMemberFlag member_flags_ExecInst = TypeObjectUtils::build_struct_member_flag(eprosima::fastdds::dds::xtypes::TryConstructFailAction::DISCARD,
                    false, false, false, false);
            MemberId member_id_ExecInst = 0x00000002;
            bool common_ExecInst_ec {false};
            CommonStructMember common_ExecInst {TypeObjectUtils::build_common_struct_member(member_id_ExecInst, member_flags_ExecInst, TypeObjectUtils::retrieve_complete_type_identifier(type_ids_ExecInst, common_ExecInst_ec))};
            if (!common_ExecInst_ec)
            {
                EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION, "Structure ExecInst member TypeIdentifier inconsistent.");
                return;
            }
            MemberName name_ExecInst = "ExecInst";
            eprosima::fastcdr::optional<AppliedBuiltinMemberAnnotations> member_ann_builtin_ExecInst;
            ann_custom_NewOrderSingle.reset();
            CompleteMemberDetail detail_ExecInst = TypeObjectUtils::build_complete_member_detail(name_ExecInst, member_ann_builtin_ExecInst, ann_custom_NewOrderSingle);
            CompleteStructMember member_ExecInst = TypeObjectUtils::build_complete_struct_member(common_ExecInst, detail_ExecInst);
            TypeObjectUtils::add_complete_struct_member(member_seq_NewOrderSingle, member_ExecInst);
        }
        {
            TypeIdentifierPair type_ids_Symbol;
            ReturnCode_t return_code_Symbol {eprosima::fastdds::dds::RETCODE_OK};
            return_code_Symbol =
                eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->type_object_registry().get_type_identifiers(
                "anonymous_string_unbounded", type_ids_Symbol);

            if (eprosima::fastdds::dds::RETCODE_OK != return_code_Symbol)
            {
                {
                    SBound bound = 0;
                    StringSTypeDefn string_sdefn = TypeObjectUtils::build_string_s_type_defn(bound);
                    if (eprosima::fastdds::dds::RETCODE_BAD_PARAMETER ==
                            TypeObjectUtils::build_and_register_s_string_type_identifier(string_sdefn,
                            "anonymous_string_unbounded", type_ids_Symbol))
                    {
                        EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION,
                            "anonymous_string_unbounded already registered in TypeObjectRegistry for a different type.");
                    }
                }
            }
            StructMemberFlag member_flags_Symbol = TypeObjectUtils::build_struct_member_flag(eprosima::fastdds::dds::xtypes::TryConstructFailAction::DISCARD,
                    false, false, false, false);
            MemberId member_id_Symbol = 0x00000003;
            bool common_Symbol_ec {false};
            CommonStructMember common_Symbol {TypeObjectUtils::build_common_struct_member(member_id_Symbol, member_flags_Symbol, TypeObjectUtils::retrieve_complete_type_identifier(type_ids_Symbol, common_Symbol_ec))};
            if (!common_Symbol_ec)
            {
                EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION, "Structure Symbol member TypeIdentifier inconsistent.");
                return;
            }
            MemberName name_Symbol = "Symbol";
            eprosima::fastcdr::optional<AppliedBuiltinMemberAnnotations> member_ann_builtin_Symbol;
            ann_custom_NewOrderSingle.reset();
            CompleteMemberDetail detail_Symbol = TypeObjectUtils::build_complete_member_detail(name_Symbol, member_ann_builtin_Symbol, ann_custom_NewOrderSingle);
            CompleteStructMember member_Symbol = TypeObjectUtils::build_complete_struct_member(common_Symbol, detail_Symbol);
            TypeObjectUtils::add_complete_struct_member(member_seq_NewOrderSingle, member_Symbol);
        }
        {
            TypeIdentifierPair type_ids_SecurityExchange;
            ReturnCode_t return_code_SecurityExchange {eprosima::fastdds::dds::RETCODE_OK};
            return_code_SecurityExchange =
                eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->type_object_registry().get_type_identifiers(
                "anonymous_string_unbounded", type_ids_SecurityExchange);

            if (eprosima::fastdds::dds::RETCODE_OK != return_code_SecurityExchange)
            {
                {
                    SBound bound = 0;
                    StringSTypeDefn string_sdefn = TypeObjectUtils::build_string_s_type_defn(bound);
                    if (eprosima::fastdds::dds::RETCODE_BAD_PARAMETER ==
                            TypeObjectUtils::build_and_register_s_string_type_identifier(string_sdefn,
                            "anonymous_string_unbounded", type_ids_SecurityExchange))
                    {
                        EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION,
                            "anonymous_string_unbounded already registered in TypeObjectRegistry for a different type.");
                    }
                }
            }
            StructMemberFlag member_flags_SecurityExchange = TypeObjectUtils::build_struct_member_flag(eprosima::fastdds::dds::xtypes::TryConstructFailAction::DISCARD,
                    false, false, false, false);
            MemberId member_id_SecurityExchange = 0x00000004;
            bool common_SecurityExchange_ec {false};
            CommonStructMember common_SecurityExchange {TypeObjectUtils::build_common_struct_member(member_id_SecurityExchange, member_flags_SecurityExchange, TypeObjectUtils::retrieve_complete_type_identifier(type_ids_SecurityExchange, common_SecurityExchange_ec))};
            if (!common_SecurityExchange_ec)
            {
                EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION, "Structure SecurityExchange member TypeIdentifier inconsistent.");
                return;
            }
            MemberName name_SecurityExchange = "SecurityExchange";
            eprosima::fastcdr::optional<AppliedBuiltinMemberAnnotations> member_ann_builtin_SecurityExchange;
            ann_custom_NewOrderSingle.reset();
            CompleteMemberDetail detail_SecurityExchange = TypeObjectUtils::build_complete_member_detail(name_SecurityExchange, member_ann_builtin_SecurityExchange, ann_custom_NewOrderSingle);
            CompleteStructMember member_SecurityExchange = TypeObjectUtils::build_complete_struct_member(common_SecurityExchange, detail_SecurityExchange);
            TypeObjectUtils::add_complete_struct_member(member_seq_NewOrderSingle, member_SecurityExchange);
        }
        {
            TypeIdentifierPair type_ids_Side;
            ReturnCode_t return_code_Side {eprosima::fastdds::dds::RETCODE_OK};
            return_code_Side =
                eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->type_object_registry().get_type_identifiers(
                "_char", type_ids_Side);

            if (eprosima::fastdds::dds::RETCODE_OK != return_code_Side)
            {
                EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION,
                        "Side Structure member TypeIdentifier unknown to TypeObjectRegistry.");
                return;
            }
            StructMemberFlag member_flags_Side = TypeObjectUtils::build_struct_member_flag(eprosima::fastdds::dds::xtypes::TryConstructFailAction::DISCARD,
                    false, false, false, false);
            MemberId member_id_Side = 0x00000005;
            bool common_Side_ec {false};
            CommonStructMember common_Side {TypeObjectUtils::build_common_struct_member(member_id_Side, member_flags_Side, TypeObjectUtils::retrieve_complete_type_identifier(type_ids_Side, common_Side_ec))};
            if (!common_Side_ec)
            {
                EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION, "Structure Side member TypeIdentifier inconsistent.");
                return;
            }
            MemberName name_Side = "Side";
            eprosima::fastcdr::optional<AppliedBuiltinMemberAnnotations> member_ann_builtin_Side;
            ann_custom_NewOrderSingle.reset();
            CompleteMemberDetail detail_Side = TypeObjectUtils::build_complete_member_detail(name_Side, member_ann_builtin_Side, ann_custom_NewOrderSingle);
            CompleteStructMember member_Side = TypeObjectUtils::build_complete_struct_member(common_Side, detail_Side);
            TypeObjectUtils::add_complete_struct_member(member_seq_NewOrderSingle, member_Side);
        }
        {
            TypeIdentifierPair type_ids_TransactTime;
            ReturnCode_t return_code_TransactTime {eprosima::fastdds::dds::RETCODE_OK};
            return_code_TransactTime =
                eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->type_object_registry().get_type_identifiers(
                "_uint64_t", type_ids_TransactTime);

            if (eprosima::fastdds::dds::RETCODE_OK != return_code_TransactTime)
            {
                EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION,
                        "TransactTime Structure member TypeIdentifier unknown to TypeObjectRegistry.");
                return;
            }
            StructMemberFlag member_flags_TransactTime = TypeObjectUtils::build_struct_member_flag(eprosima::fastdds::dds::xtypes::TryConstructFailAction::DISCARD,
                    false, false, false, false);
            MemberId member_id_TransactTime = 0x00000006;
            bool common_TransactTime_ec {false};
            CommonStructMember common_TransactTime {TypeObjectUtils::build_common_struct_member(member_id_TransactTime, member_flags_TransactTime, TypeObjectUtils::retrieve_complete_type_identifier(type_ids_TransactTime, common_TransactTime_ec))};
            if (!common_TransactTime_ec)
            {
                EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION, "Structure TransactTime member TypeIdentifier inconsistent.");
                return;
            }
            MemberName name_TransactTime = "TransactTime";
            eprosima::fastcdr::optional<AppliedBuiltinMemberAnnotations> member_ann_builtin_TransactTime;
            ann_custom_NewOrderSingle.reset();
            CompleteMemberDetail detail_TransactTime = TypeObjectUtils::build_complete_member_detail(name_TransactTime, member_ann_builtin_TransactTime, ann_custom_NewOrderSingle);
            CompleteStructMember member_TransactTime = TypeObjectUtils::build_complete_struct_member(common_TransactTime, detail_TransactTime);
            TypeObjectUtils::add_complete_struct_member(member_seq_NewOrderSingle, member_TransactTime);
        }
        {
            TypeIdentifierPair type_ids_OrderQty;
            ReturnCode_t return_code_OrderQty {eprosima::fastdds::dds::RETCODE_OK};
            return_code_OrderQty =
                eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->type_object_registry().get_type_identifiers(
                "_int32_t", type_ids_OrderQty);

            if (eprosima::fastdds::dds::RETCODE_OK != return_code_OrderQty)
            {
                EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION,
                        "OrderQty Structure member TypeIdentifier unknown to TypeObjectRegistry.");
                return;
            }
            StructMemberFlag member_flags_OrderQty = TypeObjectUtils::build_struct_member_flag(eprosima::fastdds::dds::xtypes::TryConstructFailAction::DISCARD,
                    false, false, false, false);
            MemberId member_id_OrderQty = 0x00000007;
            bool common_OrderQty_ec {false};
            CommonStructMember common_OrderQty {TypeObjectUtils::build_common_struct_member(member_id_OrderQty, member_flags_OrderQty, TypeObjectUtils::retrieve_complete_type_identifier(type_ids_OrderQty, common_OrderQty_ec))};
            if (!common_OrderQty_ec)
            {
                EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION, "Structure OrderQty member TypeIdentifier inconsistent.");
                return;
            }
            MemberName name_OrderQty = "OrderQty";
            eprosima::fastcdr::optional<AppliedBuiltinMemberAnnotations> member_ann_builtin_OrderQty;
            ann_custom_NewOrderSingle.reset();
            CompleteMemberDetail detail_OrderQty = TypeObjectUtils::build_complete_member_detail(name_OrderQty, member_ann_builtin_OrderQty, ann_custom_NewOrderSingle);
            CompleteStructMember member_OrderQty = TypeObjectUtils::build_complete_struct_member(common_OrderQty, detail_OrderQty);
            TypeObjectUtils::add_complete_struct_member(member_seq_NewOrderSingle, member_OrderQty);
        }
        {
            TypeIdentifierPair type_ids_OrdType;
            ReturnCode_t return_code_OrdType {eprosima::fastdds::dds::RETCODE_OK};
            return_code_OrdType =
                eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->type_object_registry().get_type_identifiers(
                "_char", type_ids_OrdType);

            if (eprosima::fastdds::dds::RETCODE_OK != return_code_OrdType)
            {
                EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION,
                        "OrdType Structure member TypeIdentifier unknown to TypeObjectRegistry.");
                return;
            }
            StructMemberFlag member_flags_OrdType = TypeObjectUtils::build_struct_member_flag(eprosima::fastdds::dds::xtypes::TryConstructFailAction::DISCARD,
                    false, false, false, false);
            MemberId member_id_OrdType = 0x00000008;
            bool common_OrdType_ec {false};
            CommonStructMember common_OrdType {TypeObjectUtils::build_common_struct_member(member_id_OrdType, member_flags_OrdType, TypeObjectUtils::retrieve_complete_type_identifier(type_ids_OrdType, common_OrdType_ec))};
            if (!common_OrdType_ec)
            {
                EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION, "Structure OrdType member TypeIdentifier inconsistent.");
                return;
            }
            MemberName name_OrdType = "OrdType";
            eprosima::fastcdr::optional<AppliedBuiltinMemberAnnotations> member_ann_builtin_OrdType;
            ann_custom_NewOrderSingle.reset();
            CompleteMemberDetail detail_OrdType = TypeObjectUtils::build_complete_member_detail(name_OrdType, member_ann_builtin_OrdType, ann_custom_NewOrderSingle);
            CompleteStructMember member_OrdType = TypeObjectUtils::build_complete_struct_member(common_OrdType, detail_OrdType);
            TypeObjectUtils::add_complete_struct_member(member_seq_NewOrderSingle, member_OrdType);
        }
        {
            TypeIdentifierPair type_ids_Price;
            ReturnCode_t return_code_Price {eprosima::fastdds::dds::RETCODE_OK};
            return_code_Price =
                eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->type_object_registry().get_type_identifiers(
                "_float", type_ids_Price);

            if (eprosima::fastdds::dds::RETCODE_OK != return_code_Price)
            {
                EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION,
                        "Price Structure member TypeIdentifier unknown to TypeObjectRegistry.");
                return;
            }
            StructMemberFlag member_flags_Price = TypeObjectUtils::build_struct_member_flag(eprosima::fastdds::dds::xtypes::TryConstructFailAction::DISCARD,
                    false, false, false, false);
            MemberId member_id_Price = 0x00000009;
            bool common_Price_ec {false};
            CommonStructMember common_Price {TypeObjectUtils::build_common_struct_member(member_id_Price, member_flags_Price, TypeObjectUtils::retrieve_complete_type_identifier(type_ids_Price, common_Price_ec))};
            if (!common_Price_ec)
            {
                EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION, "Structure Price member TypeIdentifier inconsistent.");
                return;
            }
            MemberName name_Price = "Price";
            eprosima::fastcdr::optional<AppliedBuiltinMemberAnnotations> member_ann_builtin_Price;
            ann_custom_NewOrderSingle.reset();
            CompleteMemberDetail detail_Price = TypeObjectUtils::build_complete_member_detail(name_Price, member_ann_builtin_Price, ann_custom_NewOrderSingle);
            CompleteStructMember member_Price = TypeObjectUtils::build_complete_struct_member(common_Price, detail_Price);
            TypeObjectUtils::add_complete_struct_member(member_seq_NewOrderSingle, member_Price);
        }
        {
            TypeIdentifierPair type_ids_StopPx;
            ReturnCode_t return_code_StopPx {eprosima::fastdds::dds::RETCODE_OK};
            return_code_StopPx =
                eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->type_object_registry().get_type_identifiers(
                "_float", type_ids_StopPx);

            if (eprosima::fastdds::dds::RETCODE_OK != return_code_StopPx)
            {
                EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION,
                        "StopPx Structure member TypeIdentifier unknown to TypeObjectRegistry.");
                return;
            }
            StructMemberFlag member_flags_StopPx = TypeObjectUtils::build_struct_member_flag(eprosima::fastdds::dds::xtypes::TryConstructFailAction::DISCARD,
                    false, false, false, false);
            MemberId member_id_StopPx = 0x0000000a;
            bool common_StopPx_ec {false};
            CommonStructMember common_StopPx {TypeObjectUtils::build_common_struct_member(member_id_StopPx, member_flags_StopPx, TypeObjectUtils::retrieve_complete_type_identifier(type_ids_StopPx, common_StopPx_ec))};
            if (!common_StopPx_ec)
            {
                EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION, "Structure StopPx member TypeIdentifier inconsistent.");
                return;
            }
            MemberName name_StopPx = "StopPx";
            eprosima::fastcdr::optional<AppliedBuiltinMemberAnnotations> member_ann_builtin_StopPx;
            ann_custom_NewOrderSingle.reset();
            CompleteMemberDetail detail_StopPx = TypeObjectUtils::build_complete_member_detail(name_StopPx, member_ann_builtin_StopPx, ann_custom_NewOrderSingle);
            CompleteStructMember member_StopPx = TypeObjectUtils::build_complete_struct_member(common_StopPx, detail_StopPx);
            TypeObjectUtils::add_complete_struct_member(member_seq_NewOrderSingle, member_StopPx);
        }
        {
            TypeIdentifierPair type_ids_TimeInForce;
            ReturnCode_t return_code_TimeInForce {eprosima::fastdds::dds::RETCODE_OK};
            return_code_TimeInForce =
                eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->type_object_registry().get_type_identifiers(
                "_char", type_ids_TimeInForce);

            if (eprosima::fastdds::dds::RETCODE_OK != return_code_TimeInForce)
            {
                EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION,
                        "TimeInForce Structure member TypeIdentifier unknown to TypeObjectRegistry.");
                return;
            }
            StructMemberFlag member_flags_TimeInForce = TypeObjectUtils::build_struct_member_flag(eprosima::fastdds::dds::xtypes::TryConstructFailAction::DISCARD,
                    false, false, false, false);
            MemberId member_id_TimeInForce = 0x0000000b;
            bool common_TimeInForce_ec {false};
            CommonStructMember common_TimeInForce {TypeObjectUtils::build_common_struct_member(member_id_TimeInForce, member_flags_TimeInForce, TypeObjectUtils::retrieve_complete_type_identifier(type_ids_TimeInForce, common_TimeInForce_ec))};
            if (!common_TimeInForce_ec)
            {
                EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION, "Structure TimeInForce member TypeIdentifier inconsistent.");
                return;
            }
            MemberName name_TimeInForce = "TimeInForce";
            eprosima::fastcdr::optional<AppliedBuiltinMemberAnnotations> member_ann_builtin_TimeInForce;
            ann_custom_NewOrderSingle.reset();
            CompleteMemberDetail detail_TimeInForce = TypeObjectUtils::build_complete_member_detail(name_TimeInForce, member_ann_builtin_TimeInForce, ann_custom_NewOrderSingle);
            CompleteStructMember member_TimeInForce = TypeObjectUtils::build_complete_struct_member(common_TimeInForce, detail_TimeInForce);
            TypeObjectUtils::add_complete_struct_member(member_seq_NewOrderSingle, member_TimeInForce);
        }
        {
            TypeIdentifierPair type_ids_Text;
            ReturnCode_t return_code_Text {eprosima::fastdds::dds::RETCODE_OK};
            return_code_Text =
                eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->type_object_registry().get_type_identifiers(
                "anonymous_string_unbounded", type_ids_Text);

            if (eprosima::fastdds::dds::RETCODE_OK != return_code_Text)
            {
                {
                    SBound bound = 0;
                    StringSTypeDefn string_sdefn = TypeObjectUtils::build_string_s_type_defn(bound);
                    if (eprosima::fastdds::dds::RETCODE_BAD_PARAMETER ==
                            TypeObjectUtils::build_and_register_s_string_type_identifier(string_sdefn,
                            "anonymous_string_unbounded", type_ids_Text))
                    {
                        EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION,
                            "anonymous_string_unbounded already registered in TypeObjectRegistry for a different type.");
                    }
                }
            }
            StructMemberFlag member_flags_Text = TypeObjectUtils::build_struct_member_flag(eprosima::fastdds::dds::xtypes::TryConstructFailAction::DISCARD,
                    false, false, false, false);
            MemberId member_id_Text = 0x0000000c;
            bool common_Text_ec {false};
            CommonStructMember common_Text {TypeObjectUtils::build_common_struct_member(member_id_Text, member_flags_Text, TypeObjectUtils::retrieve_complete_type_identifier(type_ids_Text, common_Text_ec))};
            if (!common_Text_ec)
            {
                EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION, "Structure Text member TypeIdentifier inconsistent.");
                return;
            }
            MemberName name_Text = "Text";
            eprosima::fastcdr::optional<AppliedBuiltinMemberAnnotations> member_ann_builtin_Text;
            ann_custom_NewOrderSingle.reset();
            CompleteMemberDetail detail_Text = TypeObjectUtils::build_complete_member_detail(name_Text, member_ann_builtin_Text, ann_custom_NewOrderSingle);
            CompleteStructMember member_Text = TypeObjectUtils::build_complete_struct_member(common_Text, detail_Text);
            TypeObjectUtils::add_complete_struct_member(member_seq_NewOrderSingle, member_Text);
        }
        CompleteStructType struct_type_NewOrderSingle = TypeObjectUtils::build_complete_struct_type(struct_flags_NewOrderSingle, header_NewOrderSingle, member_seq_NewOrderSingle);
        if (eprosima::fastdds::dds::RETCODE_BAD_PARAMETER ==
                TypeObjectUtils::build_and_register_struct_type_object(struct_type_NewOrderSingle, type_name_NewOrderSingle.to_string(), type_ids_NewOrderSingle))
        {
            EPROSIMA_LOG_ERROR(XTYPES_TYPE_REPRESENTATION,
                    "NewOrderSingle::NewOrderSingle already registered in TypeObjectRegistry for a different type.");
        }
    }
}

} // namespace NewOrderSingle

