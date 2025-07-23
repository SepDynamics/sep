#include "metrics_dashboard.h"
#include <imgui.h>
#include <filesystem>
// #include <implot.h> // TODO: Add implot to third_party
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <chrono>
#include <ctime>
#include <limits>
#include <cstdio>
#include <cmath>
#include <numeric>
#include "file_dialog.hpp"

namespace sep::workbench {

MetricsDashboard::MetricsDashboard()
    : monitor_(std::make_unique<MetricsMonitor>()),
      memory_monitor_(std::make_unique<MemoryMonitor>()) {
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
    
    // Initialize ImPlot if not already done
    // TODO: Add implot support
    // if (!ImPlot::GetCurrentContext()) {
    //     ImPlot::CreateContext();
    // }
    
    // Start memory monitoring if auto-enabled
    if (auto_monitor_memory_) {
        memory_monitor_->startMonitoring();
    }
    
    // Initialize OANDA if available
    initializeOandaConnection();
    
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
        renderOandaMainView();
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
    
    // TODO: Replace with ImPlot when available
    ImGui::Text("Real-time Metrics Graph");
    ImGui::Separator();
    
    if (!coherence_history_.empty()) {
        ImGui::PlotLines("Coherence", coherence_history_.data(), coherence_history_.size(),
                        0, nullptr, 0.0f, 1.0f, ImVec2(0, 80));
        ImGui::PlotLines("Stability", stability_history_.data(), stability_history_.size(),
                        0, nullptr, 0.0f, 1.0f, ImVec2(0, 80));
        ImGui::PlotLines("Entropy", entropy_history_.data(), entropy_history_.size(),
                        0, nullptr, 0.0f, 1.0f, ImVec2(0, 80));
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
        
        // TODO: Replace with ImPlot when available
        ImGui::Text("Memory Usage Over Time");
        ImGui::PlotLines("RSS Memory (MB)", memory_history_.data(), memory_history_.size(),
                        0, nullptr, 0.0f, mem_stats.peak_rss / (1024.0f * 1024.0f) * 1.2f,
                        ImVec2(0, 150));
        
        if (!memory_growth_history_.empty()) {
            ImGui::Text("Memory Growth Rate");
            ImGui::PlotLines("Growth Rate (MB/s)", memory_growth_history_.data(),
                           memory_growth_history_.size(), 0, nullptr, FLT_MIN, FLT_MAX,
                           ImVec2(0, 100));
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

void MetricsDashboard::initializeOandaConnection() {
    const char* api_key = std::getenv("OANDA_API_KEY");
    const char* account_id = std::getenv("OANDA_ACCOUNT_ID");
    
    if (api_key && account_id) {
        oanda_connector_ = std::make_unique<sep::connectors::OandaConnector>(api_key, account_id, true); // Use practice server
        if (oanda_connector_->initialize()) {
            oanda_connected_ = true;
            oanda_status_ = "Connected";
            use_oanda_data_ = true; // Enable data feed by default
            std::cout << "[MetricsDashboard] OANDA connected successfully" << std::endl;
            
            // Automatically start market data updates
            updateOandaData();
        } else {
            oanda_status_ = "Failed to connect";
            std::cout << "[MetricsDashboard] OANDA connection failed: " << oanda_connector_->getLastError() << std::endl;
        }
    } else {
        oanda_status_ = "No credentials";
        std::cout << "[MetricsDashboard] OANDA credentials not found" << std::endl;
    }
}

void MetricsDashboard::updateOandaData() {
    if (!oanda_connected_ || !oanda_connector_) return;
    
    // Set up price callback to update our candlestick and technical data
    oanda_connector_->setPriceCallback([this](const sep::connectors::MarketData& data) {
        // Update candlestick data and technical indicators
        updateCandlestickData(data.instrument, data);
        
        // Feed into pattern analysis if enabled
        if (use_oanda_data_) {
            std::vector<uint8_t> price_data;
            price_data.resize(sizeof(double) * 4);
            memcpy(price_data.data(), &data.bid, sizeof(double));
            memcpy(price_data.data() + sizeof(double), &data.ask, sizeof(double));
            memcpy(price_data.data() + sizeof(double) * 2, &data.mid, sizeof(double));
            memcpy(price_data.data() + sizeof(double) * 3, &data.timestamp, sizeof(double));
            
            monitor_->ingestData(price_data.data(), price_data.size());
        }
        
        std::cout << "[MetricsDashboard] Market data updated: " << data.instrument 
                  << " Mid: " << data.mid << std::endl;
    });
    
    // Start price stream for major pairs
    std::vector<std::string> instruments = {"EUR_USD", "GBP_USD", "USD_JPY", "AUD_USD"};
    if (!oanda_connector_->startPriceStream(instruments)) {
        std::cout << "[MetricsDashboard] Failed to start price stream: " 
                  << oanda_connector_->getLastError() << std::endl;
    } else {
        std::cout << "[MetricsDashboard] Price stream started for " << instruments.size() 
                  << " instruments" << std::endl;
    }
}

void MetricsDashboard::renderOandaPanel() {
    ImGui::Text("OANDA Market Data & Analysis");
    ImGui::Separator();
    
    // Connection status
    ImGui::Text("Status: %s", oanda_status_.c_str());
    if (oanda_connected_) {
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "● Connected");
    } else {
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "● Disconnected");
    }
    
    if (!oanda_connected_) {
        ImGui::Text("Set OANDA_API_KEY and OANDA_ACCOUNT_ID environment variables");
        return;
    }
    
    // Simple data display without visual charts
    ImGui::Text("Real-time OANDA data flowing into SEP engine:");
    
    // Show latest prices
    {
        std::lock_guard<std::mutex> lock(market_data_mutex_);
        for (const auto& [instrument, price] : latest_prices_) {
            ImGui::Text("%s: %.5f", instrument.c_str(), price.mid);
        }
    }
    
    // Show SEP metrics without charts
    if (monitor_) {
        const auto& metrics = monitor_->getSystemMetrics();
        ImGui::Separator();
        ImGui::Text("SEP Engine Metrics (0-1 normalized):");
        ImGui::Text("Coherence: %.3f", metrics.avg_coherence);
        ImGui::Text("Stability: %.3f", metrics.avg_stability);
        ImGui::Text("Predictability: %.3f", 1.0f - metrics.avg_entropy);
        ImGui::Text("Active Patterns: %zu", metrics.active_patterns);
        ImGui::Text("Pattern Rate: %.2f/sec", metrics.pattern_emergence_rate);
    }
    
    ImGui::Separator();
    ImGui::Text("Charts temporarily disabled for debugging");
    ImGui::Text("Data pipeline is active and processing market data");
    
    // Trading panel
    if (ImGui::CollapsingHeader("Trading Controls")) {
        static float units = 1000;
        static float stop_loss_pips = 10;
        
        ImGui::InputFloat("Units", &units);
        ImGui::InputFloat("Stop Loss (pips)", &stop_loss_pips);
        
        if (ImGui::Button("Buy", ImVec2(100, 30))) {
            if (trade_manager_) {
                trade_manager_->placeOrder(selected_instrument_.c_str(), units, 1.1, stop_loss_pips);
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Sell", ImVec2(100, 30))) {
            if (trade_manager_) {
                trade_manager_->placeOrder(selected_instrument_.c_str(), -units, 1.1, stop_loss_pips);
            }
        }
    }
}

void MetricsDashboard::renderCandlestickChart(const std::string& instrument) {
    std::lock_guard<std::mutex> lock(market_data_mutex_);
    
    auto it = candlestick_data_.find(instrument);
    if (it == candlestick_data_.end() || it->second.empty()) {
        ImGui::Text("No candlestick data available for %s", instrument.c_str());
        ImGui::Text("Waiting for market data...");
        
        // Debug info
        ImGui::Text("Debug: candlestick_data_ size: %zu", candlestick_data_.size());
        ImGui::Text("Debug: latest_prices_ size: %zu", latest_prices_.size());
        for (const auto& [inst, data] : latest_prices_) {
            ImGui::Text("  Latest: %s -> %.5f", inst.c_str(), data.mid);
        }
        return;
    }
    
    const auto& candles = it->second;
    ImGui::Text("Candlestick Chart: %s (%zu candles)", instrument.c_str(), candles.size());
    
    // Prepare price data
    std::vector<float> close_prices;
    for (const auto& candle : candles) {
        close_prices.push_back((float)candle.close);
    }
    
    if (!close_prices.empty()) {
        float min_price = *std::min_element(close_prices.begin(), close_prices.end());
        float max_price = *std::max_element(close_prices.begin(), close_prices.end());
        
        ImGui::Text("Current Price: %.5f | Range: %.5f - %.5f", 
                   close_prices.back(), min_price, max_price);
        
        // Simple price line chart first
        ImGui::PlotLines("Price", close_prices.data(), close_prices.size(),
                        0, nullptr, min_price * 0.999f, max_price * 1.001f, 
                        ImVec2(0, 200));
        
        // SEP Engine Metrics as 0-1 line chart
        renderSEPMetricChart(instrument);
    }
}

void MetricsDashboard::renderTechnicalIndicators(const std::string& instrument) {
    std::lock_guard<std::mutex> lock(market_data_mutex_);
    
    auto it = technical_indicators_.find(instrument);
    if (it == technical_indicators_.end()) {
        ImGui::Text("No technical indicators calculated for %s", instrument.c_str());
        return;
    }
    
    const auto& indicators = it->second;
    
    if (ImGui::BeginTable("TechnicalIndicators", 4, ImGuiTableFlags_Borders)) {
        ImGui::TableSetupColumn("Indicator");
        ImGui::TableSetupColumn("Current");
        ImGui::TableSetupColumn("Previous");
        ImGui::TableSetupColumn("Signal");
        ImGui::TableHeadersRow();
        
        // RSI
        if (!indicators.rsi.empty()) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("RSI");
            ImGui::TableSetColumnIndex(1);
            double current_rsi = indicators.rsi.back();
            ImGui::Text("%.2f", current_rsi);
            ImGui::TableSetColumnIndex(2);
            if (indicators.rsi.size() > 1) {
                ImGui::Text("%.2f", indicators.rsi[indicators.rsi.size()-2]);
            }
            ImGui::TableSetColumnIndex(3);
            if (current_rsi > 70) {
                ImGui::TextColored(ImVec4(1, 0, 0, 1), "Overbought");
            } else if (current_rsi < 30) {
                ImGui::TextColored(ImVec4(0, 1, 0, 1), "Oversold");
            } else {
                ImGui::Text("Neutral");
            }
        }
        
        // SMA
        if (!indicators.sma_20.empty()) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("SMA 20");
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%.5f", indicators.sma_20.back());
            ImGui::TableSetColumnIndex(2);
            if (indicators.sma_20.size() > 1) {
                ImGui::Text("%.5f", indicators.sma_20[indicators.sma_20.size()-2]);
            }
        }
        
        // MACD
        if (!indicators.macd.empty()) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("MACD");
            ImGui::TableSetColumnIndex(1);
            double macd_val = indicators.macd.back();
            ImGui::Text("%.5f", macd_val);
            ImGui::TableSetColumnIndex(2);
            if (indicators.macd.size() > 1) {
                ImGui::Text("%.5f", indicators.macd[indicators.macd.size()-2]);
            }
            ImGui::TableSetColumnIndex(3);
            if (!indicators.macd_signal.empty()) {
                double signal = indicators.macd_signal.back();
                if (macd_val > signal) {
                    ImGui::TextColored(ImVec4(0, 1, 0, 1), "Bullish");
                } else {
                    ImGui::TextColored(ImVec4(1, 0, 0, 1), "Bearish");
                }
            }
        }
        
        ImGui::EndTable();
    }
    
