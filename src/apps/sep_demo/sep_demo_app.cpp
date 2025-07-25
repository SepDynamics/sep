#include "sep_demo_app.hpp"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <random>
#include <sstream>

#include "imgui.h"

namespace sep::demo {

// MemoryTierPanel Implementation
MemoryTierPanel::MemoryTierPanel(std::shared_ptr<memory::MemoryTierManager> manager)
    : manager_(std::move(manager)) {}

void MemoryTierPanel::render() {
    if (!ImGui::Begin(getName(), &visible_)) {
        ImGui::End();
        return;
    }

    ImGui::Text("Memory Tier Overview");
    ImGui::Separator();

    // Render each tier
    renderTierInfo("Short-Term Memory (STM)", memory::MemoryTierEnum::STM);
    ImGui::Separator();
    renderTierInfo("Mid-Term Memory (MTM)", memory::MemoryTierEnum::MTM);
    ImGui::Separator();
    renderTierInfo("Long-Term Memory (LTM)", memory::MemoryTierEnum::LTM);

    ImGui::End();
}

void MemoryTierPanel::renderTierInfo(const char* tier_name, memory::MemoryTierEnum tier) {
    if (ImGui::TreeNode(tier_name)) {
        // Since getTierStats doesn't exist, we'll show placeholder info
        ImGui::Text("Tier: %s", tier_name);
        ImGui::Text("Status: Active");
        
        // Show some mock statistics
        float usage = 0.0f;
        switch (tier) {
            case memory::MemoryTierEnum::STM:
                usage = 0.3f;
                ImGui::Text("Capacity: 1024 blocks");
                break;
            case memory::MemoryTierEnum::MTM:
                usage = 0.5f;
                ImGui::Text("Capacity: 4096 blocks");
                break;
            case memory::MemoryTierEnum::LTM:
                usage = 0.7f;
                ImGui::Text("Capacity: 16384 blocks");
                break;
            case memory::MemoryTierEnum::HOST:
            case memory::MemoryTierEnum::DEVICE:
            case memory::MemoryTierEnum::UNIFIED:
                // These are memory location types, not tier types
                usage = 0.0f;
                ImGui::Text("N/A - Memory location type");
                break;
        }
        
        ImGui::ProgressBar(usage, ImVec2(-1, 0), "Usage");

        if (show_block_details_ && ImGui::TreeNode("Block Details")) {
            ImGui::Text("Block details would be shown here");
            ImGui::TreePop();
        }

        ImGui::TreePop();
    }
}

// PatternProcessingPanel Implementation
PatternProcessingPanel::PatternProcessingPanel(std::shared_ptr<quantum::Processor> processor)
    : processor_(std::move(processor)) {}

void PatternProcessingPanel::render() {
    if (!ImGui::Begin(getName(), &visible_)) {
        ImGui::End();
        return;
    }

    ImGui::Text("Pattern Processing Overview");
    ImGui::Separator();

    renderProcessingMetrics();
    
    ImGui::Separator();
    
    // Pattern list
    if (ImGui::CollapsingHeader("Active Patterns")) {
        auto patterns = processor_->getPatterns();
        for (const auto& pattern : patterns) {
            renderPatternInfo(pattern);
            ImGui::Separator();
        }
    }

    ImGui::End();
}

void PatternProcessingPanel::renderPatternInfo(const quantum::Pattern& pattern) {
    if (ImGui::TreeNode(pattern.id.c_str())) {
        ImGui::Text("Position: (%.2f, %.2f, %.2f, %.2f)",
                    pattern.position.x, pattern.position.y, pattern.position.z, pattern.position.w);
        ImGui::Text("Velocity: (%.2f, %.2f, %.2f, %.2f)",
                    pattern.velocity.x, pattern.velocity.y, pattern.velocity.z, pattern.velocity.w);
        ImGui::Text("Coherence: %.3f", pattern.coherence);
        ImGui::Text("Generation: %d", pattern.generation);
        ImGui::Text("Amplitude: (%.3f, %.3f)", pattern.amplitude.real(), pattern.amplitude.imag());
        
        // Quantum state
        if (ImGui::TreeNode("Quantum State")) {
            ImGui::Text("Coherence: %.3f", pattern.quantum_state.coherence);
            ImGui::Text("Stability: %.3f", pattern.quantum_state.stability);
            ImGui::Text("Energy: %.3f", pattern.quantum_state.energy);
            ImGui::Text("Phase: %.3f", pattern.quantum_state.phase);
            ImGui::Text("Entropy: %.3f", pattern.quantum_state.entropy);
            ImGui::Text("Memory Tier: %s",
                pattern.quantum_state.memory_tier == memory::MemoryTierEnum::STM ? "STM" :
                pattern.quantum_state.memory_tier == memory::MemoryTierEnum::MTM ? "MTM" : "LTM");
            ImGui::TreePop();
        }
        
        ImGui::TreePop();
    }
}

void PatternProcessingPanel::renderProcessingMetrics() {
    auto patterns = processor_->getPatterns();
    
    // Calculate average metrics
    float avg_coherence = 0.0f;
    float avg_stability = 0.0f;
    
    if (!patterns.empty()) {
        for (const auto& pattern : patterns) {
            avg_coherence += pattern.coherence;
            avg_stability += pattern.quantum_state.stability;
        }
        avg_coherence /= patterns.size();
        avg_stability /= patterns.size();
    }
    
    // Update history
    coherence_history_.push_back(avg_coherence);
    stability_history_.push_back(avg_stability);
    
    while (coherence_history_.size() > HISTORY_SIZE) {
        coherence_history_.pop_front();
    }
    while (stability_history_.size() > HISTORY_SIZE) {
        stability_history_.pop_front();
    }
    
    ImGui::Text("Active Patterns: %zu", patterns.size());
    ImGui::Text("Average Coherence: %.3f", avg_coherence);
    ImGui::Text("Average Stability: %.3f", avg_stability);
    
    // Simple text-based history display instead of plot
    if (ImGui::CollapsingHeader("Metrics History")) {
        ImGui::Text("Recent Coherence Values:");
        size_t start = coherence_history_.size() > 10 ? coherence_history_.size() - 10 : 0;
        for (size_t i = start; i < coherence_history_.size(); ++i) {
            ImGui::Text("  [%zu]: %.3f", i, coherence_history_[i]);
        }
    }
}

// SystemMetricsPanel Implementation
SystemMetricsPanel::SystemMetricsPanel(std::shared_ptr<memory::MemoryTierManager> mem_manager,
                                     std::shared_ptr<quantum::Processor> processor)
    : mem_manager_(std::move(mem_manager)), processor_(std::move(processor)) {}

void SystemMetricsPanel::render() {
    if (!ImGui::Begin(getName(), &visible_)) {
        ImGui::End();
        return;
    }

    updateMetrics();

    ImGui::Text("System Performance Metrics");
    ImGui::Separator();

    // Memory metrics
    if (ImGui::CollapsingHeader("Memory Usage", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Text("Total Allocated: %zu bytes", metrics_.memory_allocated);
        ImGui::Text("Fragmented: %zu bytes", metrics_.memory_fragmented);
        
        float usage_ratio = metrics_.memory_allocated > 0 ? 
            static_cast<float>(metrics_.memory_allocated - metrics_.memory_fragmented) / 
            static_cast<float>(metrics_.memory_allocated) : 0.0f;
        ImGui::ProgressBar(usage_ratio, ImVec2(-1, 0), "Efficiency");
    }

    // Pattern metrics
    if (ImGui::CollapsingHeader("Pattern Statistics", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Text("Total Patterns: %zu", metrics_.total_patterns);
        ImGui::Text("Active Patterns: %zu", metrics_.active_patterns);
        ImGui::Text("Average Coherence: %.3f", metrics_.avg_coherence);
        ImGui::Text("Average Stability: %.3f", metrics_.avg_stability);
    }

    // Processing metrics
    if (ImGui::CollapsingHeader("Processing Performance", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Text("Processing Rate: %.2f patterns/sec", metrics_.processing_rate);
    }

    ImGui::End();
}

void SystemMetricsPanel::updateMetrics() {
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_update_).count();
    
    if (elapsed < 100) return; // Update every 100ms
    
    // Update memory metrics
    metrics_.memory_allocated = 0;
    metrics_.memory_fragmented = 0;
    
    // Mock memory metrics since getTierStats doesn't exist
    metrics_.memory_allocated = 1024 * 1024; // 1MB
    metrics_.memory_fragmented = 102400; // 100KB
    
    // Update pattern metrics
    auto patterns = processor_->getPatterns();
    metrics_.total_patterns = patterns.size();
    metrics_.active_patterns = std::count_if(patterns.begin(), patterns.end(),
        [](const auto& p) { return p.coherence > 0.5f; });
    
    metrics_.avg_coherence = 0.0f;
    metrics_.avg_stability = 0.0f;
    if (!patterns.empty()) {
        for (const auto& pattern : patterns) {
            metrics_.avg_coherence += pattern.coherence;
            metrics_.avg_stability += pattern.quantum_state.stability;
        }
        metrics_.avg_coherence /= patterns.size();
        metrics_.avg_stability /= patterns.size();
    }
    
    // Update processing rate (simplified)
    static size_t last_pattern_count = 0;
    float time_delta = elapsed / 1000.0f;
    metrics_.processing_rate = (patterns.size() - last_pattern_count) / time_delta;
    last_pattern_count = patterns.size();
    
    last_update_ = now;
}

// PatternGeneratorPanel Implementation
PatternGeneratorPanel::PatternGeneratorPanel(std::shared_ptr<quantum::Processor> processor)
    : processor_(std::move(processor)) {}

void PatternGeneratorPanel::render() {
    if (!ImGui::Begin(getName(), &visible_)) {
        ImGui::End();
        return;
    }

    ImGui::Text("Pattern Generation Tools");
    ImGui::Separator();

    ImGui::InputInt("Pattern Count", &pattern_count_);
    pattern_count_ = std::max(1, std::min(pattern_count_, 1000));

    ImGui::SliderFloat2("Coherence Range", coherence_range_, 0.0f, 1.0f);
    ImGui::SliderFloat2("Stability Range", stability_range_, 0.0f, 1.0f);

    ImGui::RadioButton("Random", &generation_type_, 0);
    ImGui::SameLine();
    ImGui::RadioButton("Wave", &generation_type_, 1);
    ImGui::SameLine();
    ImGui::RadioButton("Cluster", &generation_type_, 2);

    if (ImGui::Button("Generate Patterns")) {
        switch (generation_type_) {
            case 0: generateRandomPattern(); break;
            case 1: generateWavePattern(); break;
            case 2: generateClusterPattern(); break;
        }
    }

    ImGui::SameLine();
    if (ImGui::Button("Clear All Patterns")) {
        // Clear patterns by removing them one by one
        auto patterns = processor_->getPatterns();
        for (const auto& pattern : patterns) {
            processor_->removePattern(pattern.id);
        }
    }

    ImGui::End();
}

void PatternGeneratorPanel::generateRandomPattern() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> pos_dist(-10.0f, 10.0f);
    std::uniform_real_distribution<float> coherence_dist(coherence_range_[0], coherence_range_[1]);
    std::uniform_real_distribution<float> stability_dist(stability_range_[0], stability_range_[1]);
    std::uniform_real_distribution<float> energy_dist(0.1f, 2.0f);

    std::vector<quantum::Pattern> patterns;
    patterns.reserve(pattern_count_);

    for (int i = 0; i < pattern_count_; ++i) {
        quantum::Pattern pattern;
        pattern.id = "pattern_" + std::to_string(i) + "_" + std::to_string(gen());
        pattern.position = glm::vec4(pos_dist(gen), pos_dist(gen), pos_dist(gen), 1.0f);
        pattern.velocity = glm::vec4(0.0f);
        pattern.coherence = coherence_dist(gen);
        pattern.amplitude = std::complex<float>(1.0f, 0.0f);
        pattern.generation = 0;
        
        pattern.quantum_state.coherence = pattern.coherence;
        pattern.quantum_state.stability = stability_dist(gen);
        pattern.quantum_state.energy = energy_dist(gen);
        pattern.quantum_state.phase = 0.0f;
        
        patterns.push_back(pattern);
    }

    for (const auto& pattern : patterns) {
        processor_->addPattern(pattern);
    }
}

void PatternGeneratorPanel::generateWavePattern() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> coherence_dist(coherence_range_[0], coherence_range_[1]);
    std::uniform_real_distribution<float> stability_dist(stability_range_[0], stability_range_[1]);

    std::vector<quantum::Pattern> patterns;
    patterns.reserve(pattern_count_);

    float wave_length = 10.0f;
    float wave_amplitude = 5.0f;

    for (int i = 0; i < pattern_count_; ++i) {
        float t = static_cast<float>(i) / static_cast<float>(pattern_count_ - 1);
        float angle = t * 2.0f * M_PI;

        quantum::Pattern pattern;
        pattern.id = "wave_" + std::to_string(i) + "_" + std::to_string(gen());
        pattern.position = glm::vec4(
            t * wave_length - wave_length / 2.0f,
            std::sin(angle * 3.0f) * wave_amplitude,
            std::cos(angle * 2.0f) * wave_amplitude,
            1.0f
        );
        pattern.velocity = glm::vec4(0.0f);
        pattern.coherence = coherence_dist(gen);
        pattern.amplitude = std::complex<float>(std::cos(angle), std::sin(angle));
        pattern.generation = 0;
        
        pattern.quantum_state.coherence = pattern.coherence;
        pattern.quantum_state.stability = stability_dist(gen);
        pattern.quantum_state.energy = 1.0f + std::sin(angle);
        pattern.quantum_state.phase = angle;
        
        patterns.push_back(pattern);
    }

    for (const auto& pattern : patterns) {
        processor_->addPattern(pattern);
    }
}

void PatternGeneratorPanel::generateClusterPattern() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> offset_dist(-1.0f, 1.0f);
    std::uniform_real_distribution<float> coherence_dist(coherence_range_[0], coherence_range_[1]);
    std::uniform_real_distribution<float> stability_dist(stability_range_[0], stability_range_[1]);

