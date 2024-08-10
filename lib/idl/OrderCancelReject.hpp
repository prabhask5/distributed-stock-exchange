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
 * @file OrderCancelReject.hpp
 * This header file contains the declaration of the described types in the IDL file.
 *
 * This file was generated by the tool fastddsgen.
 */

#ifndef FAST_DDS_GENERATED__ORDERCANCELREJECT_ORDERCANCELREJECT_HPP
#define FAST_DDS_GENERATED__ORDERCANCELREJECT_ORDERCANCELREJECT_HPP

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
#if defined(ORDERCANCELREJECT_SOURCE)
#define ORDERCANCELREJECT_DllAPI __declspec( dllexport )
#else
#define ORDERCANCELREJECT_DllAPI __declspec( dllimport )
#endif // ORDERCANCELREJECT_SOURCE
#else
#define ORDERCANCELREJECT_DllAPI
#endif  // EPROSIMA_USER_DLL_EXPORT
#else
#define ORDERCANCELREJECT_DllAPI
#endif // _WIN32

namespace OrderCancelReject {

/*!
 * @brief This class represents the structure OrderCancelReject defined by the user in the IDL file.
 * @ingroup OrderCancelReject
 */
class OrderCancelReject
{
public:

    /*!
     * @brief Default constructor.
     */
    eProsima_user_DllExport OrderCancelReject()
    {
    }

    /*!
     * @brief Default destructor.
     */
    eProsima_user_DllExport ~OrderCancelReject()
    {
    }

    /*!
     * @brief Copy constructor.
     * @param x Reference to the object OrderCancelReject that will be copied.
     */
    eProsima_user_DllExport OrderCancelReject(
            const OrderCancelReject& x)
    {
                    m_header = x.m_header;

                    m_OrderID = x.m_OrderID;

                    m_ClOrdID = x.m_ClOrdID;

                    m_OrigClOrdID = x.m_OrigClOrdID;

                    m_OrdStatus = x.m_OrdStatus;

                    m_TransactTime = x.m_TransactTime;

                    m_CxlRejResponseTo = x.m_CxlRejResponseTo;

                    m_Text = x.m_Text;

    }

    /*!
     * @brief Move constructor.
     * @param x Reference to the object OrderCancelReject that will be copied.
     */
    eProsima_user_DllExport OrderCancelReject(
            OrderCancelReject&& x) noexcept
    {
        m_header = std::move(x.m_header);
        m_OrderID = std::move(x.m_OrderID);
        m_ClOrdID = std::move(x.m_ClOrdID);
        m_OrigClOrdID = std::move(x.m_OrigClOrdID);
        m_OrdStatus = x.m_OrdStatus;
        m_TransactTime = x.m_TransactTime;
        m_CxlRejResponseTo = x.m_CxlRejResponseTo;
        m_Text = std::move(x.m_Text);
    }

    /*!
     * @brief Copy assignment.
     * @param x Reference to the object OrderCancelReject that will be copied.
     */
    eProsima_user_DllExport OrderCancelReject& operator =(
            const OrderCancelReject& x)
    {

                    m_header = x.m_header;

                    m_OrderID = x.m_OrderID;

                    m_ClOrdID = x.m_ClOrdID;

                    m_OrigClOrdID = x.m_OrigClOrdID;

                    m_OrdStatus = x.m_OrdStatus;

                    m_TransactTime = x.m_TransactTime;

                    m_CxlRejResponseTo = x.m_CxlRejResponseTo;

                    m_Text = x.m_Text;

        return *this;
    }

    /*!
     * @brief Move assignment.
     * @param x Reference to the object OrderCancelReject that will be copied.
     */
    eProsima_user_DllExport OrderCancelReject& operator =(
            OrderCancelReject&& x) noexcept
    {

        m_header = std::move(x.m_header);
        m_OrderID = std::move(x.m_OrderID);
        m_ClOrdID = std::move(x.m_ClOrdID);
        m_OrigClOrdID = std::move(x.m_OrigClOrdID);
        m_OrdStatus = x.m_OrdStatus;
        m_TransactTime = x.m_TransactTime;
        m_CxlRejResponseTo = x.m_CxlRejResponseTo;
        m_Text = std::move(x.m_Text);
        return *this;
    }

