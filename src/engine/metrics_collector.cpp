#include "metrics_collector.h"

namespace sep {
namespace core {

    void MetricsCollector::increment(const shim::string& metric_name, double value)
    {
        metrics_[metric_name] += value;
    }

void MetricsCollector::set(const shim::string& metric_name, double value)
{
    metrics_[metric_name] = value;
}

std::map<shim::string, double> MetricsCollector::getMetrics() const { return metrics_; }

} // namespace core
} // namespace sep
