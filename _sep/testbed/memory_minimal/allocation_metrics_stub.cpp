#include "core/allocation_metrics.h"

namespace sep::metrics {
Counter& allocationFailures() {
    static Counter stub{"allocation_failures_total", "Total memory allocation failures"};
    return stub;
}
}
