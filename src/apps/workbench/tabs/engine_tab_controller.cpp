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
    
    // Check core components
    if (!backtester_) {
        std::cerr << "[EngineTabController] Error: backtester not created" << std::endl;
        return false;
    }
    
    if (!data_loader_) {
        std::cerr << "[EngineTabController] Error: data loader not created" << std::endl;
        return false;
    }
    
    // Log warnings for optional components
    if (!metrics_monitor_) {
        std::cerr << "[EngineTabController] Warning: metrics monitor not available" << std::endl;
    }
    
    if (!pattern_engine_) {
        std::cerr << "[EngineTabController] Warning: pattern engine not available" << std::endl;
    }
    
    if (!coherence_manager_) {
        std::cerr << "[EngineTabController] Warning: coherence manager not available" << std::endl;
    }
    
    if (!multi_timeframe_analyzer_) {
        std::cerr << "[EngineTabController] Warning: multi timeframe analyzer not available" << std::endl;
    }
    
    std::cout << "[EngineTabController] Initialization successful" << std::endl;
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
    if (engine && engine != reinterpret_cast<core::Engine*>(service_proxy_engine_)) {
        local_engine_ = engine;
    }
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
    if (use_remote_engine_ && service_proxy_engine_) {
        sep_engine_ = service_proxy_engine_;
    }
}

void EngineTabController::renderSEPMetricsPanel() {
    ImGui::Text("SEP Real-Time Metrics");
    ImGui::Separator();
    
    if (!metrics_monitor_) {
        ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "Error: Metrics monitor not available");
        ImGui::TextWrapped("The metrics monitoring system is not properly initialized. Check system initialization and component dependencies.");
        return;
    }

    try {
        const auto& system_metrics = metrics_monitor_->getSystemMetrics();
        const auto& rolling = metrics_monitor_->getRollingMetrics();

        // Color-coded metrics display
        ImGui::Text("Coherence: "); ImGui::SameLine();
        ImVec4 coherence_color = system_metrics.avg_coherence > 0.7f ? ImVec4(0.0f, 1.0f, 0.0f, 1.0f) :
                                system_metrics.avg_coherence > 0.4f ? ImVec4(1.0f, 1.0f, 0.0f, 1.0f) :
                                ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
        ImGui::TextColored(coherence_color, "%.3f", system_metrics.avg_coherence);
        ImGui::ProgressBar(system_metrics.avg_coherence, ImVec2(-1, 0));

        ImGui::Text("Stability: "); ImGui::SameLine();
        ImVec4 stability_color = system_metrics.avg_stability > 0.7f ? ImVec4(0.0f, 1.0f, 0.0f, 1.0f) :
                                system_metrics.avg_stability > 0.4f ? ImVec4(1.0f, 1.0f, 0.0f, 1.0f) :
                                ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
        ImGui::TextColored(stability_color, "%.3f", system_metrics.avg_stability);
        ImGui::ProgressBar(system_metrics.avg_stability, ImVec2(-1, 0));

        ImGui::Text("Entropy:   "); ImGui::SameLine();
        ImVec4 entropy_color = system_metrics.avg_entropy < 0.3f ? ImVec4(0.0f, 1.0f, 0.0f, 1.0f) :
                              system_metrics.avg_entropy < 0.6f ? ImVec4(1.0f, 1.0f, 0.0f, 1.0f) :
                              ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
        ImGui::TextColored(entropy_color, "%.3f", system_metrics.avg_entropy);
        ImGui::ProgressBar(1.0f - system_metrics.avg_entropy, ImVec2(-1, 0));

        // Rolling averages with tooltips
        if (ImGui::TreeNode("Rolling Averages")) {
            ImGui::Text("1h Averages:");
            ImGui::Text("  Coherence: %.3f", rolling.coherence_1h_avg);
            if (ImGui::IsItemHovered()) {
                ImGui::SetTooltip("1-hour rolling average of system coherence");
            }
            ImGui::Text("  Stability: %.3f", rolling.stability_1h_avg);
            if (ImGui::IsItemHovered()) {
                ImGui::SetTooltip("1-hour rolling average of system stability");
            }
            ImGui::Text("  Entropy: %.3f", rolling.entropy_1h_avg);
            if (ImGui::IsItemHovered()) {
                ImGui::SetTooltip("1-hour rolling average of system entropy");
            }

            ImGui::Text("4h Averages:");
            ImGui::Text("  Coherence: %.3f", rolling.coherence_4h_avg);
            if (ImGui::IsItemHovered()) {
                ImGui::SetTooltip("4-hour rolling average of system coherence");
            }
            ImGui::Text("  Stability: %.3f", rolling.stability_4h_avg);
            if (ImGui::IsItemHovered()) {
                ImGui::SetTooltip("4-hour rolling average of system stability");
            }
            ImGui::Text("  Entropy: %.3f", rolling.entropy_4h_avg);
            if (ImGui::IsItemHovered()) {
                ImGui::SetTooltip("4-hour rolling average of system entropy");
            }
            ImGui::TreePop();
        }

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

    } catch (const std::exception& e) {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Error retrieving metrics:");
        ImGui::TextWrapped("%s", e.what());
        return;
    }

    // Export section
    if (ImGui::CollapsingHeader("Export Options")) {
        ImGui::InputText("Metrics JSON Path", metrics_export_path_, sizeof(metrics_export_path_));
        if (ImGui::IsItemHovered()) {
            ImGui::SetTooltip("Specify path for metrics export (e.g., 'metrics/data.json')");
        }

        if (ImGui::Button("Export Metrics")) {
            std::string path_str(metrics_export_path_);
            if (path_str.empty()) {
                ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "Please specify an export path");
            } else {
                try {
                    std::filesystem::path export_path(path_str);
                    std::filesystem::path dir = export_path.parent_path();
                    
                    // Validate file extension
                    if (export_path.extension() != ".json") {
                        path_str += ".json";
                        export_path = std::filesystem::path(path_str);
                    }
                    
                    // Create directory if needed
                    if (!dir.empty() && !std::filesystem::exists(dir)) {
                        try {
                            if (!std::filesystem::create_directories(dir)) {
                                ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Failed to create directory");
                                return;
                            }
                        } catch (const std::filesystem::filesystem_error& e) {
                            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Directory creation error:");
                            ImGui::TextWrapped("%s", e.what());
                            return;
                        }
                    }
                    
                    // Check if file exists
                    if (std::filesystem::exists(export_path)) {
                        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "File exists. Overwriting...");
                    }
                    
                    metrics_monitor_->saveMetricsToFile(path_str);
                    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Metrics exported to: %s", path_str.c_str());
                } catch (const std::exception& e) {
                    ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Export failed:");
                    ImGui::TextWrapped("%s", e.what());
                }
            }
        }
        if (ImGui::IsItemHovered()) {
            ImGui::SetTooltip("Export current metrics to JSON file");
        }
    }
}

