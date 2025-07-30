#pragma once

#include "engine/internal/standard_includes.h"
#include "prometheus_exporter.h"

namespace sep::metrics {

/// Global counter tracking memory allocation failures.
Counter& allocationFailures();

}  // namespace sep::metrics
