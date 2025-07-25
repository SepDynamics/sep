#pragma once

#include <deque>
#include <memory>
#include <string>
#include <vector>

#include "core/common_structs.h"
#include "core/metrics_monitor.h"
#include "engine/engine.h"
#include "quantum/coherence_manager.h"
#include "quantum/pattern_metric_engine.h"

namespace sep::workbench {

class EngineTabController {
public:
    EngineTabController();
    ~EngineTabController();

    bool initialize();
    void render();
    void shutdown();

    void setMetricsMonitor(std::shared_ptr<MetricsMonitor> monitor);
    void setSEPEngine(sep::core::Engine* engine);
    void setPatternMetricEngine(sep::quantum::PatternMetricEngine* pattern_engine);
    void setCoherenceManager(sep::quantum::CoherenceManager* coherence_manager);

private:
    std::shared_ptr<MetricsMonitor> metrics_monitor_;
    sep::core::Engine* sep_engine_ = nullptr;
    sep::quantum::PatternMetricEngine* pattern_engine_ = nullptr;
    sep::quantum::CoherenceManager* coherence_manager_ = nullptr;

    // Rendering functions
    void renderSEPMetricsPanel();
    void renderEngineControls();

    // Data retrieval
    std::vector<sep::quantum::PatternMetrics> getCurrentPatternMetrics();
    sep::quantum::CoherenceManager::CoherenceMetrics getCoherenceMetrics();
    void resetEngineState();
};

} // namespace sep::workbench
