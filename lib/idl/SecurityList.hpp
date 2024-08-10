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
 * @file SecurityList.hpp
 * This header file contains the declaration of the described types in the IDL file.
 *
 * This file was generated by the tool fastddsgen.
 */

#ifndef FAST_DDS_GENERATED__SECURITYLIST_SECURITYLIST_HPP
#define FAST_DDS_GENERATED__SECURITYLIST_SECURITYLIST_HPP

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

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
#if defined(SECURITYLIST_SOURCE)
#define SECURITYLIST_DllAPI __declspec( dllexport )
#else
#define SECURITYLIST_DllAPI __declspec( dllimport )
#endif // SECURITYLIST_SOURCE
#else
#define SECURITYLIST_DllAPI
#endif  // EPROSIMA_USER_DLL_EXPORT
#else
#define SECURITYLIST_DllAPI
#endif // _WIN32

namespace SecurityList {

/*!
 * @brief This class represents the structure NoRelatedSym defined by the user in the IDL file.
 * @ingroup SecurityList
 */
class NoRelatedSym
{
public:

    /*!
     * @brief Default constructor.
     */
    eProsima_user_DllExport NoRelatedSym()
    {
    }

    /*!
     * @brief Default destructor.
     */
    eProsima_user_DllExport ~NoRelatedSym()
    {
    }

    /*!
     * @brief Copy constructor.
     * @param x Reference to the object NoRelatedSym that will be copied.
     */
    eProsima_user_DllExport NoRelatedSym(
            const NoRelatedSym& x)
    {
                    m_Symbol = x.m_Symbol;

                    m_SecurityExchange = x.m_SecurityExchange;

                    m_Text = x.m_Text;

    }

    /*!
     * @brief Move constructor.
     * @param x Reference to the object NoRelatedSym that will be copied.
     */
    eProsima_user_DllExport NoRelatedSym(
            NoRelatedSym&& x) noexcept
    {
        m_Symbol = std::move(x.m_Symbol);
        m_SecurityExchange = std::move(x.m_SecurityExchange);
        m_Text = std::move(x.m_Text);
    }

    /*!
     * @brief Copy assignment.
     * @param x Reference to the object NoRelatedSym that will be copied.
     */
    eProsima_user_DllExport NoRelatedSym& operator =(
            const NoRelatedSym& x)
    {

                    m_Symbol = x.m_Symbol;

                    m_SecurityExchange = x.m_SecurityExchange;

                    m_Text = x.m_Text;

        return *this;
    }

    /*!
     * @brief Move assignment.
     * @param x Reference to the object NoRelatedSym that will be copied.
     */
    eProsima_user_DllExport NoRelatedSym& operator =(
            NoRelatedSym&& x) noexcept
    {

        m_Symbol = std::move(x.m_Symbol);
        m_SecurityExchange = std::move(x.m_SecurityExchange);
        m_Text = std::move(x.m_Text);
        return *this;
    }

    /*!
     * @brief Comparison operator.
     * @param x NoRelatedSym object to compare.
     */
    eProsima_user_DllExport bool operator ==(
            const NoRelatedSym& x) const
    {
        return (m_Symbol == x.m_Symbol &&
           m_SecurityExchange == x.m_SecurityExchange &&
           m_Text == x.m_Text);
    }

    /*!
     * @brief Comparison operator.
     * @param x NoRelatedSym object to compare.
     */
    eProsima_user_DllExport bool operator !=(
            const NoRelatedSym& x) const
    {
        return !(*this == x);
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

    std::string m_Symbol;
    std::string m_SecurityExchange;
    std::string m_Text;

};
typedef std::vector<NoRelatedSym> NoRelatedSymSeq;

/*!
 * @brief This class represents the structure SecurityList defined by the user in the IDL file.
 * @ingroup SecurityList
 */
class SecurityList
{
public:

    /*!
     * @brief Default constructor.
     */
    eProsima_user_DllExport SecurityList()
    {
    }

    /*!
     * @brief Default destructor.
     */
    eProsima_user_DllExport ~SecurityList()
    {
    }

    /*!
     * @brief Copy constructor.
     * @param x Reference to the object SecurityList that will be copied.
     */
    eProsima_user_DllExport SecurityList(
            const SecurityList& x)
    {
                    m_header = x.m_header;

                    m_SecurityReqID = x.m_SecurityReqID;

                    m_SecurityResponseID = x.m_SecurityResponseID;

                    m_SecurityRequestResult = x.m_SecurityRequestResult;

                    m_c_NoRelatedSym = x.m_c_NoRelatedSym;

    }

    /*!
     * @brief Move constructor.
     * @param x Reference to the object SecurityList that will be copied.
     */
    eProsima_user_DllExport SecurityList(
            SecurityList&& x) noexcept
    {
        m_header = std::move(x.m_header);
        m_SecurityReqID = std::move(x.m_SecurityReqID);
        m_SecurityResponseID = std::move(x.m_SecurityResponseID);
        m_SecurityRequestResult = x.m_SecurityRequestResult;
        m_c_NoRelatedSym = std::move(x.m_c_NoRelatedSym);
    }