    // RSI chart
    if (show_rsi_ && !indicators.rsi.empty()) {
        std::vector<float> rsi_float(indicators.rsi.begin(), indicators.rsi.end());
        ImGui::Text("RSI (14)");
        ImGui::PlotLines("RSI", rsi_float.data(), rsi_float.size(),
                        0, nullptr, 0.0f, 100.0f, ImVec2(0, 100));
    }
    
    // MACD chart
    if (show_macd_ && !indicators.macd.empty()) {
        std::vector<float> macd_float(indicators.macd.begin(), indicators.macd.end());
        std::vector<float> signal_float(indicators.macd_signal.begin(), indicators.macd_signal.end());
        ImGui::Text("MACD");
        ImGui::PlotLines("MACD Line", macd_float.data(), macd_float.size(),
                        0, nullptr, FLT_MIN, FLT_MAX, ImVec2(0, 100));
        if (!signal_float.empty()) {
            ImGui::PlotLines("Signal Line", signal_float.data(), signal_float.size(),
                            0, nullptr, FLT_MIN, FLT_MAX, ImVec2(0, 100));
        }
    }
}

void MetricsDashboard::renderMarketDepth(const std::string& instrument) {
    std::lock_guard<std::mutex> lock(market_data_mutex_);
    
    auto it = market_depth_.find(instrument);
    if (it == market_depth_.end()) {
        ImGui::Text("No market depth data available for %s", instrument.c_str());
        return;
    }
    
    const auto& depth = it->second;
    
    ImGui::Text("Market Depth: %s", instrument.c_str());
    
    if (ImGui::BeginTable("MarketDepth", 4, ImGuiTableFlags_Borders)) {
        ImGui::TableSetupColumn("Bid Size");
        ImGui::TableSetupColumn("Bid Price");
        ImGui::TableSetupColumn("Ask Price");
        ImGui::TableSetupColumn("Ask Size");
        ImGui::TableHeadersRow();
        
        size_t max_rows = std::max(depth.bids.size(), depth.asks.size());
        for (size_t i = 0; i < max_rows && i < 10; ++i) {
            ImGui::TableNextRow();
            
            // Bids
            ImGui::TableSetColumnIndex(0);
            if (i < depth.bids.size()) {
                ImGui::Text("%.0f", depth.bids[i].second);
            }
            ImGui::TableSetColumnIndex(1);
            if (i < depth.bids.size()) {
                ImGui::Text("%.5f", depth.bids[i].first);
            }
            
            // Asks
            ImGui::TableSetColumnIndex(2);
            if (i < depth.asks.size()) {
                ImGui::Text("%.5f", depth.asks[i].first);
            }
            ImGui::TableSetColumnIndex(3);
            if (i < depth.asks.size()) {
                ImGui::Text("%.0f", depth.asks[i].second);
            }
        }
        
        ImGui::EndTable();
    }
}

