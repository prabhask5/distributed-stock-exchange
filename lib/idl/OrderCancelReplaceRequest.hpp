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
 * @file OrderCancelReplaceRequest.hpp
 * This header file contains the declaration of the described types in the IDL file.
 *
 * This file was generated by the tool fastddsgen.
 */

#ifndef FAST_DDS_GENERATED__ORDERCANCELREPLACEREQUEST_ORDERCANCELREPLACEREQUEST_HPP
#define FAST_DDS_GENERATED__ORDERCANCELREPLACEREQUEST_ORDERCANCELREPLACEREQUEST_HPP

#include <cstdint>
#include <string>
#include <utility>
#include <fastcdr/cdr/fixed_size_string.hpp>
#include "Header.hpp"

#if defined(_WIN32)
#if defined(EPROSIMA_USER_DLL_EXPORT)
#define eProsima_user_DllExport __declspec( dllexport )
#else
#define eProsima_user_DllExport
#endif  // EPROSIMA_USER_DLL_EXPORT
#else
#define eProsima_user_DllExport
#endif  // _WIN32

#if defined(_WIN32)
#if defined(EPROSIMA_USER_DLL_EXPORT)
#if defined(ORDERCANCELREPLACEREQUEST_SOURCE)
#define ORDERCANCELREPLACEREQUEST_DllAPI __declspec( dllexport )
#else
#define ORDERCANCELREPLACEREQUEST_DllAPI __declspec( dllimport )
#endif // ORDERCANCELREPLACEREQUEST_SOURCE
#else
#define ORDERCANCELREPLACEREQUEST_DllAPI
#endif  // EPROSIMA_USER_DLL_EXPORT
#else
#define ORDERCANCELREPLACEREQUEST_DllAPI
#endif // _WIN32

namespace OrderCancelReplaceRequest {

/*!
 * @brief This class represents the structure OrderCancelReplaceRequest defined by the user in the IDL file.
 * @ingroup OrderCancelReplaceRequest
 */
class OrderCancelReplaceRequest
{
public:

    /*!
     * @brief Default constructor.
     */
    eProsima_user_DllExport OrderCancelReplaceRequest()
    {
    }

    /*!
     * @brief Default destructor.
     */
    eProsima_user_DllExport ~OrderCancelReplaceRequest()
    {
    }

    /*!
     * @brief Copy constructor.
     * @param x Reference to the object OrderCancelReplaceRequest that will be copied.
     */
    eProsima_user_DllExport OrderCancelReplaceRequest(
            const OrderCancelReplaceRequest& x)
    {
                    m_header = x.m_header;

                    m_OrigClOrdID = x.m_OrigClOrdID;

                    m_ClOrdID = x.m_ClOrdID;

                    m_ExecInst = x.m_ExecInst;

                    m_Symbol = x.m_Symbol;

                    m_SecurityExchange = x.m_SecurityExchange;

                    m_Side = x.m_Side;

                    m_TransactTime = x.m_TransactTime;

                    m_OrderQty = x.m_OrderQty;

                    m_OrdType = x.m_OrdType;

                    m_Price = x.m_Price;

                    m_StopPx = x.m_StopPx;

                    m_TimeInForce = x.m_TimeInForce;

                    m_Text = x.m_Text;

    }

    /*!
     * @brief Move constructor.
     * @param x Reference to the object OrderCancelReplaceRequest that will be copied.
     */
    eProsima_user_DllExport OrderCancelReplaceRequest(
            OrderCancelReplaceRequest&& x) noexcept
    {
        m_header = std::move(x.m_header);
        m_OrigClOrdID = std::move(x.m_OrigClOrdID);
        m_ClOrdID = std::move(x.m_ClOrdID);
        m_ExecInst = std::move(x.m_ExecInst);
        m_Symbol = std::move(x.m_Symbol);
        m_SecurityExchange = std::move(x.m_SecurityExchange);
        m_Side = x.m_Side;
        m_TransactTime = x.m_TransactTime;
        m_OrderQty = x.m_OrderQty;
        m_OrdType = x.m_OrdType;
        m_Price = x.m_Price;
        m_StopPx = x.m_StopPx;
        m_TimeInForce = x.m_TimeInForce;
        m_Text = std::move(x.m_Text);
    }

    /*!
     * @brief Copy assignment.
     * @param x Reference to the object OrderCancelReplaceRequest that will be copied.
     */
    eProsima_user_DllExport OrderCancelReplaceRequest& operator =(
            const OrderCancelReplaceRequest& x)
    {

                    m_header = x.m_header;

                    m_OrigClOrdID = x.m_OrigClOrdID;

                    m_ClOrdID = x.m_ClOrdID;

                    m_ExecInst = x.m_ExecInst;

                    m_Symbol = x.m_Symbol;

                    m_SecurityExchange = x.m_SecurityExchange;

                    m_Side = x.m_Side;

                    m_TransactTime = x.m_TransactTime;

                    m_OrderQty = x.m_OrderQty;

                    m_OrdType = x.m_OrdType;

                    m_Price = x.m_Price;

                    m_StopPx = x.m_StopPx;

                    m_TimeInForce = x.m_TimeInForce;

                    m_Text = x.m_Text;

        return *this;
    }

