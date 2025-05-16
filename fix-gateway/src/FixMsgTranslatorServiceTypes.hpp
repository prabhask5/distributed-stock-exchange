#pragma once

#include <functional>

class FixApplication; // From FixApplication.hpp.

template <typename T>
using TranslatorFunc = std::function<void(FixApplication &, T &)>;