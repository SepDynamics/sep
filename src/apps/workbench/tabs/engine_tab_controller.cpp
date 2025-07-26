#include "engine_tab_controller.h"
#include "core/multi_timeframe_analyzer.h"
#include "engine/data_parser.h"
#include <map>

#include <iostream>
#include <filesystem>

#include "imgui.h"

namespace sep::workbench {

EngineTabController::EngineTabController()
    : backtester_(std::make_unique<backtester::Backtester>()),
      data_loader_(std::make_unique<backtester::DataLoader>()) {}

EngineTabController::~EngineTabController() { shutdown(); }
bool EngineTabController::initialize() {
    std::cout << "[EngineTabController] Initializing..." << std::endl;
    return true;
}

void EngineTabController::render() {
    if (ImGui::BeginTabItem("SEP Engine")) {
        ImGui::Columns(2);
        renderSEPMetricsPanel();
        ImGui::NextColumn();
        renderConnectorMetricsPanel();
        ImGui::Columns(1);

        renderEngineControls();
        renderCorrelationPanel();
        renderStrategyOptimization();
        ImGui::EndTabItem();
    }
}

void EngineTabController::shutdown() {}

void EngineTabController::setMetricsMonitor(std::shared_ptr<MetricsMonitor> monitor) {
    metrics_monitor_ = monitor;
}

void EngineTabController::setSEPEngine(core::Engine* engine) {
    sep_engine_ = engine;
}

void EngineTabController::setPatternMetricEngine(quantum::PatternMetricEngine* pattern_engine) {
    pattern_engine_ = pattern_engine;
}

void EngineTabController::setCoherenceManager(quantum::CoherenceManager* coherence_manager) {
    coherence_manager_ = coherence_manager;
}

void EngineTabController::setMultiTimeframeAnalyzer(MultiTimeframeAnalyzer* analyzer) {
    multi_timeframe_analyzer_ = analyzer;
}

void EngineTabController::setServiceProxyEngine(core::ServiceProxyEngine* engine) {
    service_proxy_engine_ = engine;
}

void EngineTabController::renderSEPMetricsPanel() {
    ImGui::Text("SEP Real-Time Metrics");
    ImGui::Separator();
    
    if (!metrics_monitor_) {
        ImGui::Text("Metrics monitor not available.");
        return;
    }

    const auto& system_metrics = metrics_monitor_->getSystemMetrics();
    const auto& rolling = metrics_monitor_->getRollingMetrics();

    ImGui::Text("Coherence: %.3f", system_metrics.avg_coherence);
    ImGui::ProgressBar(system_metrics.avg_coherence, ImVec2(-1, 0));

    ImGui::Text("Stability: %.3f", system_metrics.avg_stability);
    ImGui::ProgressBar(system_metrics.avg_stability, ImVec2(-1, 0));

    ImGui::Text("Entropy:   %.3f", system_metrics.avg_entropy);
    ImGui::ProgressBar(1.0f - system_metrics.avg_entropy, ImVec2(-1, 0));

    ImGui::Text("1h Avg Coherence: %.3f", rolling.coherence_1h_avg);
    ImGui::Text("1h Avg Stability: %.3f", rolling.stability_1h_avg);
    ImGui::Text("1h Avg Entropy: %.3f", rolling.entropy_1h_avg);

    ImGui::Text("4h Avg Coherence: %.3f", rolling.coherence_4h_avg);
    ImGui::Text("4h Avg Stability: %.3f", rolling.stability_4h_avg);
    ImGui::Text("4h Avg Entropy: %.3f", rolling.entropy_4h_avg);

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Text("Engine Status:");

    auto pattern_metrics = getCurrentPatternMetrics();
    ImGui::Text("Active Patterns: %zu", pattern_metrics.size());

    if (!pattern_metrics.empty()) {
        float avg_coherence = 0.0f, avg_stability = 0.0f, avg_entropy = 0.0f;
        for (const auto& metric : pattern_metrics) {
            avg_coherence += metric.coherence;
            avg_stability += metric.stability;
            avg_entropy += metric.entropy;
        }
        size_t count = pattern_metrics.size();
        avg_coherence /= count;
        avg_stability /= count;
        avg_entropy /= count;

        ImGui::Text("Avg Pattern Coherence: %.3f", avg_coherence);
        ImGui::Text("Avg Pattern Stability: %.3f", avg_stability);
        ImGui::Text("Avg Pattern Entropy: %.3f", avg_entropy);
    }

    auto coherence_metrics = getCoherenceMetrics();
    if (coherence_metrics.total_patterns > 0) {
        ImGui::Spacing();
        ImGui::Text("Coherence Manager:");
        ImGui::Text("Global Coherence: %.3f", coherence_metrics.global_coherence);
        ImGui::Text("Total Patterns: %lu", coherence_metrics.total_patterns);
        ImGui::Text("Coherent Patterns: %lu", coherence_metrics.coherent_patterns);
        ImGui::Text("Memory Pressure: %.3f", coherence_metrics.memory_pressure);
        ImGui::Text("Entanglement Density: %.3f", coherence_metrics.entanglement_density);

        ImGui::Text("Memory Tiers:");
        for (int i = 0; i < 3; i++) {
            ImGui::Text("  Tier %d: %.3f coherence, %.3f fragmentation",
                       i, coherence_metrics.tier_coherence[i], coherence_metrics.tier_fragmentation[i]);
        }
    }

    ImGui::InputText("Metrics JSON Path", metrics_export_path_, sizeof(metrics_export_path_));
    if (ImGui::Button("Export Metrics")) {
        metrics_monitor_->saveMetricsToFile(metrics_export_path_);
    }
}

void EngineTabController::renderEngineControls() {
    ImGui::Text("Engine Controls:");

    if (ImGui::Button("Reset Engine State")) {
        resetEngineState();
    }
    ImGui::SameLine();
    if (ImGui::Button("Force Pattern Evolution")) {
        if (pattern_engine_) {
            pattern_engine_->evolvePatterns();
        }
    }
}

std::vector<quantum::PatternMetrics> EngineTabController::getCurrentPatternMetrics() {
    if (!pattern_engine_) {
        return {};
    }
    try {
        return pattern_engine_->computeMetrics();
    } catch (const std::exception& e) {
        std::cerr << "[EngineTabController] Error getting pattern metrics: " << e.what() << std::endl;
        return {};
    }
}

quantum::CoherenceManager::CoherenceMetrics EngineTabController::getCoherenceMetrics() {
    if (!coherence_manager_) {
        return {};
    }
    try {
        return coherence_manager_->getMetrics();
    } catch (const std::exception& e) {
        std::cerr << "[EngineTabController] Error getting coherence metrics: " << e.what() << std::endl;
        return {};
    }
}

void EngineTabController::resetEngineState() {
    std::cout << "[EngineTabController] Resetting SEP Engine state..." << std::endl;
    try {
        if (pattern_engine_) {
            pattern_engine_->clear();
        }
        if (metrics_monitor_) {
            auto metrics = metrics_monitor_->getMetrics();
            for (const auto& [key, value] : metrics) {
                metrics_monitor_->set(key, 0.0);
            }
        }
        std::cout << "[EngineTabController] Engine state reset complete" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "[EngineTabController] Error resetting engine state: " << e.what() << std::endl;
    }
}

void EngineTabController::renderConnectorMetricsPanel() {
    ImGui::BeginChild("Connector Metrics", ImVec2(0, 250), true);
    ImGui::Text("Connector & Market Metrics");
    ImGui::Separator();

    if (metrics_monitor_) {
        auto market_data = metrics_monitor_->getLatestMarketData();
        ImGui::Text("Instrument: %s", market_data.instrument.c_str());
        ImGui::Text("Timestamp: %llu", market_data.timestamp);
        ImGui::Text("Bid: %.5f", market_data.bid);
        ImGui::Text("Ask: %.5f", market_data.ask);
        ImGui::Text("Spread: %.5f", market_data.spread);
        ImGui::Text("ATR (H1): %.5f", market_data.atr);
        ImGui::Text("Volatility Level: %d", market_data.volatility_level);
    } else {
        ImGui::Text("Metrics monitor not available.");
    }

    ImGui::EndChild();
}

void EngineTabController::renderCorrelationPanel() {
    ImGui::NextColumn();
    ImGui::Begin("Correlation Analysis");

    if (!multi_timeframe_analyzer_) {
        ImGui::Text("MultiTimeframeAnalyzer not available.");
        ImGui::End();
        return;
    }

    static std::string selected_timeframe = "1m";
    if (ImGui::BeginCombo("Timeframe", selected_timeframe.c_str())) {
        for (const auto& tf : multi_timeframe_analyzer_->getActiveTimeframes()) {
            bool is_selected = (selected_timeframe == tf);
            if (ImGui::Selectable(tf.c_str(), is_selected)) {
                selected_timeframe = tf;
            }
            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    auto correlation_metrics = multi_timeframe_analyzer_->calculateCorrelationMetrics(selected_timeframe);

    ImGui::Text("Correlation Coefficients");
    ImGui::Columns(3);
    ImGui::Text("Metric"); ImGui::NextColumn();
    ImGui::Text("Pearson"); ImGui::NextColumn();
    ImGui::Text("Spearman"); ImGui::NextColumn();
    ImGui::Separator();

    ImGui::Text("Coherence"); ImGui::NextColumn();
    ImGui::Text("%.3f", correlation_metrics.coherence_pearson); ImGui::NextColumn();
    ImGui::Text("%.3f", correlation_metrics.coherence_spearman); ImGui::NextColumn();

    ImGui::Text("Stability"); ImGui::NextColumn();
    ImGui::Text("%.3f", correlation_metrics.stability_pearson); ImGui::NextColumn();
    ImGui::Text("%.3f", correlation_metrics.stability_spearman); ImGui::NextColumn();

    ImGui::Text("Entropy"); ImGui::NextColumn();
    ImGui::Text("%.3f", correlation_metrics.entropy_pearson); ImGui::NextColumn();
    ImGui::Text("%.3f", correlation_metrics.entropy_spearman); ImGui::NextColumn();
    ImGui::Columns(1);
    ImGui::Text("Samples: %d", correlation_metrics.sample_count);

    // Plot correlation history
    auto history = multi_timeframe_analyzer_->getCorrelationHistory(selected_timeframe);
    std::vector<float> coh_p, coh_s, stab_p, stab_s, ent_p, ent_s;
    for (const auto& h : history) {
        coh_p.push_back(static_cast<float>(h.coherence_pearson));
        coh_s.push_back(static_cast<float>(h.coherence_spearman));
        stab_p.push_back(static_cast<float>(h.stability_pearson));
        stab_s.push_back(static_cast<float>(h.stability_spearman));
        ent_p.push_back(static_cast<float>(h.entropy_pearson));
        ent_s.push_back(static_cast<float>(h.entropy_spearman));
    }
    const ImVec2 graph_size(200, 60);
    ImGui::PlotLines("Coh Pearson", coh_p.data(), coh_p.size(), 0, nullptr, -1.0f, 1.0f, graph_size);
    ImGui::PlotLines("Coh Spearman", coh_s.data(), coh_s.size(), 0, nullptr, -1.0f, 1.0f, graph_size);
    ImGui::PlotLines("Stab Pearson", stab_p.data(), stab_p.size(), 0, nullptr, -1.0f, 1.0f, graph_size);
    ImGui::PlotLines("Stab Spearman", stab_s.data(), stab_s.size(), 0, nullptr, -1.0f, 1.0f, graph_size);
    ImGui::PlotLines("Ent Pearson", ent_p.data(), ent_p.size(), 0, nullptr, -1.0f, 1.0f, graph_size);
    ImGui::PlotLines("Ent Spearman", ent_s.data(), ent_s.size(), 0, nullptr, -1.0f, 1.0f, graph_size);

    ImGui::InputText("Export Path", correlation_export_path_, sizeof(correlation_export_path_));
    if (ImGui::Button("Export")) {
        correlation_export_status_.clear();
        std::string path_str(correlation_export_path_);
        std::filesystem::path csv_path(path_str);
        std::filesystem::path dir = csv_path.parent_path();
        bool ok = true;
        if (!dir.empty() && !std::filesystem::exists(dir)) {
            try {
                std::filesystem::create_directories(dir);
            } catch (const std::exception& e) {
                std::cerr << "[EngineTabController] Failed to create directory: " << e.what() << std::endl;
                correlation_export_status_ = "Failed to create directory";
                ok = false;
            }
        }
        if (ok) {
            DataParser parser;
            std::map<std::string, workbench::CorrelationMetrics> data{{selected_timeframe, correlation_metrics}};
            bool csv_ok = parser.exportCorrelationCSV(path_str, data);
            bool json_ok = parser.exportCorrelationJSON(path_str + ".json", data);
            auto history = multi_timeframe_analyzer_->getCorrelationHistory(selected_timeframe);
            parser.exportCorrelationForBacktester(path_str + ".bt.csv", history);
            if (!csv_ok || !json_ok) {
                std::cerr << "[EngineTabController] Failed to export correlation metrics" << std::endl;
                correlation_export_status_ = "Export failed";
            } else {
                correlation_export_status_ = "Export successful";
            }
        }
    }
    if (!correlation_export_status_.empty()) {
        ImGui::Text("%s", correlation_export_status_.c_str());
    }

    ImGui::End();
}

void EngineTabController::renderStrategyOptimization()
{
    ImGui::Begin("Strategy Optimization");
    ImGui::InputText("Dataset", dataset_path_, sizeof(dataset_path_));
    if (ImGui::Button("Run Backtest")) {
        if (dataset_path_[0] != '\0') {
            if (service_proxy_engine_) {
                float coh, stab, ent;
                last_result_ = service_proxy_engine_->optimize_strategy(dataset_path_, coh, stab, ent);
                opt_coherence_ = coh;
                opt_stability_ = stab;
                opt_entropy_ = ent;
            } else if (pattern_engine_) {
                data_loader_->load_data(dataset_path_);
                backtester_->run(pattern_engine_, data_loader_.get());
                last_result_ = backtester_->getResult();
                opt_coherence_ = 0.5f + last_result_.win_rate * 0.5f;
            }
        }
    }
    ImGui::Text("Win Rate: %.2f", last_result_.win_rate);
    ImGui::SliderFloat("Coherence", &opt_coherence_, 0.0f, 1.0f);
    ImGui::SliderFloat("Stability", &opt_stability_, 0.0f, 1.0f);
    ImGui::SliderFloat("Entropy", &opt_entropy_, 0.0f, 1.0f);
    if (ImGui::Button("Apply Thresholds")) {
        if (pattern_engine_) {
            quantum::SignalThresholds th;
            th.buy_min_coherence = opt_coherence_;
            th.buy_min_stability = opt_stability_;
            th.buy_max_entropy = opt_entropy_;
            pattern_engine_->setSignalThresholds(th);
        }
    }
    ImGui::End();
}

} // namespace sep::workbench
