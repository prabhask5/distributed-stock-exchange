#pragma once

#include <map>
#include <quickfix/SessionFactory.h>

namespace FIX {
class SessionSettings;
class SessionFactory;
class Dictionary;
class SessionID;
}; // namespace FIX

using UserMap = std::map<std::string, FIX::SessionID>;

class FixSocketConnection; // From FixSocketConnection.hpp.
using PendingLogonSocketConnectionMap =
    std::map<std::string, FixSocketConnection *>;

using FixSettingsPtr = std::unique_ptr<FIX::SessionSettings>;
using FixSessionFactoryPtr = std::unique_ptr<FIX::SessionFactory>;
using FixDictionaryPtr = std::unique_ptr<FIX::Dictionary>;

class AuthHelper; // From AuthHelper.hpp.
using AuthHelperPtr = std::unique_ptr<AuthHelper>;