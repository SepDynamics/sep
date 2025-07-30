#include "prometheus_exporter.h"

// Standard library includes
#include <sstream>

namespace sep::metrics {

PrometheusExporter &PrometheusExporter::instance() {
  static PrometheusExporter inst;
  return inst;
}

void PrometheusExporter::registerCounter(Counter *counter) {
  if (!counter) return;
  std::lock_guard<std::mutex> lock(mutex_);
  
  counters_.push_back(counter); 
}

void PrometheusExporter::registerGauge(Gauge *gauge) {
  if (!gauge) return;
  std::lock_guard<std::mutex> lock(mutex_);
  gauges_.push_back(gauge);
}

std::string PrometheusExporter::exportMetrics()
{
    std::lock_guard<std::mutex> lock(mutex_);
    std::ostringstream oss;
    for (const auto *c : counters_)
    {
        oss << "# HELP " << c->name << ' ' << c->help << "\n";
        oss << "# TYPE " << c->name << " counter\n";
        oss << c->name << ' ' << c->value.load() << "\n";
    }
    for (const auto *g : gauges_)
    {
        oss << "# HELP " << g->name << ' ' << g->help << "\n";
        oss << "# TYPE " << g->name << " gauge\n";
        oss << g->name << ' ' << g->value.load() << "\n";
    }
    return oss.str();
}

}  // namespace sep::metrics
