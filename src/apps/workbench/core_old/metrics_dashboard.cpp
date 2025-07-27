#include "metrics_dashboard.h"

#include <filesystem>

#include "backtester/backtester.h"
#include "backtester/data/data_loader.h"
#include "quantum/pattern_metric_engine.h"
#include "imgui.h"
#include <implot.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <chrono>
#include <ctime>
#include <limits>
#include <cstdio>
#include <cmath>
#include <numeric>
#include <fstream>
#include <nlohmann/json.hpp>
#include "file_dialog.hpp"

namespace sep::workbench {

    MetricsDashboard::MetricsDashboard()
        : monitor_(std::make_unique<MetricsMonitor>()),
          memory_monitor_(std::make_unique<MemoryMonitor>()),
          backtester_(std::make_unique<sep::workbench::backtester::Backtester>())
    {
    }

MetricsDashboard::~MetricsDashboard() {
    shutdown();
}

bool MetricsDashboard::initialize() {
    if (!monitor_->initialize()) {
        std::cerr << "[MetricsDashboard] Failed to initialize metrics monitor" << std::endl;
        return false;
    }
    
    if (!memory_monitor_->initialize()) {
        std::cerr << "[MetricsDashboard] Failed to initialize memory monitor" << std::endl;
        return false;
    }
    
    // Initialize ImPlot context if needed
    if (!ImPlot::GetCurrentContext()) {
        ImPlot::CreateContext();
    }
    
    // Start memory monitoring if auto-enabled
    if (auto_monitor_memory_) {
        memory_monitor_->startMonitoring();
    }
    
    // Initialize OANDA if available
    initializeOandaConnection();
    
    // Try to load sample data for demonstration
    if (loadSampleData()) {
        std::cout << "[MetricsDashboard] Using sample data for demonstration" << std::endl;
    } else {
        std::cout << "[MetricsDashboard] Sample data not available, will use OANDA API when needed" << std::endl;
    }
    
    std::cout << "[MetricsDashboard] Initialized successfully" << std::endl;
    return true;
}

void MetricsDashboard::setTradeManager(TradeManager* trade_manager) {
    trade_manager_ = trade_manager;
}

void MetricsDashboard::shutdown() {
    if (monitor_) {
        monitor_->shutdown();
    }
    if (memory_monitor_) {
        memory_monitor_->shutdown();
    }
}

void MetricsDashboard::render() {
    if (!show_dashboard_) return;

    ImGui::SetNextWindowSize(ImVec2(1200, 800), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("SEP Pattern Metrics Dashboard", &show_dashboard_)) {
        
        // Main menu bar
        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("View")) {
                ImGui::MenuItem("Control Panel", nullptr, &show_control_panel_);
                ImGui::MenuItem("System Metrics", nullptr, &show_system_metrics_);
                ImGui::MenuItem("Pattern List", nullptr, &show_pattern_list_);
                ImGui::MenuItem("Metrics Graphs", nullptr, &show_metrics_graphs_);
                ImGui::MenuItem("Memory Monitor", nullptr, &show_memory_monitor_);
                ImGui::MenuItem("Backtester", nullptr, &show_backtester_panel_);
                ImGui::EndMenu();
            }
            
            if (ImGui::BeginMenu("Help")) {
                if (ImGui::MenuItem("About SEP Engine")) {
                    // Could open help window
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        // Main OANDA trading view with historical chart
        renderComprehensiveTradingCharts();

        if (show_backtester_panel_)
        {
            renderBacktesterPanel();
        }
    }
    ImGui::End();

    std::string selected;
    if (file_dialog_.render(selected)) {
        strncpy(file_path_buffer_, selected.c_str(), sizeof(file_path_buffer_) - 1);
        file_path_buffer_[sizeof(file_path_buffer_) - 1] = '\0';
    }
}

void MetricsDashboard::renderControlPanel() {
    ImGui::SeparatorText("Data Source");
    
    renderDataSourceSelector();
    
    ImGui::Spacing();
    ImGui::SeparatorText("Processing Controls");
    
    // Processing controls
    if (monitor_->isProcessing()) {
        if (ImGui::Button("Stop Processing", ImVec2(-1, 0))) {
            handleStopProcessing();
        }
        ImGui::TextColored(ImVec4(0, 1, 0, 1), "● Processing Active");
    } else {
        if (ImGui::Button("Start Processing", ImVec2(-1, 0))) {
            handleStartProcessing();
        }
        ImGui::TextColored(ImVec4(1, 1, 0, 1), "○ Processing Stopped");
    }
    
    if (ImGui::Button("Clear All Data", ImVec2(-1, 0))) {
        handleClearData();
    }
    
    ImGui::Spacing();
    ImGui::SeparatorText("Export");
    
    ImGui::InputText("Export Path", export_path_buffer_, sizeof(export_path_buffer_));
    if (ImGui::Button("Export Metrics", ImVec2(-1, 0))) {
        handleExportMetrics();
    }
    
    ImGui::Spacing();
    ImGui::SeparatorText("Pattern Filters");
    
    ImGui::SliderFloat("Min Coherence", &min_coherence_filter_, 0.0f, 1.0f);
    ImGui::SliderFloat("Min Stability", &min_stability_filter_, 0.0f, 1.0f);
    ImGui::SliderFloat("Max Entropy", &max_entropy_filter_, 0.0f, 1.0f);
    
    const char* sort_modes[] = {"Coherence", "Stability", "Frequency"};
    ImGui::Combo("Sort By", &pattern_sort_mode_, sort_modes, 3);
}

void MetricsDashboard::renderSystemMetrics() {
    const auto& metrics = monitor_->getSystemMetrics();
    
    ImGui::SeparatorText("System Overview");
    
    // Key metrics in a grid
    if (ImGui::BeginTable("SystemMetrics", 3, ImGuiTableFlags_Borders)) {
        ImGui::TableSetupColumn("Coherence");
        ImGui::TableSetupColumn("Stability");
        ImGui::TableSetupColumn("Entropy");
        ImGui::TableHeadersRow();
        
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("%.3f", metrics.avg_coherence);
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%.3f", metrics.avg_stability);
        ImGui::TableSetColumnIndex(2);
        ImGui::Text("%.3f", metrics.avg_entropy);
        
        ImGui::EndTable();
    }

    ImGui::Spacing();
    
    // Pattern statistics
    ImGui::SeparatorText("Pattern Statistics");
    ImGui::Text("Total Patterns: %zu", metrics.total_patterns);
    ImGui::Text("Unique Patterns: %zu", metrics.unique_patterns);
    ImGui::Text("Active Patterns: %zu", metrics.active_patterns);
    ImGui::Text("Emergence Rate: %.2f patterns/sec", metrics.pattern_emergence_rate);
    
    // Status indicators
    ImGui::Spacing();
    ImGui::SeparatorText("Status");
    
    // Coherence indicator
    ImVec4 coherence_color = metrics.avg_coherence > 0.7f ? ImVec4(0, 1, 0, 1) : 
                            metrics.avg_coherence > 0.4f ? ImVec4(1, 1, 0, 1) : ImVec4(1, 0, 0, 1);
    ImGui::TextColored(coherence_color, "Coherence: %s", 
                      metrics.avg_coherence > 0.7f ? "High" : 
                      metrics.avg_coherence > 0.4f ? "Medium" : "Low");
    
    // Pattern activity indicator
    ImVec4 activity_color = metrics.active_patterns > 0 ? ImVec4(0, 1, 0, 1) : ImVec4(0.5f, 0.5f, 0.5f, 1);
    ImGui::TextColored(activity_color, "Pattern Activity: %s", 
                      metrics.active_patterns > 0 ? "Active" : "Idle");
}

void MetricsDashboard::renderPatternList() {
    const auto& patterns = monitor_->getPatternStats();
    
    ImGui::SeparatorText("Detected Patterns");
    ImGui::Text("Found %zu patterns", patterns.size());
    
    if (patterns.empty()) {
        ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1), "No patterns detected. Load data and start processing.");
        return;
    }
    