void MetricsDashboard::renderSEPMetricChart(const std::string& instrument) {
    const auto& metrics = monitor_->getSystemMetrics();
    
    ImGui::Separator();
    ImGui::Text("SEP Engine Metrics (Real-time from OANDA)");
    
    // Add current metrics to member variables (already 0-1 normalized)
    coherence_history_.push_back(metrics.avg_coherence);
    stability_history_.push_back(metrics.avg_stability);
    entropy_history_.push_back(1.0f - metrics.avg_entropy); // Invert entropy so high = good
    
    // Limit history size
    if (coherence_history_.size() > max_history_size_) {
        coherence_history_.erase(coherence_history_.begin());
        stability_history_.erase(stability_history_.begin());
        entropy_history_.erase(entropy_history_.begin());
    }
    
    // Display current values
    ImGui::Text("Coherence: %.3f | Stability: %.3f | Predictability: %.3f", 
               metrics.avg_coherence, metrics.avg_stability, 1.0f - metrics.avg_entropy);
    
    // Plot 0-1 normalized metrics
    if (!coherence_history_.empty()) {
        ImGui::PlotLines("Coherence (0-1)", coherence_history_.data(), coherence_history_.size(),
                        0, nullptr, 0.0f, 1.0f, ImVec2(0, 100));
        
        ImGui::PlotLines("Stability (0-1)", stability_history_.data(), stability_history_.size(),
                        0, nullptr, 0.0f, 1.0f, ImVec2(0, 100));
        
        ImGui::PlotLines("Predictability (0-1)", entropy_history_.data(), entropy_history_.size(),
                        0, nullptr, 0.0f, 1.0f, ImVec2(0, 100));
    }
    
    // Data pipeline status
    ImGui::Separator();
    ImGui::Text("Data Pipeline Status:");
    ImGui::Text("• Engine processing: %s", monitor_ ? "✓ Active" : "✗ Inactive");
    ImGui::Text("• Active patterns: %zu", metrics.active_patterns);
    ImGui::Text("• Pattern rate: %.2f/sec", metrics.pattern_emergence_rate);
}

void MetricsDashboard::renderPatternMetrics() {
    const auto& metrics = monitor_->getSystemMetrics();
    
    ImGui::Text("SEP Pattern Analysis on Market Data");
    ImGui::Separator();
    
    // Display pattern metrics in context of trading
    if (ImGui::BeginTable("PatternTradingMetrics", 3, ImGuiTableFlags_Borders)) {
        ImGui::TableSetupColumn("Metric");
        ImGui::TableSetupColumn("Value");
        ImGui::TableSetupColumn("Trading Signal");
        ImGui::TableHeadersRow();
        
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("Pattern Coherence");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%.3f", metrics.avg_coherence);
        ImGui::TableSetColumnIndex(2);
        if (metrics.avg_coherence > 0.7) {
            ImGui::TextColored(ImVec4(0, 1, 0, 1), "Strong Trend");
        } else if (metrics.avg_coherence > 0.4) {
            ImGui::TextColored(ImVec4(1, 1, 0, 1), "Weak Trend");
        } else {
            ImGui::TextColored(ImVec4(1, 0, 0, 1), "Choppy Market");
        }
        
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("Pattern Stability");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%.3f", metrics.avg_stability);
        ImGui::TableSetColumnIndex(2);
        if (metrics.avg_stability > 0.6) {
            ImGui::TextColored(ImVec4(0, 1, 0, 1), "Stable Conditions");
        } else {
            ImGui::TextColored(ImVec4(1, 0, 0, 1), "Volatile Market");
        }
        
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("Market Entropy");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%.3f", metrics.avg_entropy);
        ImGui::TableSetColumnIndex(2);
        if (metrics.avg_entropy < 0.3) {
            ImGui::TextColored(ImVec4(0, 1, 0, 1), "Predictable");
        } else if (metrics.avg_entropy < 0.7) {
            ImGui::TextColored(ImVec4(1, 1, 0, 1), "Moderate");
        } else {
            ImGui::TextColored(ImVec4(1, 0, 0, 1), "High Uncertainty");
        }
        
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("Active Patterns");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%zu", metrics.active_patterns);
        ImGui::TableSetColumnIndex(2);
        if (metrics.active_patterns > 5) {
            ImGui::TextColored(ImVec4(0, 1, 0, 1), "Rich Pattern Environment");
        } else {
            ImGui::Text("Standard");
        }
        
        ImGui::EndTable();
    }
    
    // Pattern emergence rate as trading signal
    ImGui::Separator();
    ImGui::Text("Pattern Emergence Rate: %.2f patterns/sec", metrics.pattern_emergence_rate);
    if (metrics.pattern_emergence_rate > 1.0) {
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1, 1, 0, 1), "⚠ High Activity");
    }
}

