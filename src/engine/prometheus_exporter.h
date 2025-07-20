#pragma once

#include <atomic>
#include <cstdint>
#include <mutex>
#include <string>
#include <vector>

#include "engine/shim.h"

namespace sep::metrics {

struct Counter {
    shim::string name;
    shim::string help;
    std::atomic<uint64_t> value{0};
};

struct Gauge {
    shim::string name;
    shim::string help;
    std::atomic<double> value{0.0};
};

class PrometheusExporter {
public:
    static PrometheusExporter &instance();

    void registerCounter(Counter *counter);
    void registerGauge(Gauge *gauge);
    shim::string exportMetrics();

private:
    PrometheusExporter() = default;
    shim::vector<Counter *> counters_;
    shim::vector<Gauge *> gauges_;
    mutable std::mutex mutex_;
};

} // namespace sep::metrics