    // Pattern table
    if (ImGui::BeginTable("Patterns", 6, ImGuiTableFlags_Borders | ImGuiTableFlags_Sortable | ImGuiTableFlags_ScrollY)) {
        ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed, 80);
        ImGui::TableSetupColumn("Coherence", ImGuiTableColumnFlags_WidthFixed, 80);
        ImGui::TableSetupColumn("Stability", ImGuiTableColumnFlags_WidthFixed, 80);
        ImGui::TableSetupColumn("Entropy", ImGuiTableColumnFlags_WidthFixed, 80);
        ImGui::TableSetupColumn("Length", ImGuiTableColumnFlags_WidthFixed, 60);
        ImGui::TableSetupColumn("Frequency", ImGuiTableColumnFlags_WidthFixed, 80);
        ImGui::TableHeadersRow();
        
        // Filter and sort patterns
        std::vector<const MetricsMonitor::PatternStats*> filtered_patterns;
        for (const auto& pattern : patterns) {
            if (pattern.coherence >= min_coherence_filter_ &&
                pattern.stability >= min_stability_filter_ &&
                pattern.entropy <= max_entropy_filter_) {
                filtered_patterns.push_back(&pattern);
            }
        }
        
        // Sort based on selected mode
        std::sort(filtered_patterns.begin(), filtered_patterns.end(), 
                 [this](const auto* a, const auto* b) {
                     switch (pattern_sort_mode_) {
                         case 0: return a->coherence > b->coherence;
                         case 1: return a->stability > b->stability;
                         case 2: return a->frequency > b->frequency;
                         default: return false;
                     }
                 });
        