void MetricsDashboard::updateCandlestickData(const std::string& instrument, const sep::connectors::MarketData& data) {
    std::lock_guard<std::mutex> lock(market_data_mutex_);
    
    // Store latest price
    latest_prices_[instrument] = data;
    
    // Create or update candlestick
    auto& candles = candlestick_data_[instrument];
    
    // For demo purposes, create simple candlesticks from tick data
    // In production, this would aggregate ticks into proper time-based candles
    Candlestick new_candle;
    new_candle.timestamp = data.timestamp;
    new_candle.open = data.mid;
    new_candle.high = data.mid;
    new_candle.low = data.mid;
    new_candle.close = data.mid;
    new_candle.volume = 1000; // Placeholder volume
    
    // If we have previous data, update high/low
    if (!candles.empty()) {
        auto& last_candle = candles.back();
        // If same minute, update the current candle
        if (static_cast<int>(new_candle.timestamp / 60) == static_cast<int>(last_candle.timestamp / 60)) {
            last_candle.high = std::max(last_candle.high, data.mid);
            last_candle.low = std::min(last_candle.low, data.mid);
            last_candle.close = data.mid;
            last_candle.timestamp = data.timestamp;
        } else {
            // New minute, add new candle
            candles.push_back(new_candle);
        }
    } else {
        candles.push_back(new_candle);
    }
    
    // Limit history
    while (candles.size() > max_candlesticks_) {
        candles.pop_front();
    }
    
    // Update technical indicators
    calculateTechnicalIndicators(instrument);
}

void MetricsDashboard::calculateTechnicalIndicators(const std::string& instrument) {
    std::lock_guard<std::mutex> lock(market_data_mutex_);
    
    auto candle_it = candlestick_data_.find(instrument);
    if (candle_it == candlestick_data_.end() || candle_it->second.size() < 20) {
        return; // Need at least 20 periods for calculations
    }
    
    const auto& candles = candle_it->second;
    auto& indicators = technical_indicators_[instrument];
    
    // Clear existing indicators
    indicators.sma_20.clear();
    indicators.sma_50.clear();
    indicators.ema_12.clear();
    indicators.ema_26.clear();
    indicators.rsi.clear();
    indicators.macd.clear();
    indicators.macd_signal.clear();
    indicators.bollinger_upper.clear();
    indicators.bollinger_lower.clear();
    indicators.atr.clear();
    indicators.timestamps.clear();
    
    std::vector<double> closes;
    for (const auto& candle : candles) {
        closes.push_back(candle.close);
        indicators.timestamps.push_back(candle.timestamp);
    }
    
    // Calculate SMA 20
    for (size_t i = 19; i < closes.size(); ++i) {
        double sum = 0;
        for (size_t j = i - 19; j <= i; ++j) {
            sum += closes[j];
        }
        indicators.sma_20.push_back(sum / 20.0);
    }
    
    // Calculate SMA 50
    if (closes.size() >= 50) {
        for (size_t i = 49; i < closes.size(); ++i) {
            double sum = 0;
            for (size_t j = i - 49; j <= i; ++j) {
                sum += closes[j];
            }
            indicators.sma_50.push_back(sum / 50.0);
        }
    }
    
    // Calculate RSI (14 periods)
    if (closes.size() >= 15) {
        std::vector<double> gains, losses;
        for (size_t i = 1; i < closes.size(); ++i) {
            double change = closes[i] - closes[i-1];
            gains.push_back(change > 0 ? change : 0);
            losses.push_back(change < 0 ? -change : 0);
        }
        
        for (size_t i = 13; i < gains.size(); ++i) {
            double avg_gain = 0, avg_loss = 0;
            for (size_t j = i - 13; j <= i; ++j) {
                avg_gain += gains[j];
                avg_loss += losses[j];
            }
            avg_gain /= 14.0;
            avg_loss /= 14.0;
            
            if (avg_loss == 0) {
                indicators.rsi.push_back(100);
            } else {
                double rs = avg_gain / avg_loss;
                double rsi = 100 - (100 / (1 + rs));
                indicators.rsi.push_back(rsi);
            }
        }
    }
    
    // Calculate Bollinger Bands (20 period, 2 std dev)
    if (indicators.sma_20.size() > 0) {
        for (size_t i = 0; i < indicators.sma_20.size(); ++i) {
            size_t close_idx = i + 19; // SMA starts at index 19
            double sma = indicators.sma_20[i];
            
            // Calculate standard deviation
            double variance = 0;
            for (size_t j = close_idx - 19; j <= close_idx; ++j) {
                variance += (closes[j] - sma) * (closes[j] - sma);
            }
            double std_dev = sqrt(variance / 20.0);
            
            indicators.bollinger_upper.push_back(sma + 2.0 * std_dev);
            indicators.bollinger_lower.push_back(sma - 2.0 * std_dev);
        }
    }
    
    // Simple MACD calculation (12-26-9)
    if (closes.size() >= 26) {
        // Calculate EMA 12 and EMA 26
        double multiplier_12 = 2.0 / (12 + 1);
        double multiplier_26 = 2.0 / (26 + 1);
        
        std::vector<double> ema12, ema26;
        ema12.push_back(closes[0]);
        ema26.push_back(closes[0]);
        
        for (size_t i = 1; i < closes.size(); ++i) {
            double ema12_val = (closes[i] * multiplier_12) + (ema12.back() * (1 - multiplier_12));
            double ema26_val = (closes[i] * multiplier_26) + (ema26.back() * (1 - multiplier_26));
            ema12.push_back(ema12_val);
            ema26.push_back(ema26_val);
        }
        
        // Calculate MACD line
        for (size_t i = 25; i < ema12.size(); ++i) {
            indicators.macd.push_back(ema12[i] - ema26[i]);
        }
        
        // Calculate MACD signal line (9-period EMA of MACD)
        if (indicators.macd.size() >= 9) {
            double signal_multiplier = 2.0 / (9 + 1);
            indicators.macd_signal.push_back(indicators.macd[0]);
            
            for (size_t i = 1; i < indicators.macd.size(); ++i) {
                double signal = (indicators.macd[i] * signal_multiplier) + 
                              (indicators.macd_signal.back() * (1 - signal_multiplier));
                indicators.macd_signal.push_back(signal);
            }
        }
    }
}

