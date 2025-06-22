#pragma once

#include "metrics/prometheus_exporter.h"

namespace sep::metrics {

/// Global counter tracking memory allocation failures.
Counter& allocationFailures();

}  // namespace sep::metrics