    /*!
     * @brief Move assignment.
     * @param x Reference to the object OrderCancelReplaceRequest that will be copied.
     */
    eProsima_user_DllExport OrderCancelReplaceRequest& operator =(
            OrderCancelReplaceRequest&& x) noexcept
    {

        m_header = std::move(x.m_header);
        m_OrigClOrdID = std::move(x.m_OrigClOrdID);
        m_ClOrdID = std::move(x.m_ClOrdID);
        m_ExecInst = std::move(x.m_ExecInst);
        m_Symbol = std::move(x.m_Symbol);
        m_SecurityExchange = std::move(x.m_SecurityExchange);
        m_Side = x.m_Side;
        m_TransactTime = x.m_TransactTime;
        m_OrderQty = x.m_OrderQty;
        m_OrdType = x.m_OrdType;
        m_Price = x.m_Price;
        m_StopPx = x.m_StopPx;
        m_TimeInForce = x.m_TimeInForce;
        m_Text = std::move(x.m_Text);
        return *this;
    }

    /*!
     * @brief Comparison operator.
     * @param x OrderCancelReplaceRequest object to compare.
     */
    eProsima_user_DllExport bool operator ==(
            const OrderCancelReplaceRequest& x) const
    {
        return (m_header == x.m_header &&
           m_OrigClOrdID == x.m_OrigClOrdID &&
           m_ClOrdID == x.m_ClOrdID &&
           m_ExecInst == x.m_ExecInst &&
           m_Symbol == x.m_Symbol &&
           m_SecurityExchange == x.m_SecurityExchange &&
           m_Side == x.m_Side &&
           m_TransactTime == x.m_TransactTime &&
           m_OrderQty == x.m_OrderQty &&
           m_OrdType == x.m_OrdType &&
           m_Price == x.m_Price &&
           m_StopPx == x.m_StopPx &&
           m_TimeInForce == x.m_TimeInForce &&
           m_Text == x.m_Text);
    }

    /*!
     * @brief Comparison operator.
     * @param x OrderCancelReplaceRequest object to compare.
     */
    eProsima_user_DllExport bool operator !=(
            const OrderCancelReplaceRequest& x) const
    {
        return !(*this == x);
    }

    /*!
     * @brief This function copies the value in member header
     * @param _header New value to be copied in member header
     */
    eProsima_user_DllExport void header(
            const Header& _header)
    {
        m_header = _header;
    }

    /*!
     * @brief This function moves the value in member header
     * @param _header New value to be moved in member header
     */
    eProsima_user_DllExport void header(
            Header&& _header)
    {
        m_header = std::move(_header);
    }

    /*!
     * @brief This function returns a constant reference to member header
     * @return Constant reference to member header
     */
    eProsima_user_DllExport const Header& header() const
    {
        return m_header;
    }

    /*!
     * @brief This function returns a reference to member header
     * @return Reference to member header
     */
    eProsima_user_DllExport Header& header()
    {
        return m_header;
    }


    /*!
     * @brief This function copies the value in member OrigClOrdID
     * @param _OrigClOrdID New value to be copied in member OrigClOrdID
     */
    eProsima_user_DllExport void OrigClOrdID(
            const std::string& _OrigClOrdID)
    {
        m_OrigClOrdID = _OrigClOrdID;
    }

    /*!
     * @brief This function moves the value in member OrigClOrdID
     * @param _OrigClOrdID New value to be moved in member OrigClOrdID
     */
    eProsima_user_DllExport void OrigClOrdID(
            std::string&& _OrigClOrdID)
    {
        m_OrigClOrdID = std::move(_OrigClOrdID);
    }

    /*!
     * @brief This function returns a constant reference to member OrigClOrdID
     * @return Constant reference to member OrigClOrdID
     */
    eProsima_user_DllExport const std::string& OrigClOrdID() const
    {
        return m_OrigClOrdID;
    }

    /*!
     * @brief This function returns a reference to member OrigClOrdID
     * @return Reference to member OrigClOrdID
     */
    eProsima_user_DllExport std::string& OrigClOrdID()
    {
        return m_OrigClOrdID;
    }


    /*!
     * @brief This function copies the value in member ClOrdID
     * @param _ClOrdID New value to be copied in member ClOrdID
     */
    eProsima_user_DllExport void ClOrdID(
            const std::string& _ClOrdID)
    {
        m_ClOrdID = _ClOrdID;
    }

