#pragma once

#include <atomic>
#include <cstdint>
#include <mutex>
#include <string>
#include <vector>

#include "engine/internal/standard_includes.h"

namespace sep::metrics {

struct Counter {
    std::string name;
    std::string help;
    std::atomic<uint64_t> value{0};
};

struct Gauge {
    std::string name;
    std::string help;
    std::atomic<double> value{0.0};
};

class PrometheusExporter {
public:
    static PrometheusExporter &instance();

    void registerCounter(Counter *counter);
    void registerGauge(Gauge *gauge);
    std::string exportMetrics();

private:
    PrometheusExporter() = default;
    std::vector<Counter *> counters_;
    std::vector<Gauge *> gauges_;
    mutable std::mutex mutex_;
};

} // namespace sep::metrics