    std::vector<quantum::Pattern> patterns;
    patterns.reserve(pattern_count_);

    int clusters = 3;
    std::vector<glm::vec3> cluster_centers = {
        glm::vec3(-5.0f, 0.0f, 0.0f),
        glm::vec3(5.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 5.0f, 0.0f)
    };

    for (int i = 0; i < pattern_count_; ++i) {
        int cluster_idx = i % clusters;
        glm::vec3 center = cluster_centers[cluster_idx];

        quantum::Pattern pattern;
        pattern.id = "cluster_" + std::to_string(cluster_idx) + "_" + std::to_string(i);
        pattern.position = glm::vec4(
            center.x + offset_dist(gen) * 2.0f,
            center.y + offset_dist(gen) * 2.0f,
            center.z + offset_dist(gen) * 2.0f,
            1.0f
        );
        pattern.velocity = glm::vec4(0.0f);
        pattern.coherence = coherence_dist(gen);
        pattern.amplitude = std::complex<float>(1.0f, 0.0f);
        pattern.generation = 0;
        
        pattern.quantum_state.coherence = pattern.coherence;
        pattern.quantum_state.stability = stability_dist(gen);
        pattern.quantum_state.energy = 1.0f + static_cast<float>(cluster_idx) * 0.5f;
        pattern.quantum_state.phase = 0.0f;
        
        patterns.push_back(pattern);
    }

