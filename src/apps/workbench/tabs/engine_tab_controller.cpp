#include "engine_tab_controller.h"
#include <imgui.h>
#include <iostream>

namespace sep::workbench {

EngineTabController::EngineTabController() {}

EngineTabController::~EngineTabController() {
    shutdown();
}

bool EngineTabController::initialize() {
    std::cout << "[EngineTabController] Initializing..." << std::endl;
    return true;
}

void EngineTabController::render() {
    renderSEPMetricsPanel();
    ImGui::Separator();
    renderEngineControls();
}

void EngineTabController::shutdown() {}

void EngineTabController::setMetricsMonitor(std::shared_ptr<MetricsMonitor> monitor) {
    metrics_monitor_ = monitor;
}

void EngineTabController::setSEPEngine(sep::core::Engine* engine) {
    sep_engine_ = engine;
}

void EngineTabController::setPatternMetricEngine(sep::quantum::PatternMetricEngine* pattern_engine) {
    pattern_engine_ = pattern_engine;
}

void EngineTabController::setCoherenceManager(sep::quantum::CoherenceManager* coherence_manager) {
    coherence_manager_ = coherence_manager;
}

void EngineTabController::renderSEPMetricsPanel() {
    ImGui::Text("SEP Real-Time Metrics");
    ImGui::Separator();
    
    if (!metrics_monitor_) {
        ImGui::Text("Metrics monitor not available.");
        return;
    }

    const auto& system_metrics = metrics_monitor_->getSystemMetrics();

    ImGui::Text("Coherence: %.3f", system_metrics.avg_coherence);
    ImGui::ProgressBar(system_metrics.avg_coherence, ImVec2(-1, 0));

    ImGui::Text("Stability: %.3f", system_metrics.avg_stability);
    ImGui::ProgressBar(system_metrics.avg_stability, ImVec2(-1, 0));

    ImGui::Text("Entropy:   %.3f", system_metrics.avg_entropy);
    ImGui::ProgressBar(1.0f - system_metrics.avg_entropy, ImVec2(-1, 0));

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

std::vector<sep::quantum::PatternMetrics> EngineTabController::getCurrentPatternMetrics() {
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

sep::quantum::CoherenceManager::CoherenceMetrics EngineTabController::getCoherenceMetrics() {
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

} // namespace sep::workbench
