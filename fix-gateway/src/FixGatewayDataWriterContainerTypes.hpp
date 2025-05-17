#pragma once

#include <memory>

struct DataWriterContainer; // From FixGatewayDataWriterContainer.hpp.
using DataWriterContainerPtr = std::unique_ptr<DataWriterContainer>;