    for (const auto& pattern : patterns) {
        processor_->addPattern(pattern);
    }
}

// SEPDemoApp Implementation
SEPDemoApp::SEPDemoApp() = default;
SEPDemoApp::~SEPDemoApp() = default;

bool SEPDemoApp::initialize() {
    try {
        // Initialize memory manager
        memory_manager_ = std::make_shared<memory::MemoryTierManager>();
        memory::MemoryThresholdConfig mem_config;
        // Set default configuration
        mem_config.stm_size = 1024;
        mem_config.mtm_size = 4096;
        mem_config.ltm_size = 16384;
        mem_config.promote_stm_to_mtm = 0.7f;
        mem_config.promote_mtm_to_ltm = 0.9f;
        mem_config.demote_threshold = 0.3f;
        mem_config.fragmentation_threshold = 0.3f;
        
        memory_manager_->init(mem_config);

        // Initialize quantum processor with default config
        sep::ProcessingConfig proc_config;
        proc_config.max_patterns = 1000;
        proc_config.mutation_rate = 0.1f;
        proc_config.ltm_coherence_threshold = 0.9f;
        proc_config.mtm_coherence_threshold = 0.6f;
        proc_config.stability_threshold = 0.8f;
        proc_config.enable_cuda = false;
        
        quantum_processor_ = std::make_shared<quantum::Processor>(proc_config);
        quantum_processor_->init(nullptr); // No GPU context for now

        signal_generator_ = std::make_shared<workbench::QuantumSignalGenerator>();

        // Create demo panels
        panels_.push_back(std::make_unique<MemoryTierPanel>(memory_manager_));
        panels_.push_back(std::make_unique<PatternProcessingPanel>(quantum_processor_));
        panels_.push_back(std::make_unique<SystemMetricsPanel>(memory_manager_, quantum_processor_));
        panels_.push_back(std::make_unique<PatternGeneratorPanel>(quantum_processor_));
        
        // Add signal testing panel to main panels list
        auto signal_panel = std::make_unique<workbench::SignalTestingPanel>(signal_generator_);
        signal_testing_panel_ = signal_panel.get(); // Keep raw pointer for direct access
        panels_.push_back(std::move(signal_panel));

        // Initialize with some demo data
        initializeDemoData();

        initialized_ = true;
        last_frame_time_ = std::chrono::steady_clock::now();
        
        return true;
    } catch (const std::exception& e) {
        // Log error
        return false;
    }
}

