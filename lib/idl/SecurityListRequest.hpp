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
 * @file SecurityListRequest.hpp
 * This header file contains the declaration of the described types in the IDL file.
 *
 * This file was generated by the tool fastddsgen.
 */

#ifndef FAST_DDS_GENERATED__SECURITYLISTREQUEST_SECURITYLISTREQUEST_HPP
#define FAST_DDS_GENERATED__SECURITYLISTREQUEST_SECURITYLISTREQUEST_HPP

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
#if defined(SECURITYLISTREQUEST_SOURCE)
#define SECURITYLISTREQUEST_DllAPI __declspec( dllexport )
#else
#define SECURITYLISTREQUEST_DllAPI __declspec( dllimport )
#endif // SECURITYLISTREQUEST_SOURCE
#else
#define SECURITYLISTREQUEST_DllAPI
#endif  // EPROSIMA_USER_DLL_EXPORT
#else
#define SECURITYLISTREQUEST_DllAPI
#endif // _WIN32

namespace SecurityListRequest {

/*!
 * @brief This class represents the structure SecurityListRequest defined by the user in the IDL file.
 * @ingroup SecurityListRequest
 */
class SecurityListRequest
{
public:

    /*!
     * @brief Default constructor.
     */
    eProsima_user_DllExport SecurityListRequest()
    {
    }

    /*!
     * @brief Default destructor.
     */
    eProsima_user_DllExport ~SecurityListRequest()
    {
    }

    /*!
     * @brief Copy constructor.
     * @param x Reference to the object SecurityListRequest that will be copied.
     */
    eProsima_user_DllExport SecurityListRequest(
            const SecurityListRequest& x)
    {
                    m_header = x.m_header;

                    m_SecurityReqID = x.m_SecurityReqID;

                    m_SecurityListRequestType = x.m_SecurityListRequestType;

                    m_Symbol = x.m_Symbol;

                    m_SecurityExchange = x.m_SecurityExchange;

                    m_Text = x.m_Text;

    }

    /*!
     * @brief Move constructor.
     * @param x Reference to the object SecurityListRequest that will be copied.
     */
    eProsima_user_DllExport SecurityListRequest(
            SecurityListRequest&& x) noexcept
    {
        m_header = std::move(x.m_header);
        m_SecurityReqID = std::move(x.m_SecurityReqID);
        m_SecurityListRequestType = x.m_SecurityListRequestType;
        m_Symbol = std::move(x.m_Symbol);
        m_SecurityExchange = std::move(x.m_SecurityExchange);
        m_Text = std::move(x.m_Text);
    }

    /*!
     * @brief Copy assignment.
     * @param x Reference to the object SecurityListRequest that will be copied.
     */
    eProsima_user_DllExport SecurityListRequest& operator =(
            const SecurityListRequest& x)
    {

                    m_header = x.m_header;

                    m_SecurityReqID = x.m_SecurityReqID;

                    m_SecurityListRequestType = x.m_SecurityListRequestType;

                    m_Symbol = x.m_Symbol;

                    m_SecurityExchange = x.m_SecurityExchange;

                    m_Text = x.m_Text;

        return *this;
    }

    /*!
     * @brief Move assignment.
     * @param x Reference to the object SecurityListRequest that will be copied.
     */
    eProsima_user_DllExport SecurityListRequest& operator =(
            SecurityListRequest&& x) noexcept
    {

        m_header = std::move(x.m_header);
        m_SecurityReqID = std::move(x.m_SecurityReqID);
        m_SecurityListRequestType = x.m_SecurityListRequestType;
        m_Symbol = std::move(x.m_Symbol);
        m_SecurityExchange = std::move(x.m_SecurityExchange);
        m_Text = std::move(x.m_Text);
        return *this;
    }

    /*!
     * @brief Comparison operator.
     * @param x SecurityListRequest object to compare.
     */
    eProsima_user_DllExport bool operator ==(
            const SecurityListRequest& x) const
    {
        return (m_header == x.m_header &&
           m_SecurityReqID == x.m_SecurityReqID &&
           m_SecurityListRequestType == x.m_SecurityListRequestType &&
           m_Symbol == x.m_Symbol &&
           m_SecurityExchange == x.m_SecurityExchange &&
           m_Text == x.m_Text);
    }

    /*!
     * @brief Comparison operator.
     * @param x SecurityListRequest object to compare.
     */
    eProsima_user_DllExport bool operator !=(
            const SecurityListRequest& x) const
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
     * @brief This function copies the value in member SecurityReqID
     * @param _SecurityReqID New value to be copied in member SecurityReqID
     */
    eProsima_user_DllExport void SecurityReqID(
            const std::string& _SecurityReqID)
    {
        m_SecurityReqID = _SecurityReqID;
    }

    /*!
     * @brief This function moves the value in member SecurityReqID
     * @param _SecurityReqID New value to be moved in member SecurityReqID
     */
    eProsima_user_DllExport void SecurityReqID(
            std::string&& _SecurityReqID)
    {
        m_SecurityReqID = std::move(_SecurityReqID);
    }

    /*!
     * @brief This function returns a constant reference to member SecurityReqID
     * @return Constant reference to member SecurityReqID
     */
    eProsima_user_DllExport const std::string& SecurityReqID() const
    {
        return m_SecurityReqID;
    }

    /*!
     * @brief This function returns a reference to member SecurityReqID
     * @return Reference to member SecurityReqID
     */
    eProsima_user_DllExport std::string& SecurityReqID()
    {
        return m_SecurityReqID;
    }


    /*!
     * @brief This function sets a value in member SecurityListRequestType
     * @param _SecurityListRequestType New value for member SecurityListRequestType
     */
    eProsima_user_DllExport void SecurityListRequestType(
            int32_t _SecurityListRequestType)
    {
        m_SecurityListRequestType = _SecurityListRequestType;
    }

    /*!
     * @brief This function returns the value of member SecurityListRequestType
     * @return Value of member SecurityListRequestType
     */
    eProsima_user_DllExport int32_t SecurityListRequestType() const
    {
        return m_SecurityListRequestType;
    }

    /*!
     * @brief This function returns a reference to member SecurityListRequestType
     * @return Reference to member SecurityListRequestType
     */
    eProsima_user_DllExport int32_t& SecurityListRequestType()
    {
        return m_SecurityListRequestType;
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
    std::string m_SecurityReqID;
    int32_t m_SecurityListRequestType{0};
    std::string m_Symbol;
    std::string m_SecurityExchange;
    std::string m_Text;

};

} // namespace SecurityListRequest

#endif // _FAST_DDS_GENERATED_SECURITYLISTREQUEST_SECURITYLISTREQUEST_HPP_