    /*!
     * @brief Copy assignment.
     * @param x Reference to the object SecurityList that will be copied.
     */
    eProsima_user_DllExport SecurityList& operator =(
            const SecurityList& x)
    {

                    m_header = x.m_header;

                    m_SecurityReqID = x.m_SecurityReqID;

                    m_SecurityResponseID = x.m_SecurityResponseID;

                    m_SecurityRequestResult = x.m_SecurityRequestResult;

                    m_c_NoRelatedSym = x.m_c_NoRelatedSym;

        return *this;
    }

    /*!
     * @brief Move assignment.
     * @param x Reference to the object SecurityList that will be copied.
     */
    eProsima_user_DllExport SecurityList& operator =(
            SecurityList&& x) noexcept
    {

        m_header = std::move(x.m_header);
        m_SecurityReqID = std::move(x.m_SecurityReqID);
        m_SecurityResponseID = std::move(x.m_SecurityResponseID);
        m_SecurityRequestResult = x.m_SecurityRequestResult;
        m_c_NoRelatedSym = std::move(x.m_c_NoRelatedSym);
        return *this;
    }

    /*!
     * @brief Comparison operator.
     * @param x SecurityList object to compare.
     */
    eProsima_user_DllExport bool operator ==(
            const SecurityList& x) const
    {
        return (m_header == x.m_header &&
           m_SecurityReqID == x.m_SecurityReqID &&
           m_SecurityResponseID == x.m_SecurityResponseID &&
           m_SecurityRequestResult == x.m_SecurityRequestResult &&
           m_c_NoRelatedSym == x.m_c_NoRelatedSym);
    }

    /*!
     * @brief Comparison operator.
     * @param x SecurityList object to compare.
     */
    eProsima_user_DllExport bool operator !=(
            const SecurityList& x) const
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
     * @brief This function copies the value in member SecurityResponseID
     * @param _SecurityResponseID New value to be copied in member SecurityResponseID
     */
    eProsima_user_DllExport void SecurityResponseID(
            const std::string& _SecurityResponseID)
    {
        m_SecurityResponseID = _SecurityResponseID;
    }

    /*!
     * @brief This function moves the value in member SecurityResponseID
     * @param _SecurityResponseID New value to be moved in member SecurityResponseID
     */
    eProsima_user_DllExport void SecurityResponseID(
            std::string&& _SecurityResponseID)
    {
        m_SecurityResponseID = std::move(_SecurityResponseID);
    }

    /*!
     * @brief This function returns a constant reference to member SecurityResponseID
     * @return Constant reference to member SecurityResponseID
     */
    eProsima_user_DllExport const std::string& SecurityResponseID() const
    {
        return m_SecurityResponseID;
    }

    /*!
     * @brief This function returns a reference to member SecurityResponseID
     * @return Reference to member SecurityResponseID
     */
    eProsima_user_DllExport std::string& SecurityResponseID()
    {
        return m_SecurityResponseID;
    }


    /*!
     * @brief This function sets a value in member SecurityRequestResult
     * @param _SecurityRequestResult New value for member SecurityRequestResult
     */
    eProsima_user_DllExport void SecurityRequestResult(
            int32_t _SecurityRequestResult)
    {
        m_SecurityRequestResult = _SecurityRequestResult;
    }

    /*!
     * @brief This function returns the value of member SecurityRequestResult
     * @return Value of member SecurityRequestResult
     */
    eProsima_user_DllExport int32_t SecurityRequestResult() const
    {
        return m_SecurityRequestResult;
    }

    /*!
     * @brief This function returns a reference to member SecurityRequestResult
     * @return Reference to member SecurityRequestResult
     */
    eProsima_user_DllExport int32_t& SecurityRequestResult()
    {
        return m_SecurityRequestResult;
    }


    /*!
     * @brief This function copies the value in member c_NoRelatedSym
     * @param _c_NoRelatedSym New value to be copied in member c_NoRelatedSym
     */
    eProsima_user_DllExport void c_NoRelatedSym(
            const NoRelatedSymSeq& _c_NoRelatedSym)
    {
        m_c_NoRelatedSym = _c_NoRelatedSym;
    }

    /*!
     * @brief This function moves the value in member c_NoRelatedSym
     * @param _c_NoRelatedSym New value to be moved in member c_NoRelatedSym
     */
    eProsima_user_DllExport void c_NoRelatedSym(
            NoRelatedSymSeq&& _c_NoRelatedSym)
    {
        m_c_NoRelatedSym = std::move(_c_NoRelatedSym);
    }

    /*!
     * @brief This function returns a constant reference to member c_NoRelatedSym
     * @return Constant reference to member c_NoRelatedSym
     */
    eProsima_user_DllExport const NoRelatedSymSeq& c_NoRelatedSym() const
    {
        return m_c_NoRelatedSym;
    }

    /*!
     * @brief This function returns a reference to member c_NoRelatedSym
     * @return Reference to member c_NoRelatedSym
     */
    eProsima_user_DllExport NoRelatedSymSeq& c_NoRelatedSym()
    {
        return m_c_NoRelatedSym;
    }



private:

    Header m_header;
    std::string m_SecurityReqID;
    std::string m_SecurityResponseID;
    int32_t m_SecurityRequestResult{0};
    NoRelatedSymSeq m_c_NoRelatedSym;

};

} // namespace SecurityList

#endif // _FAST_DDS_GENERATED_SECURITYLIST_SECURITYLIST_HPP_