        for (const auto* pattern : filtered_patterns) {
            ImGui::TableNextRow();
            
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%.8s", pattern->pattern_id.c_str());
            
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%.3f", pattern->coherence);
            
            ImGui::TableSetColumnIndex(2);
            ImGui::Text("%.3f", pattern->stability);
            
            ImGui::TableSetColumnIndex(3);
            ImGui::Text("%.3f", pattern->entropy);
            
            ImGui::TableSetColumnIndex(4);
            ImGui::Text("%zu", pattern->length);
            
            ImGui::TableSetColumnIndex(5);
            ImGui::Text("%zu", pattern->frequency);
        }
        
        ImGui::EndTable();
    }
}

void MetricsDashboard::renderMetricsGraphs() {
    // Update history with current metrics
    const auto& metrics = monitor_->getSystemMetrics();
    
    coherence_history_.push_back(metrics.avg_coherence);
    stability_history_.push_back(metrics.avg_stability);
    entropy_history_.push_back(metrics.avg_entropy);
    
    // Keep history within bounds
    if (coherence_history_.size() > max_history_size_) {
        coherence_history_.erase(coherence_history_.begin());
        stability_history_.erase(stability_history_.begin());
        entropy_history_.erase(entropy_history_.begin());
    }
    
    ImGui::Text("Real-time Metrics Graph");
    ImGui::Separator();

    if (!coherence_history_.empty()) {
        if (ImPlot::BeginPlot("Metrics", ImVec2(-1, 100), ImPlotFlags_NoLegend)) {
            ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoTickLabels, ImPlotAxisFlags_NoTickLabels);
            ImPlot::SetupAxisLimits(ImAxis_X1, 0, coherence_history_.size(), ImGuiCond_Always);
            ImPlot::SetupAxisLimits(ImAxis_Y1, 0.0f, 1.0f, ImGuiCond_Always);
            ImPlot::PlotLine("Coherence", coherence_history_.data(), coherence_history_.size());
            ImPlot::PlotLine("Stability", stability_history_.data(), stability_history_.size());
            ImPlot::PlotLine("Entropy", entropy_history_.data(), entropy_history_.size());
            ImPlot::EndPlot();
        }
    }
}

void MetricsDashboard::renderDataSourceSelector() {
    const char* source_types[] = {"File", "Live Stream", "Generated Data"};
    ImGui::Combo("Source Type", &data_source_type_, source_types, 3);
    
    switch (data_source_type_) {
        case 0: // File
            ImGui::InputText("File Path", file_path_buffer_, sizeof(file_path_buffer_));
            ImGui::SameLine();
            if (ImGui::Button("Browse")) {
                file_dialog_.open(std::filesystem::path(file_path_buffer_).empty() ? "." : std::filesystem::path(file_path_buffer_).parent_path().string());
            }
            if (ImGui::Button("Load File", ImVec2(-1, 0))) {
                handleDataLoad();
            }
            break;
            
        case 1: // Live Stream
            ImGui::Text("Live stream not yet implemented");
            break;
            
        case 2: // Generated
            if (ImGui::Button("Generate Test Data", ImVec2(-1, 0))) {
                // Generate some test data
                std::vector<uint8_t> test_data(1024);
                for (size_t i = 0; i < test_data.size(); ++i) {
                    test_data[i] = static_cast<uint8_t>(i % 256);
                }
                monitor_->ingestData(test_data.data(), test_data.size());
            }
            break;
    }
}

void MetricsDashboard::handleDataLoad() {
    if (strlen(file_path_buffer_) > 0) {
        monitor_->ingestFile(file_path_buffer_);
    }
}

void MetricsDashboard::handleClearData() {
    monitor_->clear();
    coherence_history_.clear();
    stability_history_.clear();
    entropy_history_.clear();
}

void MetricsDashboard::handleExportMetrics() {
    monitor_->saveMetricsToFile(export_path_buffer_);
}

void MetricsDashboard::handleStartProcessing() {
    monitor_->startProcessing();
}

void MetricsDashboard::handleStopProcessing() {
    monitor_->stopProcessing();
}