void SEPDemoApp::render() {
    if (!initialized_) return;

    // Update frame time
    auto now = std::chrono::steady_clock::now();
    frame_time_ms_ = std::chrono::duration<float, std::milli>(now - last_frame_time_).count();
    last_frame_time_ = now;

    // Render main menu
    renderMainMenu();

    // Render dockspace
    renderDockSpace();

    // Render panels
    for (auto& panel : panels_) {
        if (panel->isVisible()) {
            panel->render();
        }
    }

    // Render ImGui demo window if enabled
    if (show_demo_window_) {
        ImGui::ShowDemoWindow(&show_demo_window_);
    }

    // Render metrics overlay
    if (show_metrics_overlay_) {
        ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowBgAlpha(0.35f);
        if (ImGui::Begin("Metrics Overlay", &show_metrics_overlay_, 
                        ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | 
                        ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | 
                        ImGuiWindowFlags_NoNav)) {
            ImGui::Text("Frame Time: %.1f ms", frame_time_ms_);
            ImGui::Text("FPS: %.0f", 1000.0f / frame_time_ms_);
        }
        ImGui::End();
    }
}

void SEPDemoApp::shutdown() {
    if (!initialized_) return;

    panels_.clear();
    quantum_processor_.reset();
    memory_manager_.reset();
    
    initialized_ = false;
}