void EngineTabController::renderEngineControls() {
    ImGui::Text("Engine Controls:");

    bool connected = service_proxy_engine_ && service_proxy_engine_->isConnected();
    ImVec4 status_color = connected ? ImVec4(0.0f, 1.0f, 0.0f, 1.0f)
                                    : ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
    ImGui::TextColored(status_color, "Service Status: %s",
                       service_proxy_engine_ ? (connected ? "Connected" : "Disconnected")
                                             : "Offline");

    if (service_proxy_engine_) {
        ImGui::SameLine();
        if (ImGui::Checkbox("Use Remote Engine", &use_remote_engine_)) {
            if (use_remote_engine_ && connected) {
                sep_engine_ = service_proxy_engine_;
            } else if (local_engine_) {
                sep_engine_ = local_engine_;
            }
        }
    }

    ImGui::Text("Current Engine: %s", (sep_engine_ == service_proxy_engine_ && connected)
                                          ? "Remote"
                                          : "Local");

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
        ImGui::Text("Timestamp: %lu", market_data.timestamp);
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
        if (strlen(correlation_export_path_) == 0) {
            correlation_export_status_ = "Please specify an export path";
            return;
        }

        correlation_export_status_.clear();
        try {
            std::string path_str(correlation_export_path_);
            std::filesystem::path base_path(path_str);
            std::filesystem::path dir = base_path.parent_path();

            // Create directory if needed
            if (!dir.empty() && !std::filesystem::exists(dir)) {
                try {
                    if (!std::filesystem::create_directories(dir)) {
                        correlation_export_status_ = "Failed to create directory";
                        return;
                    }
                } catch (const std::filesystem::filesystem_error& e) {
                    correlation_export_status_ = std::string("Directory error: ") + e.what();
                    return;
                }
            }

            DataParser parser;
            
            // Validate data availability
            if (!multi_timeframe_analyzer_) {
                correlation_export_status_ = "Error: Analyzer not available";
                return;
            }

            auto history = multi_timeframe_analyzer_->getCorrelationHistory(selected_timeframe);
            if (history.empty()) {
                correlation_export_status_ = "Warning: No correlation history available";
                return;
            }

            // Convert and export metrics
            const sep::workbench::CorrelationMetrics& wb_metrics = correlation_metrics;
            ::sep::common::CorrelationMetrics common_metrics{
                wb_metrics.coherence_pearson,
                wb_metrics.coherence_spearman,
                wb_metrics.stability_pearson,
                wb_metrics.stability_spearman,
                wb_metrics.entropy_pearson,
                wb_metrics.entropy_spearman,
                wb_metrics.sample_count
            };

            std::map<std::string, ::sep::common::CorrelationMetrics> data{{selected_timeframe, common_metrics}};
            
            // Export files with proper extension handling
            std::string csv_path = base_path.extension().empty() ? path_str + ".csv" : path_str;
            std::string json_path = base_path.stem().string() + ".json";
            std::string bt_path = base_path.stem().string() + ".bt.csv";

            bool csv_ok = parser.exportCorrelationCSV(csv_path, data);
            bool json_ok = parser.exportCorrelationJSON(json_path, data);
            
            // Convert and export history
            std::deque<::sep::common::CorrelationMetrics> common_history;
            for (const auto& wb_metric : history) {
                common_history.push_back(::sep::common::CorrelationMetrics{
                    wb_metric.coherence_pearson,
                    wb_metric.coherence_spearman,
                    wb_metric.stability_pearson,
                    wb_metric.stability_spearman,
                    wb_metric.entropy_pearson,
                    wb_metric.entropy_spearman,
                    wb_metric.sample_count
                });
            }
            
            bool bt_ok = parser.exportCorrelationForBacktester(bt_path, common_history);

            if (!csv_ok || !json_ok || !bt_ok) {
                correlation_export_status_ = "Warning: Some files failed to export";
            } else {
                correlation_export_status_ = "Successfully exported:\n" + csv_path + "\n" + json_path + "\n" + bt_path;
            }
        } catch (const std::exception& e) {
            correlation_export_status_ = std::string("Export error: ") + e.what();
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