void MetricsDashboard::renderOHLCCandlesticks(const std::deque<Candlestick>& candles, float min_price, float max_price, float price_range) {
    // Enhanced OHLC candlestick visualization using ImGui drawing
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 canvas_pos = ImGui::GetCursorScreenPos();
    ImVec2 canvas_size = ImVec2(ImGui::GetContentRegionAvail().x, 300);
    
    // Ensure we have a minimum width
    if (canvas_size.x < 200) canvas_size.x = 200;
    
    ImGui::InvisibleButton("candlestick_canvas", canvas_size);
    
    if (candles.empty()) return;
    
    // Price range with padding
    float price_min = min_price - price_range * 0.05f;
    float price_max = max_price + price_range * 0.05f;
    float effective_range = price_max - price_min;
    
    // Calculate candle width and spacing
    float candle_width = canvas_size.x / (float)candles.size() * 0.8f;
    float candle_spacing = canvas_size.x / (float)candles.size();
    
    // Draw background
    draw_list->AddRectFilled(canvas_pos, 
                           ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), 
                           IM_COL32(25, 25, 35, 255));
    
    // Draw grid lines
    for (int i = 1; i < 5; ++i) {
        float y = canvas_pos.y + (canvas_size.y * i / 5.0f);
        draw_list->AddLine(ImVec2(canvas_pos.x, y), 
                          ImVec2(canvas_pos.x + canvas_size.x, y), 
                          IM_COL32(60, 60, 70, 100));
    }
    
    // Draw candlesticks
    for (size_t i = 0; i < candles.size(); ++i) {
        const auto& candle = candles[i];
        
        float x = canvas_pos.x + (i * candle_spacing) + (candle_spacing - candle_width) * 0.5f;
        
        // Convert prices to screen coordinates
        auto priceToY = [&](double price) {
            return canvas_pos.y + canvas_size.y - ((price - price_min) / effective_range) * canvas_size.y;
        };
        
        float open_y = priceToY(candle.open);
        float close_y = priceToY(candle.close);
        float high_y = priceToY(candle.high);
        float low_y = priceToY(candle.low);
        
        // Determine colors (green for up, red for down)
        bool is_up = candle.close > candle.open;
        ImU32 body_color = is_up ? IM_COL32(0, 200, 0, 200) : IM_COL32(200, 0, 0, 200);
        ImU32 wick_color = is_up ? IM_COL32(0, 150, 0, 255) : IM_COL32(150, 0, 0, 255);
        
        // Draw high-low wick
        float wick_x = x + candle_width * 0.5f;
        draw_list->AddLine(ImVec2(wick_x, high_y), ImVec2(wick_x, low_y), wick_color, 1.0f);
        
        // Draw body
        float body_top = std::min(open_y, close_y);
        float body_bottom = std::max(open_y, close_y);
        float body_height = body_bottom - body_top;
        
        if (body_height < 1.0f) body_height = 1.0f; // Minimum height for doji candles
        
        draw_list->AddRectFilled(ImVec2(x, body_top), 
                               ImVec2(x + candle_width, body_bottom), 
                               body_color);
        
        // Draw border
        draw_list->AddRect(ImVec2(x, body_top), 
                          ImVec2(x + candle_width, body_bottom), 
                          wick_color, 0.0f, 0, 1.0f);
    }
    
    // Draw price labels
    for (int i = 0; i <= 4; ++i) {
        float price = price_min + (effective_range * i / 4.0f);
        float y = canvas_pos.y + canvas_size.y - (canvas_size.y * i / 4.0f);
        char price_str[32];
        snprintf(price_str, sizeof(price_str), "%.5f", price);
        draw_list->AddText(ImVec2(canvas_pos.x + canvas_size.x + 5, y - 8), 
                          IM_COL32(200, 200, 200, 255), price_str);
    }
}

void MetricsDashboard::renderSEPSignalOverlay(const std::string& instrument, float min_price, float max_price, float price_range) {
    // Get current SEP metrics for signal generation
    const auto& metrics = monitor_->getSystemMetrics();
    
    // Create SEP signal data based on pattern metrics
    std::vector<float> sep_signals;
    auto it = candlestick_data_.find(instrument);
    if (it != candlestick_data_.end() && !it->second.empty()) {
        const auto& candles = it->second;
        
        // Generate signals based on coherence, stability, and entropy
        for (size_t i = 0; i < candles.size(); ++i) {
            // Create a normalized signal (0.0 to 1.0) based on pattern metrics
            float coherence_signal = metrics.avg_coherence;
            float stability_signal = metrics.avg_stability;
            float entropy_signal = 1.0f - metrics.avg_entropy; // Invert entropy (lower = better)
            
            // Combine signals with weights
            float combined_signal = (coherence_signal * 0.4f + stability_signal * 0.3f + entropy_signal * 0.3f);
            
            // Add some variation based on position in the data
            float trend_factor = std::sin((float)i / candles.size() * 3.14159f * 2.0f) * 0.1f;
            combined_signal += trend_factor;
            
            // Clamp to valid range
            combined_signal = std::max(0.0f, std::min(1.0f, combined_signal));
            
            // Convert to price range
            float signal_price = min_price + (combined_signal * price_range);
            sep_signals.push_back(signal_price);
        }
        
        // Render SEP signal line
        if (!sep_signals.empty()) {
            ImGui::PushStyleColor(ImGuiCol_PlotLines, ImVec4(1.0f, 1.0f, 0.0f, 0.8f)); // Yellow signal line
            ImGui::PlotLines("SEP Signal", sep_signals.data(), sep_signals.size(),
                           0, nullptr, min_price - price_range * 0.1f, max_price + price_range * 0.1f,
                           ImVec2(0, 200));
            ImGui::PopStyleColor();
        }
        
        // Add signal interpretation text
        ImGui::Text("SEP Pattern Analysis:");
        ImGui::SameLine();
        if (metrics.avg_coherence > 0.7f) {
            ImGui::TextColored(ImVec4(0, 1, 0, 1), "STRONG TREND");
        } else if (metrics.avg_coherence > 0.4f) {
            ImGui::TextColored(ImVec4(1, 1, 0, 1), "WEAK TREND");
        } else {
            ImGui::TextColored(ImVec4(1, 0, 0, 1), "CHOPPY");
        }
        
        // Pattern emergence rate indicator
        if (metrics.pattern_emergence_rate > 1.0) {
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(1, 0.5f, 0, 1), " | HIGH ACTIVITY");
        }
        
        // Add real-time signal strength meter
        ImGui::Text("Signal Strength: ");
        ImGui::SameLine();
        float current_signal = sep_signals.empty() ? 0.0f : sep_signals.back();
        float normalized_strength = (current_signal - min_price) / price_range;
        ImGui::ProgressBar(normalized_strength, ImVec2(200, 0), "");
        
        // SEP engine parameter controls
        if (ImGui::CollapsingHeader("SEP Engine Controls")) {
            ImGui::Text("Pattern Retention Controls:");
            
            static float pattern_quality_threshold = 0.5f;
            static int max_patterns = 100;
            static float coherence_threshold = 0.3f;
            
            if (ImGui::SliderFloat("Quality Threshold", &pattern_quality_threshold, 0.0f, 1.0f)) {
                // TODO: Apply to SEP engine
            }
            
            if (ImGui::SliderInt("Max Patterns", &max_patterns, 10, 1000)) {
                // TODO: Apply to SEP engine
            }
            
            if (ImGui::SliderFloat("Coherence Filter", &coherence_threshold, 0.0f, 1.0f)) {
                // TODO: Apply to SEP engine
            }
            
            if (ImGui::Button("Clear Pattern Cache")) {
                // TODO: Clear SEP engine pattern cache
                ImGui::Text("Pattern cache cleared!");
            }
            
            ImGui::SameLine();
            if (ImGui::Button("Reset Engine")) {
                // TODO: Reset SEP engine state
                ImGui::Text("Engine reset!");
            }
        }
    }
}