    /*!
     * @brief This function moves the value in member ClOrdID
     * @param _ClOrdID New value to be moved in member ClOrdID
     */
    eProsima_user_DllExport void ClOrdID(
            std::string&& _ClOrdID)
    {
        m_ClOrdID = std::move(_ClOrdID);
    }

    /*!
     * @brief This function returns a constant reference to member ClOrdID
     * @return Constant reference to member ClOrdID
     */
    eProsima_user_DllExport const std::string& ClOrdID() const
    {
        return m_ClOrdID;
    }

    /*!
     * @brief This function returns a reference to member ClOrdID
     * @return Reference to member ClOrdID
     */
    eProsima_user_DllExport std::string& ClOrdID()
    {
        return m_ClOrdID;
    }


    /*!
     * @brief This function copies the value in member ExecInst
     * @param _ExecInst New value to be copied in member ExecInst
     */
    eProsima_user_DllExport void ExecInst(
            const std::string& _ExecInst)
    {
        m_ExecInst = _ExecInst;
    }

    /*!
     * @brief This function moves the value in member ExecInst
     * @param _ExecInst New value to be moved in member ExecInst
     */
    eProsima_user_DllExport void ExecInst(
            std::string&& _ExecInst)
    {
        m_ExecInst = std::move(_ExecInst);
    }

    /*!
     * @brief This function returns a constant reference to member ExecInst
     * @return Constant reference to member ExecInst
     */
    eProsima_user_DllExport const std::string& ExecInst() const
    {
        return m_ExecInst;
    }

    /*!
     * @brief This function returns a reference to member ExecInst
     * @return Reference to member ExecInst
     */
    eProsima_user_DllExport std::string& ExecInst()
    {
        return m_ExecInst;
    }


    /*!
     * @brief This function copies the value in member Symbol
     * @param _Symbol New value to be copied in member Symbol
     */
    eProsima_user_DllExport void Symbol(
            const std::string& _Symbol)
    {
        m_Symbol = _Symbol;
    }

    /*!
     * @brief This function moves the value in member Symbol
     * @param _Symbol New value to be moved in member Symbol
     */
    eProsima_user_DllExport void Symbol(
            std::string&& _Symbol)
    {
        m_Symbol = std::move(_Symbol);
    }

    /*!
     * @brief This function returns a constant reference to member Symbol
     * @return Constant reference to member Symbol
     */
    eProsima_user_DllExport const std::string& Symbol() const
    {
        return m_Symbol;
    }

    /*!
     * @brief This function returns a reference to member Symbol
     * @return Reference to member Symbol
     */
    eProsima_user_DllExport std::string& Symbol()
    {
        return m_Symbol;
    }


    /*!
     * @brief This function copies the value in member SecurityExchange
     * @param _SecurityExchange New value to be copied in member SecurityExchange
     */
    eProsima_user_DllExport void SecurityExchange(
            const std::string& _SecurityExchange)
    {
        m_SecurityExchange = _SecurityExchange;
    }

    /*!
     * @brief This function moves the value in member SecurityExchange
     * @param _SecurityExchange New value to be moved in member SecurityExchange
     */
    eProsima_user_DllExport void SecurityExchange(
            std::string&& _SecurityExchange)
    {
        m_SecurityExchange = std::move(_SecurityExchange);
    }

    /*!
     * @brief This function returns a constant reference to member SecurityExchange
     * @return Constant reference to member SecurityExchange
     */
    eProsima_user_DllExport const std::string& SecurityExchange() const
    {
        return m_SecurityExchange;
    }

    /*!
     * @brief This function returns a reference to member SecurityExchange
     * @return Reference to member SecurityExchange
     */
    eProsima_user_DllExport std::string& SecurityExchange()
    {
        return m_SecurityExchange;
    }


    /*!
     * @brief This function sets a value in member Side
     * @param _Side New value for member Side
     */
    eProsima_user_DllExport void Side(
            char _Side)
    {
        m_Side = _Side;
    }

    /*!
     * @brief This function returns the value of member Side
     * @return Value of member Side
     */
    eProsima_user_DllExport char Side() const
    {
        return m_Side;
    }

    /*!
     * @brief This function returns a reference to member Side
     * @return Reference to member Side
     */
    eProsima_user_DllExport char& Side()
    {
        return m_Side;
    }


    /*!
     * @brief This function sets a value in member TransactTime
     * @param _TransactTime New value for member TransactTime
     */
    eProsima_user_DllExport void TransactTime(
            uint64_t _TransactTime)
    {
        m_TransactTime = _TransactTime;
    }

    /*!
     * @brief This function returns the value of member TransactTime
     * @return Value of member TransactTime
     */
    eProsima_user_DllExport uint64_t TransactTime() const
    {
        return m_TransactTime;
    }

