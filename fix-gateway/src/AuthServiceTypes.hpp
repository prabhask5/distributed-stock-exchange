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

using FixSessionSettingsPtr = std::shared_ptr<FIX::SessionSettings>;
using FixSessionFactoryPtr = std::shared_ptr<FIX::SessionFactory>;
using FixDictionaryPtr = std::shared_ptr<FIX::Dictionary>;