void MetricsDashboard::renderOandaMainView() {
    ImGui::Text("OANDA Trading Platform - Live Market Data");
    ImGui::Separator();
    
    // Connection status bar
    if (oanda_connected_) {
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "● Connected to OANDA");
    } else {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "● Disconnected - Check API credentials");
        return;
    }
    
    // Live data toggle button
    ImGui::SameLine();
    if (ImGui::Button(live_data_enabled_ ? "Disable Live Data" : "Enable Live Data")) {
        live_data_enabled_ = !live_data_enabled_;
        if (live_data_enabled_ && oanda_connector_) {
            oanda_connector_->startPriceStream({selected_instrument_});
        } else if (oanda_connector_) {
            oanda_connector_->stopPriceStream();
        }
    }
    
    // Instrument selector
    ImGui::SameLine();
    ImGui::SetNextItemWidth(120);
    const char* instruments[] = {"EUR_USD", "GBP_USD", "USD_JPY", "AUD_USD", "USD_CAD", "USD_CHF"};
    static int current_instrument = 0;
    if (ImGui::Combo("Instrument", &current_instrument, instruments, IM_ARRAYSIZE(instruments))) {
        selected_instrument_ = instruments[current_instrument];
        historical_data_loaded_ = false; // Force reload historical data
    }
    
    // Load historical data if not loaded
    if (!historical_data_loaded_ && oanda_connector_) {
        fetchHistoricalData();
    }
    
    // Main chart area
    ImVec2 chart_size = ImVec2(ImGui::GetContentRegionAvail().x, 400);
    
    if (ImGui::BeginChild("MainChart", chart_size, true)) {
        if (historical_data_loaded_ && !historical_data_.empty()) {
            // Display the time range of the cached candles (48 hours)
            ImGui::Text("Historical Chart (48hr) for %s", selected_instrument_.c_str());
            ImGui::Text("Candles: %zu", historical_data_.size());
            
            // Chart info header
            auto& first_candle = historical_data_.front();
            auto& last_candle = historical_data_.back();
            
            double price_change = last_candle.close - first_candle.open;
            double price_change_pct = (price_change / first_candle.open) * 100.0;
            
            ImVec4 change_color = price_change >= 0 ? ImVec4(0, 1, 0, 1) : ImVec4(1, 0, 0, 1);
            ImGui::Text("O: %.5f H: %.5f L: %.5f C: %.5f", 
                       last_candle.open, last_candle.high, last_candle.low, last_candle.close);
            ImGui::SameLine();
            ImGui::TextColored(change_color, "Change: %.5f (%.2f%%)", price_change, price_change_pct);
            
            ImGui::Separator();
            
            // Render the actual candlestick chart
            renderHistoricalChart();
            
        } else {
            ImGui::Text("Loading historical data for %s...", selected_instrument_.c_str());
        }
    }
    ImGui::EndChild();
    
    // SEP Metrics panel below chart
    if (ImGui::BeginChild("SEPMetrics", ImVec2(ImGui::GetContentRegionAvail().x, 200), true)) {
        // Metrics calculated from the cached 48hr window
        ImGui::Text("48hr Market Snapshot Metrics");
        ImGui::Separator();
        
        // Show cached snapshot metrics
        auto cache_it = instrument_cache_.find(selected_instrument_);
        if (cache_it != instrument_cache_.end() && cache_it->second.is_valid) {
            const auto& cache = cache_it->second;
            
            ImGui::Columns(3, "SnapshotColumns");
            ImGui::Text("Coherence");
            ImGui::NextColumn();
            ImGui::Text("Stability");
            ImGui::NextColumn();
            ImGui::Text("Entropy");
            ImGui::NextColumn();
            ImGui::Separator();
            
            ImVec4 coherence_color = cache.coherence_metric > 0.6 ? ImVec4(0, 1, 0, 1) : 
                                   cache.coherence_metric > 0.3 ? ImVec4(1, 1, 0, 1) : ImVec4(1, 0, 0, 1);
            ImVec4 stability_color = cache.stability_metric > 0.6 ? ImVec4(0, 1, 0, 1) : 
                                   cache.stability_metric > 0.3 ? ImVec4(1, 1, 0, 1) : ImVec4(1, 0, 0, 1);
            ImVec4 entropy_color = cache.entropy_metric < 0.3 ? ImVec4(0, 1, 0, 1) : 
                                 cache.entropy_metric < 0.7 ? ImVec4(1, 1, 0, 1) : ImVec4(1, 0, 0, 1);
            
            ImGui::TextColored(coherence_color, "%.3f", cache.coherence_metric);
            ImGui::NextColumn();
            ImGui::TextColored(stability_color, "%.3f", cache.stability_metric);
            ImGui::NextColumn();
            ImGui::TextColored(entropy_color, "%.3f", cache.entropy_metric);
            ImGui::Columns(1);
            
            ImGui::Separator();
            
            // Show cache info
            auto time_since_calc = std::chrono::duration_cast<std::chrono::minutes>(
                std::chrono::system_clock::now() - cache.metrics_calculated).count();
            ImGui::Text("Data Points: %zu | Calculated: %ld min ago", 
                       cache.minute_data.size(), time_since_calc);
        } else {
            ImGui::Text("Loading 48hr snapshot metrics...");
        }
        
        ImGui::Separator();
        
        // Show live SEP engine metrics if available
        if (monitor_) {
            const auto& metrics = monitor_->getSystemMetrics();
            ImGui::Text("Live SEP Engine: C:%.3f S:%.3f E:%.3f | %zu patterns", 
                       metrics.avg_coherence, metrics.avg_stability, 
                       metrics.avg_entropy, metrics.active_patterns);
        }
    }
    ImGui::EndChild();
}

