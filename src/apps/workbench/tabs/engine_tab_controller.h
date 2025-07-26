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
#include "../backtester/backtester.h"
#include "../backtester/data_loader.h"

namespace sep::workbench {

class MultiTimeframeAnalyzer;

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
    void setMultiTimeframeAnalyzer(MultiTimeframeAnalyzer* analyzer);

private:
    std::shared_ptr<MetricsMonitor> metrics_monitor_;
    sep::core::Engine* sep_engine_ = nullptr;
    sep::quantum::PatternMetricEngine* pattern_engine_ = nullptr;
    sep::quantum::CoherenceManager* coherence_manager_ = nullptr;
    MultiTimeframeAnalyzer* multi_timeframe_analyzer_ = nullptr;

    // Rendering functions
    void renderSEPMetricsPanel();
    void renderEngineControls();
    void renderCorrelationPanel();
    void renderConnectorMetricsPanel();

    // Data retrieval
    std::vector<sep::quantum::PatternMetrics> getCurrentPatternMetrics();
    sep::quantum::CoherenceManager::CoherenceMetrics getCoherenceMetrics();
    void resetEngineState();

    // Export path buffer
    char correlation_export_path_[512] = "correlation.csv";

    // Strategy optimization
    std::unique_ptr<backtester::Backtester> backtester_;
    std::unique_ptr<backtester::DataLoader> data_loader_;
    char dataset_path_[512] = "";
    float opt_coherence_{0.7f};
    float opt_stability_{0.6f};
    float opt_entropy_{0.3f};
    backtester::BacktestResult last_result_{};

    void renderStrategyOptimization();
};

} // namespace sep::workbench