    /*!
     * @brief This function returns a reference to member TransactTime
     * @return Reference to member TransactTime
     */
    eProsima_user_DllExport uint64_t& TransactTime()
    {
        return m_TransactTime;
    }


    /*!
     * @brief This function sets a value in member OrderQty
     * @param _OrderQty New value for member OrderQty
     */
    eProsima_user_DllExport void OrderQty(
            int32_t _OrderQty)
    {
        m_OrderQty = _OrderQty;
    }

    /*!
     * @brief This function returns the value of member OrderQty
     * @return Value of member OrderQty
     */
    eProsima_user_DllExport int32_t OrderQty() const
    {
        return m_OrderQty;
    }

    /*!
     * @brief This function returns a reference to member OrderQty
     * @return Reference to member OrderQty
     */
    eProsima_user_DllExport int32_t& OrderQty()
    {
        return m_OrderQty;
    }


    /*!
     * @brief This function sets a value in member OrdType
     * @param _OrdType New value for member OrdType
     */
    eProsima_user_DllExport void OrdType(
            char _OrdType)
    {
        m_OrdType = _OrdType;
    }

    /*!
     * @brief This function returns the value of member OrdType
     * @return Value of member OrdType
     */
    eProsima_user_DllExport char OrdType() const
    {
        return m_OrdType;
    }

    /*!
     * @brief This function returns a reference to member OrdType
     * @return Reference to member OrdType
     */
    eProsima_user_DllExport char& OrdType()
    {
        return m_OrdType;
    }


    /*!
     * @brief This function sets a value in member Price
     * @param _Price New value for member Price
     */
    eProsima_user_DllExport void Price(
            float _Price)
    {
        m_Price = _Price;
    }

    /*!
     * @brief This function returns the value of member Price
     * @return Value of member Price
     */
    eProsima_user_DllExport float Price() const
    {
        return m_Price;
    }

    /*!
     * @brief This function returns a reference to member Price
     * @return Reference to member Price
     */
    eProsima_user_DllExport float& Price()
    {
        return m_Price;
    }


    /*!
     * @brief This function sets a value in member StopPx
     * @param _StopPx New value for member StopPx
     */
    eProsima_user_DllExport void StopPx(
            float _StopPx)
    {
        m_StopPx = _StopPx;
    }

    /*!
     * @brief This function returns the value of member StopPx
     * @return Value of member StopPx
     */
    eProsima_user_DllExport float StopPx() const
    {
        return m_StopPx;
    }

    /*!
     * @brief This function returns a reference to member StopPx
     * @return Reference to member StopPx
     */
    eProsima_user_DllExport float& StopPx()
    {
        return m_StopPx;
    }


    /*!
     * @brief This function sets a value in member TimeInForce
     * @param _TimeInForce New value for member TimeInForce
     */
    eProsima_user_DllExport void TimeInForce(
            char _TimeInForce)
    {
        m_TimeInForce = _TimeInForce;
    }

    /*!
     * @brief This function returns the value of member TimeInForce
     * @return Value of member TimeInForce
     */
    eProsima_user_DllExport char TimeInForce() const
    {
        return m_TimeInForce;
    }

    /*!
     * @brief This function returns a reference to member TimeInForce
     * @return Reference to member TimeInForce
     */
    eProsima_user_DllExport char& TimeInForce()
    {
        return m_TimeInForce;
    }


    /*!
     * @brief This function copies the value in member Text
     * @param _Text New value to be copied in member Text
     */
    eProsima_user_DllExport void Text(
            const std::string& _Text)
    {
        m_Text = _Text;
    }

    /*!
     * @brief This function moves the value in member Text
     * @param _Text New value to be moved in member Text
     */
    eProsima_user_DllExport void Text(
            std::string&& _Text)
    {
        m_Text = std::move(_Text);
    }

    /*!
     * @brief This function returns a constant reference to member Text
     * @return Constant reference to member Text
     */
    eProsima_user_DllExport const std::string& Text() const
    {
        return m_Text;
    }

    /*!
     * @brief This function returns a reference to member Text
     * @return Reference to member Text
     */
    eProsima_user_DllExport std::string& Text()
    {
        return m_Text;
    }



private:

    Header m_header;
    std::string m_OrigClOrdID;
    std::string m_ClOrdID;
    std::string m_ExecInst;
    std::string m_Symbol;
    std::string m_SecurityExchange;
    char m_Side{0};
    uint64_t m_TransactTime{0};
    int32_t m_OrderQty{0};
    char m_OrdType{0};
    float m_Price{0.0};
    float m_StopPx{0.0};
    char m_TimeInForce{0};
    std::string m_Text;

};

} // namespace OrderCancelReplaceRequest

#endif // _FAST_DDS_GENERATED_ORDERCANCELREPLACEREQUEST_ORDERCANCELREPLACEREQUEST_HPP_


