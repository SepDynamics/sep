#pragma once

#include <memory>
#include <string>
#include <vector>
#include <chrono>
#include <deque>

#include <imgui.h>
#include <glm/glm.hpp>

#include "memory/memory_tier_manager.hpp"
#include "quantum/processor.h"
#include "quantum/types.h"
#include "apps/workbench/panels/signal_testing_panel.h"
#include "apps/workbench/signal_generator/quantum_signal_generator.h"
#include "demo_panel.h"

namespace sep::demo {

// Memory Tier Visualization Panel
class MemoryTierPanel : public DemoPanel {
public:
    explicit MemoryTierPanel(std::shared_ptr<memory::MemoryTierManager> manager);
    void render() override;
    const char* getName() const override { return "Memory Tiers"; }

private:
    void renderTierInfo(const char* tier_name, memory::MemoryTierEnum tier);
    void renderMemoryBlock(const memory::MemoryBlock* block);
    
    std::shared_ptr<memory::MemoryTierManager> manager_;
    bool show_block_details_ = false;
    int selected_block_id_ = -1;
};

// Pattern Processing Panel
class PatternProcessingPanel : public DemoPanel {
public:
    explicit PatternProcessingPanel(std::shared_ptr<quantum::Processor> processor);
    void render() override;
    const char* getName() const override { return "Pattern Processing"; }

private:
    void renderPatternInfo(const quantum::Pattern& pattern);
    void renderProcessingMetrics();
    
    std::shared_ptr<quantum::Processor> processor_;
    std::deque<float> coherence_history_;
    std::deque<float> stability_history_;
    static constexpr size_t HISTORY_SIZE = 100;
};

// System Metrics Panel
class SystemMetricsPanel : public DemoPanel {
public:
    explicit SystemMetricsPanel(std::shared_ptr<memory::MemoryTierManager> mem_manager,
                               std::shared_ptr<quantum::Processor> processor);
    void render() override;
    const char* getName() const override { return "System Metrics"; }

private:
    void updateMetrics();
    
    std::shared_ptr<memory::MemoryTierManager> mem_manager_;
    std::shared_ptr<quantum::Processor> processor_;
    std::chrono::steady_clock::time_point last_update_;
    
    struct Metrics {
        size_t total_patterns = 0;
        size_t active_patterns = 0;
        float avg_coherence = 0.0f;
        float avg_stability = 0.0f;
        size_t memory_allocated = 0;
        size_t memory_fragmented = 0;
        float processing_rate = 0.0f;
    } metrics_;
};

// Pattern Generator Panel (for testing)
class PatternGeneratorPanel : public DemoPanel {
public:
    explicit PatternGeneratorPanel(std::shared_ptr<quantum::Processor> processor);
    void render() override;
    const char* getName() const override { return "Pattern Generator"; }

private:
    void generateRandomPattern();
    void generateWavePattern();
    void generateClusterPattern();
    
    std::shared_ptr<quantum::Processor> processor_;
    int pattern_count_ = 10;
    float coherence_range_[2] = {0.1f, 0.9f};
    float stability_range_[2] = {0.1f, 0.9f};
    int generation_type_ = 0; // 0: Random, 1: Wave, 2: Cluster
};

// Main SEP Demo Application
class SEPDemoApp {
public:
    SEPDemoApp();
    ~SEPDemoApp();

    bool initialize();
    void render();
    void shutdown();

private:
    void renderMainMenu();
    void renderDockSpace();
    void initializeDemoData();

    // Core components
    std::shared_ptr<memory::MemoryTierManager> memory_manager_;
    std::shared_ptr<quantum::Processor> quantum_processor_;
    std::shared_ptr<workbench::QuantumSignalGenerator> signal_generator_;
    
    // Demo panels
    std::vector<std::unique_ptr<DemoPanel>> panels_;
    std::unique_ptr<workbench::SignalTestingPanel> signal_testing_panel_;
    
    // UI state
    bool show_demo_window_ = false;
    bool show_metrics_overlay_ = true;
    bool initialized_ = false;
    
    // Performance tracking
    float frame_time_ms_ = 0.0f;
    std::chrono::steady_clock::time_point last_frame_time_;
};

} // namespace sep::demo