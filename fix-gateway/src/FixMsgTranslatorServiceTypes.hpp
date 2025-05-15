#pragma once

#include <functional>

class FixApplication; // From FixApplication.hpp.

template <typename T>
using ProcessorFunc = std::function<void(FixApplication &, T &)>;