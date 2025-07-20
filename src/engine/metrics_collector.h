#ifndef SEP_CORE_METRICS_COLLECTOR_H
#define SEP_CORE_METRICS_COLLECTOR_H

#include <map>
#include <string>

#include "engine/shim.h"

namespace sep {
namespace core {

class MetricsCollector {
public:
    void increment(const shim::string& metric_name, double value = 1.0);
    void set(const shim::string& metric_name, double value);
    std::map<shim::string, double> getMetrics() const;

private:
    std::map<shim::string, double> metrics_;
};

} // namespace core
} // namespace sep

#endif // SEP_CORE_METRICS_COLLECTOR_H
