#include "core/allocation_metrics.h"
#include "core/prometheus_exporter.h"
#include "metrics/types.h"

namespace {
Counter g_allocation_failures{"allocation_failures_total", "Total memory allocation failures"};
bool    registered = false;
}  // namespace

Counter& allocationFailures()
{
    if (!registered)
    {
        PrometheusExporter::instance().registerCounter(&g_allocation_failures);
        registered = true;
    }
    return g_allocation_failures;
}

}  // namespace sep::metrics