    /*!
     * @brief Comparison operator.
     * @param x OrderCancelReject object to compare.
     */
    eProsima_user_DllExport bool operator ==(
            const OrderCancelReject& x) const
    {
        return (m_header == x.m_header &&
           m_OrderID == x.m_OrderID &&
           m_ClOrdID == x.m_ClOrdID &&
           m_OrigClOrdID == x.m_OrigClOrdID &&
           m_OrdStatus == x.m_OrdStatus &&
           m_TransactTime == x.m_TransactTime &&
           m_CxlRejResponseTo == x.m_CxlRejResponseTo &&
           m_Text == x.m_Text);
    }

    /*!
     * @brief Comparison operator.
     * @param x OrderCancelReject object to compare.
     */
    eProsima_user_DllExport bool operator !=(
            const OrderCancelReject& x) const
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
     * @brief This function copies the value in member OrderID
     * @param _OrderID New value to be copied in member OrderID
     */
    eProsima_user_DllExport void OrderID(
            const std::string& _OrderID)
    {
        m_OrderID = _OrderID;
    }

    /*!
     * @brief This function moves the value in member OrderID
     * @param _OrderID New value to be moved in member OrderID
     */
    eProsima_user_DllExport void OrderID(
            std::string&& _OrderID)
    {
        m_OrderID = std::move(_OrderID);
    }

    /*!
     * @brief This function returns a constant reference to member OrderID
     * @return Constant reference to member OrderID
     */
    eProsima_user_DllExport const std::string& OrderID() const
    {
        return m_OrderID;
    }

    /*!
     * @brief This function returns a reference to member OrderID
     * @return Reference to member OrderID
     */
    eProsima_user_DllExport std::string& OrderID()
    {
        return m_OrderID;
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
     * @brief This function sets a value in member OrdStatus
     * @param _OrdStatus New value for member OrdStatus
     */
    eProsima_user_DllExport void OrdStatus(
            char _OrdStatus)
    {
        m_OrdStatus = _OrdStatus;
    }

    /*!
     * @brief This function returns the value of member OrdStatus
     * @return Value of member OrdStatus
     */
    eProsima_user_DllExport char OrdStatus() const
    {
        return m_OrdStatus;
    }

    /*!
     * @brief This function returns a reference to member OrdStatus
     * @return Reference to member OrdStatus
     */
    eProsima_user_DllExport char& OrdStatus()
    {
        return m_OrdStatus;
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
     * @brief This function sets a value in member CxlRejResponseTo
     * @param _CxlRejResponseTo New value for member CxlRejResponseTo
     */
    eProsima_user_DllExport void CxlRejResponseTo(
            char _CxlRejResponseTo)
    {
        m_CxlRejResponseTo = _CxlRejResponseTo;
    }

    /*!
     * @brief This function returns the value of member CxlRejResponseTo
     * @return Value of member CxlRejResponseTo
     */
    eProsima_user_DllExport char CxlRejResponseTo() const
    {
        return m_CxlRejResponseTo;
    }

    /*!
     * @brief This function returns a reference to member CxlRejResponseTo
     * @return Reference to member CxlRejResponseTo
     */
    eProsima_user_DllExport char& CxlRejResponseTo()
    {
        return m_CxlRejResponseTo;
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
    std::string m_OrderID;
    std::string m_ClOrdID;
    std::string m_OrigClOrdID;
    char m_OrdStatus{0};
    uint64_t m_TransactTime{0};
    char m_CxlRejResponseTo{0};
    std::string m_Text;

};

} // namespace OrderCancelReject

#endif // _FAST_DDS_GENERATED_ORDERCANCELREJECT_ORDERCANCELREJECT_HPP_