void MetricsDashboard::fetchHistoricalData() {
    if (!oanda_connector_) return;
    
    // Check if we need to update the cache for this instrument
    updateInstrumentCache(selected_instrument_);
    
    // Use cached data for display
    auto& cache = instrument_cache_[selected_instrument_];
    if (cache.is_valid && !cache.minute_data.empty()) {
        historical_data_ = cache.minute_data;
        historical_data_loaded_ = true;
        std::cout << "[MetricsDashboard] Using cached data: " << historical_data_.size() << " minute candles" << std::endl;
        
        // Debug: Print sample data points
        if (historical_data_.size() > 0) {
            std::cout << "[DEBUG] First candle: O=" << historical_data_[0].open 
                     << " H=" << historical_data_[0].high 
                     << " L=" << historical_data_[0].low 
                     << " C=" << historical_data_[0].close << std::endl;
            if (historical_data_.size() > 1) {
                auto last_idx = historical_data_.size() - 1;
                std::cout << "[DEBUG] Last candle: O=" << historical_data_[last_idx].open 
                         << " H=" << historical_data_[last_idx].high 
                         << " L=" << historical_data_[last_idx].low 
                         << " C=" << historical_data_[last_idx].close << std::endl;
            }
        }
    } else {
        historical_data_loaded_ = false;
        std::cout << "[MetricsDashboard] Cache not ready for " << selected_instrument_ << std::endl;
    }
}

void MetricsDashboard::renderHistoricalChart() {
    if (historical_data_.empty()) return;
    
    ImVec2 canvas_pos = ImGui::GetCursorScreenPos();
    ImVec2 canvas_size = ImGui::GetContentRegionAvail();
    canvas_size.y = std::max(canvas_size.y - 50.0f, 200.0f); // Reserve space for bottom info
    
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    
    // Chart background
    ImU32 bg_color = IM_COL32(25, 25, 35, 255);
    draw_list->AddRectFilled(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), bg_color);
    
    // Chart border
    ImU32 border_color = IM_COL32(100, 100, 120, 255);
    draw_list->AddRect(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), border_color);
    
    // Calculate price range
    double min_price = std::numeric_limits<double>::max();
    double max_price = std::numeric_limits<double>::min();
    
    for (const auto& candle : historical_data_) {
        min_price = std::min(min_price, candle.low);
        max_price = std::max(max_price, candle.high);
    }
    
    // Add some padding to the price range
    double price_range = max_price - min_price;
    double padding = price_range * 0.1;
    min_price -= padding;
    max_price += padding;
    price_range = max_price - min_price;
    
    // Chart area (leave margins for axes)
    float margin_left = 80.0f;
    float margin_right = 20.0f;
    float margin_top = 20.0f;
    float margin_bottom = 40.0f;
    
    ImVec2 chart_pos = ImVec2(canvas_pos.x + margin_left, canvas_pos.y + margin_top);
    ImVec2 chart_size = ImVec2(canvas_size.x - margin_left - margin_right, 
                               canvas_size.y - margin_top - margin_bottom);
    
    // Draw price grid lines and labels
    const int num_price_lines = 5;
    for (int i = 0; i <= num_price_lines; i++) {
        double price = min_price + (price_range * i / num_price_lines);
        float y = chart_pos.y + chart_size.y - (chart_size.y * i / num_price_lines);
        
        // Grid line
        ImU32 grid_color = IM_COL32(60, 60, 70, 128);
        draw_list->AddLine(ImVec2(chart_pos.x, y), 
                          ImVec2(chart_pos.x + chart_size.x, y), grid_color);
        
        // Price label
        char price_text[32];
        sprintf(price_text, "%.5f", price);
        draw_list->AddText(ImVec2(canvas_pos.x + 5, y - 8), IM_COL32(180, 180, 190, 255), price_text);
    }
    
    // Draw candlesticks
    if (historical_data_.size() > 0) {
        float candle_width = chart_size.x / historical_data_.size();
        float wick_width = 1.0f;
        float body_width = std::max(2.0f, candle_width * 0.8f);
        
        // Debug chart rendering
        static bool debug_printed = false;
        if (!debug_printed) {
            std::cout << "[DEBUG] Chart rendering - Data points: " << historical_data_.size() 
                     << ", Chart width: " << chart_size.x 
                     << ", Candle width: " << candle_width 
                     << ", Body width: " << body_width << std::endl;
            debug_printed = true;
        }
        
        for (size_t i = 0; i < historical_data_.size(); i++) {
            const auto& candle = historical_data_[i];
            
            float x = chart_pos.x + (i * candle_width) + (candle_width * 0.5f);
            
            // Calculate Y positions
            float high_y = chart_pos.y + chart_size.y - ((candle.high - min_price) / price_range) * chart_size.y;
            float low_y = chart_pos.y + chart_size.y - ((candle.low - min_price) / price_range) * chart_size.y;
            float open_y = chart_pos.y + chart_size.y - ((candle.open - min_price) / price_range) * chart_size.y;
            float close_y = chart_pos.y + chart_size.y - ((candle.close - min_price) / price_range) * chart_size.y;
            
            // Determine candle color (green for bullish, red for bearish)
            bool is_bullish = candle.close > candle.open;
            ImU32 candle_color = is_bullish ? IM_COL32(34, 197, 94, 255) : IM_COL32(239, 68, 68, 255);
            
            // Draw wick (high-low line)
            draw_list->AddLine(ImVec2(x, high_y), ImVec2(x, low_y), candle_color, wick_width);
            
            // Draw body (open-close rectangle)
            float body_top = std::min(open_y, close_y);
            float body_bottom = std::max(open_y, close_y);
            
            if (std::abs(body_bottom - body_top) < 1.0f) {
                // Doji candle - draw a line
                draw_list->AddLine(ImVec2(x - body_width/2, open_y), 
                                  ImVec2(x + body_width/2, open_y), candle_color, 2.0f);
            } else {
                // Normal candle body
                ImVec2 body_min = ImVec2(x - body_width/2, body_top);
                ImVec2 body_max = ImVec2(x + body_width/2, body_bottom);
                
                if (is_bullish) {
                    // Hollow green candle for bullish
                    draw_list->AddRect(body_min, body_max, candle_color, 0.0f, 0, 2.0f);
                } else {
                    // Filled red candle for bearish
                    draw_list->AddRectFilled(body_min, body_max, candle_color);
                }
            }
        }
    }
    
    // Time axis (simple for now)
    if (!historical_data_.empty()) {
        // First candle time
        // Indicate the start of the cached window (48 hours ago)
        draw_list->AddText(ImVec2(chart_pos.x, chart_pos.y + chart_size.y + 5),
                          IM_COL32(180, 180, 190, 255), "48hr ago");
        
        // Last candle time  
        draw_list->AddText(ImVec2(chart_pos.x + chart_size.x - 30, chart_pos.y + chart_size.y + 5), 
                          IM_COL32(180, 180, 190, 255), "Now");
    }
    
    // Reserve space for the chart
    ImGui::SetCursorScreenPos(ImVec2(canvas_pos.x, canvas_pos.y + canvas_size.y));
    ImGui::Dummy(canvas_size);
}