void SEPDemoApp::renderMainMenu() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Exit")) {
                // Signal exit
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("View")) {
            for (auto& panel : panels_) {
                bool visible = panel->isVisible();
                if (ImGui::MenuItem(panel->getName(), nullptr, &visible)) {
                    panel->setVisible(visible);
                }
            }

            ImGui::Separator();
            ImGui::MenuItem("ImGui Demo", nullptr, &show_demo_window_);
            ImGui::MenuItem("Metrics Overlay", nullptr, &show_metrics_overlay_);
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Tools")) {
            if (ImGui::MenuItem("Process Memory Blocks")) {
                // Process memory blocks
                sep::SEPResult result = memory_manager_->processMemoryBlocks(
                    nullptr,  // input_data
                    nullptr,  // output_data
                    nullptr,  // config
                    0,        // count
                    nullptr,  // previous_data
                    nullptr   // stream
                );
            }
            if (ImGui::MenuItem("Run Quantum Processing")) {
                quantum_processor_->processAll();
            }
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

void SEPDemoApp::renderDockSpace() {
    // Simple workspace window instead of dockspace
    ImGuiWindowFlags window_flags = 0;
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                   ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                   ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    
    ImGui::Begin("Workspace", nullptr, window_flags);
    ImGui::PopStyleVar(3);

    ImGui::End();
}

void SEPDemoApp::initializeDemoData() {
    // Generate some initial patterns for demonstration
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> pos_dist(-10.0f, 10.0f);
    std::uniform_real_distribution<float> coherence_dist(0.1f, 0.9f);
    
    for (int i = 0; i < 10; ++i) {
        quantum::Pattern pattern;
        pattern.id = "init_pattern_" + std::to_string(i);
        pattern.position = glm::vec4(pos_dist(gen), pos_dist(gen), pos_dist(gen), 1.0f);
        pattern.velocity = glm::vec4(0.0f);
        pattern.coherence = coherence_dist(gen);
        pattern.amplitude = std::complex<float>(1.0f, 0.0f);
        pattern.generation = 0;
        
        pattern.quantum_state.coherence = pattern.coherence;
        pattern.quantum_state.stability = coherence_dist(gen);
        pattern.quantum_state.energy = 1.0f;
        pattern.quantum_state.phase = 0.0f;
        
        quantum_processor_->addPattern(pattern);
    }
}

} // namespace sep::demo