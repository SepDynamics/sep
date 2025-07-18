#ifndef SEP_CORE_METRICS_COLLECTOR_H
#define SEP_CORE_METRICS_COLLECTOR_H

#include <string>
#include <map>

namespace sep {
namespace core {

class MetricsCollector {
public:
    void increment(const std::string& metric_name, double value = 1.0);
    void set(const std::string& metric_name, double value);
    std::map<std::string, double> getMetrics() const;

private:
    std::map<std::string, double> metrics_;
};

} // namespace core
} // namespace sep

#endif // SEP_CORE_METRICS_COLLECTOR_H
