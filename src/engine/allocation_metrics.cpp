#include "allocation_metrics.h"

#include "metrics_collector.h"
#include "prometheus_exporter.h"
#include "types.h"

namespace sep::metrics {
namespace {
Counter g_allocation_failures{"allocation_failures_total", "Total memory allocation failures"};
bool registered = false; 
} // namespace

Counter& allocationFailures() {
    if (!registered) {
        PrometheusExporter::instance().registerCounter(&g_allocation_failures);
        registered = true;
    }
    return g_allocation_failures;
}

}  // namespace sep::metrics