void MetricsDashboard::renderBacktesterPanel()
{
    ImGui::Begin("Backtester", &show_backtester_panel_);
    if (ImGui::Button("Run Backtest")) {
        backtester::DataLoader loader;
        if (std::strlen(file_path_buffer_) > 0)
            loader.load_data(file_path_buffer_);
        else
            loader.load_48h_sample();

        sep::quantum::PatternMetricEngine engine;
        engine.init(nullptr);
        backtester_->run(&engine, loader.get_data());
        equity_curve_ = backtester_->getEquityCurve();
    }

    const auto& result = backtester_->getResult();
    ImGui::Text("Total Trades: %d", result.total_trades);
    ImGui::Text("Win Rate: %.2f", result.win_rate);
    ImGui::Text("Total PnL: %.2f", result.total_pnl);
    ImGui::Text("Sharpe Ratio: %.2f", result.sharpe_ratio);
    ImGui::Text("Max Drawdown: %.2f", result.max_drawdown);
    if (!equity_curve_.empty()) {
        if (ImPlot::BeginPlot("Equity Curve", ImVec2(-1,120))) {
            std::vector<float> xs(equity_curve_.size());
            for (size_t i = 0; i < xs.size(); ++i) xs[i] = static_cast<float>(i);
            ImPlot::PlotLine("Equity", xs.data(), equity_curve_.data(), static_cast<int>(xs.size()));
            ImPlot::EndPlot();
        }
    }

    if (!result.trades.empty()) {
        ImGui::Separator();
        ImGui::Text("Trades");
        ImGui::Columns(5, "trades");
        ImGui::Text("Type"); ImGui::NextColumn();
        ImGui::Text("Entry"); ImGui::NextColumn();
        ImGui::Text("Exit"); ImGui::NextColumn();
        ImGui::Text("Hold"); ImGui::NextColumn();
        ImGui::Text("P/L"); ImGui::NextColumn();
        ImGui::Separator();
        for (const auto& t : result.trades) {
            ImGui::Text("%s", t.type == sep::quantum::SignalType::BUY ? "BUY" : "SELL"); ImGui::NextColumn();
            ImGui::Text("%.5f", t.entry_price); ImGui::NextColumn();
            ImGui::Text("%.5f", t.exit_price); ImGui::NextColumn();
            ImGui::Text("%d", t.holding_period); ImGui::NextColumn();
            float pl = (t.type == sep::quantum::SignalType::BUY)
                         ? (t.exit_price - t.entry_price)
                         : (t.entry_price - t.exit_price);
            ImGui::Text("%.5f", pl); ImGui::NextColumn();
        }
        ImGui::Columns(1);
    }

    ImGui::End();
}

