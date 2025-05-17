#pragma once

#include <map>
#include <quickfix/SessionSettings.h>

using UserMap = std::map<std::string, FIX::SessionID>;

class FixSocketConnection; // From FixSocketConnection.hpp.
using PendingLogonSocketConnectionMap =
    std::map<std::string, FixSocketConnection *>;

namespace FIX {
class SessionSettings;
class SessionFactory;
class Dictionary;
}; // namespace FIX

using FixSettingsPtr = std::unique_ptr<FIX::SessionSettings>;
using FixSessionFactoryPtr = std::unique_ptr<FIX::SessionFactory>;
using FixDictionaryPtr = std::unique_ptr<FIX::Dictionary>;

class AuthService; // From AuthService.hpp.
using AuthServicePtr = std::unique_ptr<AuthService>;