void MetricsDashboard::updateInstrumentCache(const std::string& instrument) {
    auto& cache = instrument_cache_[instrument];
    
    // Check if cache needs updating
    if (!needsCacheUpdate(cache)) {
        return;
    }
    
    if (!oanda_connector_) {
        std::cerr << "[MetricsDashboard] OANDA connector not available for cache update" << std::endl;
        return;
    }
    
    // Get current time and 48 hours ago (2 days for sliding window)
    auto now = std::chrono::system_clock::now();
    auto yesterday = now - std::chrono::hours(48);
    
    // Format times for OANDA API (ISO 8601) 
    // Ensure they're different by adding a small offset to 'to' time
    auto to_time = now + std::chrono::minutes(1); // Slightly in the future
    
    auto from_time_t = std::chrono::system_clock::to_time_t(yesterday);
    auto to_time_t = std::chrono::system_clock::to_time_t(to_time);
    
    std::tm* from_tm = std::gmtime(&from_time_t);
    std::tm* to_tm = std::gmtime(&to_time_t);
    
    char now_str[32], yesterday_str[32];
    std::strftime(now_str, sizeof(now_str), "%Y-%m-%dT%H:%M:%SZ", to_tm);
    std::strftime(yesterday_str, sizeof(yesterday_str), "%Y-%m-%dT%H:%M:%SZ", from_tm);
    
    try {
        std::cout << "[MetricsDashboard] Updating 48hr cache for " << instrument << " (M1 data)" << std::endl;
        
        // Fetch minute-by-minute data for 48 hours
        auto new_data = oanda_connector_->getHistoricalData(
            instrument, 
            "M1",  // 1-minute granularity
            yesterday_str, 
            now_str,
            0  // Don't specify count when using from/to
        );
        
        if (!new_data.empty()) {
            // Update cache with sliding window approach
            cache.minute_data = std::move(new_data);
            cache.instrument = instrument;
            cache.last_update = now;
            cache.is_valid = true;
            
            // Calculate coherence metrics for this snapshot
            calculateSnapshotMetrics(cache);
            
            std::cout << "[MetricsDashboard] Cache updated: " << cache.minute_data.size() 
                     << " minutes, Coherence: " << cache.coherence_metric << std::endl;
        } else {
            std::cerr << "[MetricsDashboard] No data received for " << instrument << std::endl;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "[MetricsDashboard] Error updating cache for " << instrument << ": " << e.what() << std::endl;
        cache.is_valid = false;
    }
}

bool MetricsDashboard::needsCacheUpdate(const Chart24HrCache& cache) {
    if (!cache.is_valid) {
        return true; // Invalid cache needs update
    }
    
    auto now = std::chrono::system_clock::now();
    auto time_since_update = std::chrono::duration_cast<std::chrono::minutes>(now - cache.last_update);
    
    return time_since_update >= cache_update_interval_;
}

void MetricsDashboard::calculateSnapshotMetrics(Chart24HrCache& cache) {
    if (cache.minute_data.empty()) {
        cache.coherence_metric = 0.0;
        cache.stability_metric = 0.0;
        cache.entropy_metric = 1.0; // High entropy for no data
        return;
    }
    
    // Calculate price volatility-based coherence
    std::vector<double> price_changes;
    for (size_t i = 1; i < cache.minute_data.size(); ++i) {
        double change = (cache.minute_data[i].close - cache.minute_data[i-1].close) / cache.minute_data[i-1].close;
        price_changes.push_back(change);
    }
    
    if (price_changes.empty()) {
        cache.coherence_metric = 0.0;
        cache.stability_metric = 0.0;
        cache.entropy_metric = 1.0;
        return;
    }
    
    // Calculate mean and standard deviation of price changes
    double mean = std::accumulate(price_changes.begin(), price_changes.end(), 0.0) / price_changes.size();
    
    double variance = 0.0;
    for (double change : price_changes) {
        variance += (change - mean) * (change - mean);
    }
    variance /= price_changes.size();
    double std_dev = std::sqrt(variance);
    
    // Coherence: inverse of volatility (lower volatility = higher coherence)
    cache.coherence_metric = std::max(0.0, std::min(1.0, 1.0 - (std_dev * 100.0)));
    
    // Stability: consistency of price movement direction
    int trend_changes = 0;
    for (size_t i = 1; i < price_changes.size(); ++i) {
        if ((price_changes[i] > 0) != (price_changes[i-1] > 0)) {
            trend_changes++;
        }
    }
    cache.stability_metric = std::max(0.0, std::min(1.0, 1.0 - (double(trend_changes) / price_changes.size())));
    
    // Entropy: randomness of price movements
    cache.entropy_metric = std::max(0.0, std::min(1.0, std_dev * 50.0)); // Normalized entropy
    
    cache.metrics_calculated = std::chrono::system_clock::now();
    
    std::cout << "[MetricsDashboard] Snapshot metrics calculated - C:" << cache.coherence_metric 
             << " S:" << cache.stability_metric << " E:" << cache.entropy_metric << std::endl;
}

} // namespace sep::workbench