void MetricsDashboard::renderMemoryMonitor() {
    ImGui::SeparatorText("Memory Usage Monitor");
    
    // Get current memory stats
    auto mem_stats = memory_monitor_->getCurrentStats();
    auto latest_snapshot = memory_monitor_->getLatestSnapshot();
    
    // Memory overview
    ImGui::BeginGroup();
    ImGui::Text("Current Memory Usage");
    ImGui::Separator();
    
    // Format memory values
    auto format_bytes = [](size_t bytes) -> std::string {
        if (bytes < 1024) return std::to_string(bytes) + " B";
        if (bytes < 1024 * 1024) return std::to_string(bytes / 1024) + " KB";
        if (bytes < 1024 * 1024 * 1024) return std::to_string(bytes / (1024 * 1024)) + " MB";
        return std::to_string(bytes / (1024 * 1024 * 1024)) + " GB";
    };
    
    ImGui::Text("RSS: %s (Peak: %s)",
                format_bytes(mem_stats.current_rss).c_str(),
                format_bytes(mem_stats.peak_rss).c_str());
    
    ImGui::Text("Heap: %s (Peak: %s)",
                format_bytes(mem_stats.current_heap).c_str(),
                format_bytes(mem_stats.peak_heap).c_str());
    
    ImGui::Text("Growth Rate: %.2f MB/s", mem_stats.growth_rate_mb_per_sec);
    
    // Memory pressure indicator
    float pressure = memory_monitor_->getMemoryPressureLevel();
    ImVec4 pressure_color = pressure < 0.5f ? ImVec4(0, 1, 0, 1) :
                           pressure < 0.8f ? ImVec4(1, 1, 0, 1) :
                                           ImVec4(1, 0, 0, 1);
    ImGui::TextColored(pressure_color, "Memory Pressure: %.1f%%", pressure * 100.0f);
    
    ImGui::EndGroup();
    
    ImGui::SameLine();
    ImGui::BeginGroup();
    ImGui::Text("Pattern Memory Distribution");
    ImGui::Separator();
    
    ImGui::Text("Tier 1 (L1): %s", format_bytes(latest_snapshot.tier1_bytes).c_str());
    ImGui::Text("Tier 2 (L2): %s", format_bytes(latest_snapshot.tier2_bytes).c_str());
    ImGui::Text("Tier 3 (L3): %s", format_bytes(latest_snapshot.tier3_bytes).c_str());
    ImGui::Text("Cache Hit Rate: %.1f%%", latest_snapshot.cache_hit_rate * 100.0f);
    ImGui::Text("Patterns: %zu", latest_snapshot.pattern_count);
    if (latest_snapshot.pattern_count > 0) {
        ImGui::Text("Avg Bytes/Pattern: %s",
                    format_bytes(mem_stats.avg_bytes_per_pattern).c_str());
    }
    
    ImGui::EndGroup();
    
    // Memory history graph
    auto history = memory_monitor_->getHistory(500);
    if (!history.empty()) {
        // Update memory history for plotting
        memory_history_.clear();
        memory_growth_history_.clear();
        
        for (const auto& snapshot : history) {
            memory_history_.push_back(snapshot.rss_bytes / (1024.0f * 1024.0f)); // MB
        }
        
        // Calculate growth rate over time
        for (size_t i = 1; i < history.size(); ++i) {
            auto time_diff = std::chrono::duration_cast<std::chrono::milliseconds>(
                history[i].timestamp - history[i-1].timestamp).count() / 1000.0f;
            if (time_diff > 0) {
                float growth = (history[i].rss_bytes - history[i-1].rss_bytes) /
                              (1024.0f * 1024.0f * time_diff); // MB/s
                memory_growth_history_.push_back(growth);
            }
        }
        
        ImGui::Text("Memory Usage Over Time");
        if (ImPlot::BeginPlot("Memory", ImVec2(-1, 150), ImPlotFlags_NoLegend)) {
            ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoTickLabels, ImPlotAxisFlags_NoTickLabels);
            ImPlot::SetupAxisLimits(ImAxis_X1, 0, memory_history_.size(), ImGuiCond_Always);
            ImPlot::SetupAxisLimits(ImAxis_Y1, 0.0f, mem_stats.peak_rss / (1024.0f * 1024.0f) * 1.2f, ImGuiCond_Always);
            ImPlot::PlotLine("RSS Memory (MB)", memory_history_.data(), memory_history_.size());
            ImPlot::EndPlot();
        }

        if (!memory_growth_history_.empty()) {
            ImGui::Text("Memory Growth Rate");
            if (ImPlot::BeginPlot("Growth", ImVec2(-1, 100), ImPlotFlags_NoLegend)) {
                ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoTickLabels, ImPlotAxisFlags_NoTickLabels);
                ImPlot::SetupAxisLimits(ImAxis_X1, 0, memory_growth_history_.size(), ImGuiCond_Always);
                ImPlot::PlotLine("Growth Rate (MB/s)", memory_growth_history_.data(), memory_growth_history_.size());
                ImPlot::EndPlot();
            }
        }
    }
    
    // Memory control buttons
    ImGui::Separator();
    if (memory_monitor_->isMonitoring()) {
        if (ImGui::Button("Stop Monitoring")) {
            memory_monitor_->stopMonitoring();
        }
    } else {
        if (ImGui::Button("Start Monitoring")) {
            memory_monitor_->startMonitoring();
        }
    }
    
    ImGui::SameLine();
    if (ImGui::Button("Reset History")) {
        memory_monitor_->reset();
        memory_history_.clear();
        memory_growth_history_.clear();
    }
    
    ImGui::SameLine();
    ImGui::Checkbox("Auto-monitor", &auto_monitor_memory_);
}

void MetricsDashboard::initializeMetricsMonitor() {
    if (!monitor_) {
        try {
            monitor_ = std::make_unique<MetricsMonitor>();
            if (monitor_->initialize()) {
                std::cout << "MetricsMonitor initialized successfully" << std::endl;
            } else {
                std::cerr << "ERROR: Failed to initialize MetricsMonitor" << std::endl;
                monitor_.reset();
            }
        } catch (const std::exception& e) {
            std::cerr << "ERROR: Failed to create MetricsMonitor: " << e.what() << std::endl;
            monitor_.reset();
        }
    }
}

void MetricsDashboard::initializeOandaConnection() {
    const char* api_key = std::getenv("OANDA_API_KEY");
    const char* account_id = std::getenv("OANDA_ACCOUNT_ID");
}
}  // namespace sep::workbench
