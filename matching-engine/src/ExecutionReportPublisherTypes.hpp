#pragma once

#include <memory>

class ExecutionReportPublisher; // From ExecutionReportPublisher.hpp.
using ExecutionReportPublisherPtr = std::unique_ptr<ExecutionReportPublisher>;