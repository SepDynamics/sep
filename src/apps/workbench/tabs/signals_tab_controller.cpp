#include "signals_tab_controller.h"
#include "apps/workbench/core/workbench_core.hpp"
#include "quantum/pattern_metric_engine.h"
#include "apps/workbench/config.hpp"
#include "core/metrics_monitor.h"

#include <implot.h>
#include "imgui_internal.h"
#include "apps/workbench/implot_extension.h"
#include <algorithm>
#include <numeric>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <limits>

static void drawDashedHorizontal(ImDrawList* dl, float x1, float x2, float y,
                                 ImU32 col, float dash = 4.0f, float gap = 4.0f) {
    float x = x1;
    while (x < x2) {
        float x_end = std::min(x + dash, x2);
        dl->AddLine(ImVec2(x, y), ImVec2(x_end, y), col);
        x += dash + gap;
    }
}

namespace sep::workbench {

SignalsTabController::SignalsTabController()
    : price_min_(0), price_max_(0), volume_max_(0) {}

SignalsTabController::~SignalsTabController() {
    shutdown();
}

bool SignalsTabController::initialize() {
    std::cout << "[SignalsTabController] Initializing..." << std::endl;
    const auto& th = Config::getInstance().signal_thresholds();
    buy_min_coherence_ = th.buy_min_coherence;
    buy_min_stability_ = th.buy_min_stability;
    buy_max_entropy_ = th.buy_max_entropy;
    sell_max_stability_ = th.sell_max_stability;
    sell_min_entropy_ = th.sell_min_entropy;
    // Ensure dependent components are routed through the standard setters
    setMetricsMonitor(metrics_monitor_);
    setQuantumSignalGenerator(signal_generator_);
    ImPlot::GetStyle().UseISO8601 = true;
    ImPlot::GetStyle().UseLocalTime = true;
    ImPlotInputMap& map = ImPlot::GetInputMap();
    map.Pan = ImGuiMouseButton_Middle;
    map.Fit = ImGuiMouseButton_Right;
    map.Select = ImGuiMouseButton_Left;
    map.Menu = ImGuiMouseButton_Right;
    map.ZoomMod = ImGuiMod_Ctrl;
    plot_flags_ = ImPlotFlags_Crosshairs | ImPlotFlags_NoMenus | ImPlotFlags_NoLegend;

    if (metrics_monitor_) {
        metrics_monitor_->startProcessing();
    }
    return true;
}

void SignalsTabController::renderThresholdControlPanel() {
    ImGui::Begin("Signal Controls");
    ImGui::Text("BUY thresholds");
    bool changed = false;
    changed |= ImGui::SliderFloat("Buy Coherence", &buy_min_coherence_, 0.0f, 1.0f);
    changed |= ImGui::SliderFloat("Buy Stability", &buy_min_stability_, 0.0f, 1.0f);
    changed |= ImGui::SliderFloat("Buy Max Entropy", &buy_max_entropy_, 0.0f, 1.0f);
    ImGui::Separator();
    ImGui::Text("SELL thresholds");
    changed |= ImGui::SliderFloat("Sell Max Stability", &sell_max_stability_, 0.0f, 1.0f);
    changed |= ImGui::SliderFloat("Sell Min Entropy", &sell_min_entropy_, 0.0f, 1.0f);
    if (changed || ImGui::Button("Apply")) {
        if (workbench_engine_) {
            auto* pme = workbench_engine_->getPatternMetricEngine();
            if (pme) {
                quantum::SignalThresholds thresholds;
                thresholds.buy_min_coherence = buy_min_coherence_;
                thresholds.buy_min_stability = buy_min_stability_;
                thresholds.buy_max_entropy = buy_max_entropy_;
                thresholds.sell_max_stability = sell_max_stability_;
                thresholds.sell_min_entropy = sell_min_entropy_;
                pme->setSignalThresholds(thresholds);
                // Config::getInstance().set_signal_thresholds(thresholds); // This is the corrected line
                Config::getInstance().save("src/apps/workbench/config.json");
            }
        }
    }
    ImGui::End();
}

void SignalsTabController::render() {
    ImGui::Columns(2, "SignalsColumns", true);
    renderThresholdControlPanel();
    ImGui::NextColumn();

    if (metrics_monitor_) {
        (void)ImGui::Begin("SEP Engine Metrics");
        const auto& system_metrics = metrics_monitor_->getSystemMetrics();
        const auto& rolling_metrics = metrics_monitor_->getRollingMetrics();

        ImGui::Text("Avg Coherence: %.3f", system_metrics.avg_coherence);
        ImGui::Text("Avg Stability: %.3f", system_metrics.avg_stability);
        ImGui::Text("Avg Entropy: %.3f", system_metrics.avg_entropy);

        ImGui::Separator();
        ImGui::Text("1-Hour Rolling Averages");
        ImGui::Text("Coherence: %.3f", rolling_metrics.coherence_1h_avg);
        ImGui::Text("Stability: %.3f", rolling_metrics.stability_1h_avg);
        ImGui::Text("Entropy: %.3f", rolling_metrics.entropy_1h_avg);

        ImGui::Separator();
        ImGui::Text("24-Hour Rolling Averages");
        ImGui::Text("Coherence: %.3f", rolling_metrics.coherence_24h_avg);
        ImGui::Text("Stability: %.3f", rolling_metrics.stability_24h_avg);
        ImGui::Text("Entropy: %.3f", rolling_metrics.entropy_24h_avg);

        ImGui::End();
    }

    // Real data fetching implementation
    if (oanda_connector_) {
        try {
            // Fetch latest 24 hours of EUR/USD M1 data using correct method
            auto now = std::chrono::system_clock::now();
            auto day_ago = now - std::chrono::hours(24);
            
            // Format time strings for OANDA API
            auto now_t = std::chrono::system_clock::to_time_t(now);
            auto day_ago_t = std::chrono::system_clock::to_time_t(day_ago);
            
            std::string from_time = std::to_string(day_ago_t);
            std::string to_time = std::to_string(now_t);
            
            auto latest_candles = oanda_connector_->getHistoricalData("EUR_USD", "M1", from_time, to_time, 1440);
            if (!latest_candles.empty()) {
                // Update our candle data buffer with new data
                for (const auto& oanda_candle : latest_candles) {
                    // Parse OANDA timestamp (RFC 3339 format)
                    std::tm tm = {};
                    std::istringstream ss(oanda_candle.time);
                    ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S");
                    auto timestamp = std::chrono::system_clock::from_time_t(std::mktime(&tm));
                    
                    // Create CandleData using proper constructor
                    sep::common::CandleData candle_data{
                        oanda_candle.open, oanda_candle.high, 
                        oanda_candle.low, oanda_candle.close, 
                        static_cast<double>(oanda_candle.volume), timestamp};
                    
                    // Add to deque, maintain max 1440 candles (24h of M1)
                    candle_data_.push_back(candle_data);
                    if (candle_data_.size() > 1440) {
                        candle_data_.pop_front();
                    }
                }
                if (auto_detect_trends_) {
                    detectTrendLines();
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "[SignalsTab] OANDA fetch error: " << e.what() << std::endl;
        }
    }
    
    if (signal_generator_ && workbench_engine_) {
        try {
            // Get latest SEP signals from pattern metric engine
            auto* pme = workbench_engine_->getPatternMetricEngine();
            if (pme && !candle_data_.empty()) {
                // Convert latest candle data to raw bytes for SEP processing
                std::vector<uint8_t> candle_bytes;
                for (const auto& candle : candle_data_) {
                    // Convert OHLC data to bytes (4 floats = 16 bytes per candle)
                    float ohlc[4] = {
                        static_cast<float>(candle.open),
                        static_cast<float>(candle.high),
                        static_cast<float>(candle.low),
                        static_cast<float>(candle.close)
                    };
                    
                    const uint8_t* byte_ptr = reinterpret_cast<const uint8_t*>(ohlc);
                    candle_bytes.insert(candle_bytes.end(), byte_ptr, byte_ptr + sizeof(ohlc));
                }
                
                // Ingest the OHLC data into the pattern engine
                pme->ingestData(candle_bytes.data(), candle_bytes.size());
                
                // IMPORTANT: Also feed the same data to the MetricsMonitor for unified processing
                if (metrics_monitor_) {
                    metrics_monitor_->ingestData(candle_bytes.data(), candle_bytes.size());
                }
                
                // Evolve patterns and compute metrics
                pme->evolvePatterns();
                const auto& metrics_results = pme->computeMetrics();
                
                // Determine thresholds either from MetricsMonitor or user settings
                sep::quantum::SignalThresholds thresholds;
                if (metrics_monitor_) {
                    thresholds = metrics_monitor_->calculateSignalThresholds();
                } else {
                    thresholds.buy_min_coherence = buy_min_coherence_;
                    thresholds.buy_min_stability = buy_min_stability_;
                    thresholds.buy_max_entropy = buy_max_entropy_;
                    thresholds.sell_max_stability = sell_max_stability_;
                    thresholds.sell_min_entropy = sell_min_entropy_;
                }
                pme->setSignalThresholds(thresholds);

                // Convert metrics to SEP signal data
                for (size_t i = 0; i < std::min(metrics_results.size(), candle_data_.size()); i++) {
                    const auto& metrics = metrics_results[i];
                    const auto& candle = candle_data_[candle_data_.size() - metrics_results.size() + i];
                    
                    sep::common::SEPSignalData sep_signal;
                    sep_signal.signal_value = (metrics.coherence + metrics.stability - metrics.entropy) / 2.0f;
                    sep_signal.timestamp = candle.timestamp;
                    
                    const float offset = 0.05f;

                    // Determine signal type using threshold struct

                    
                    // Add to signal buffer, maintain max 1440 signals
                    sep_signals_.push_back(sep_signal);
                    if (sep_signals_.size() > 1440) {
                        sep_signals_.pop_front();
                    }
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "[SignalsTab] SEP signal generation error: " << e.what() << std::endl;
        }
    }
    setupChartArea();
    renderMainChart();
    renderMetricsGraphs();
    renderHoverInfo();

    ImGui::Columns(1);
}

void SignalsTabController::shutdown() {
    candle_data_.clear();
    sep_signals_.clear();
    indicators_.clear();
    trend_lines_.clear();
}

void SignalsTabController::setOandaConnector(sep::connectors::OandaConnector* connector) {
    oanda_connector_ = connector;
}

void SignalsTabController::setQuantumSignalGenerator(QuantumSignalGenerator* generator) {
    signal_generator_ = generator;
}

void SignalsTabController::setMetricsMonitor(std::shared_ptr<MetricsMonitor> monitor) {
    metrics_monitor_ = std::move(monitor);
    coherence_history_1h_.clear();
    stability_history_1h_.clear();
    entropy_history_1h_.clear();
    coherence_history_4h_.clear();
    stability_history_4h_.clear();
    entropy_history_4h_.clear();
    if (metrics_monitor_) {
        metrics_monitor_->startProcessing();
    }
}

void SignalsTabController::setWorkbenchEngine(WorkbenchEngine* engine) {
    workbench_engine_ = engine;
}

void SignalsTabController::setCandleData(const std::deque<sep::common::CandleData>& data) {
    candle_data_ = data;
    if (auto_detect_trends_) {
        detectTrendLines();
    }
}

void SignalsTabController::setCandleData(const std::vector<sep::common::CandleData>& data) {
    candle_data_.assign(data.begin(), data.end());
    if (auto_detect_trends_) {
        detectTrendLines();
    }
}

void SignalsTabController::setSEPSignals(const std::deque<sep::common::SEPSignalData>& signals) {
    sep_signals_ = signals;
}

void SignalsTabController::renderMainChart() {
    if (candle_data_.empty()) {
        ImGui::Text("No data available.");
        return;
    }

    updatePriceRange();
    if (auto_detect_trends_) {
        detectTrendLines();
    }

    ImPlot::SetNextAxisLimits(ImAxis_Y1, price_min_, price_max_, ImPlotCond_Once);
    ImPlot::SetNextAxisLimits(ImAxis_X1, 0, static_cast<double>(candle_data_.size()), ImPlotCond_Once);
    if (ImPlot::BeginPlot("Price", ImVec2(-1, 300), plot_flags_)) {
        renderTechnicalIndicators();
        renderCandlesticks();
        if (show_sep_overlay_) {
            renderSEPSignalOverlay();
        }
        if (show_trend_lines_) {
            renderTrendLines();
        }
        if (show_grid_) {
            renderChartGrid();
        }
        renderCrosshair();
        handleMouseInput();
        ImPlot::EndPlot();
    }

    if (show_volume_) {
        renderVolumeChart();
    }
}

void SignalsTabController::renderCandlesticks() {
    if (candle_data_.empty()) return;

    static std::vector<double> xs;
    static std::vector<double> open;
    static std::vector<double> close;
    static std::vector<double> low;
    static std::vector<double> high;

    static constexpr size_t MAX_VISIBLE = 240;
    size_t start_idx = candle_data_.size() > MAX_VISIBLE ? candle_data_.size() - MAX_VISIBLE : 0;
    const size_t count = candle_data_.size() - start_idx;

    xs.resize(count);
    open.resize(count);
    close.resize(count);
    low.resize(count);
    high.resize(count);

    for (size_t i = 0; i < count; ++i) {
        const auto& c = candle_data_[start_idx + i];
        xs[i] = static_cast<double>(i);
        open[i] = c.open;
        close[i] = c.close;
        low[i] = c.low;
        high[i] = c.high;
    }

    ImPlot::PushPlotClipRect();
    ImPlot::PlotCandles("Price", xs.data(), open.data(), close.data(), low.data(), high.data(), static_cast<int>(count));
    ImPlot::PopPlotClipRect();
}

void SignalsTabController::renderTechnicalIndicators() {
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    
    static constexpr size_t MAX_CANDLES = 1440;
    static constexpr size_t MAX_VISIBLE = 240;
    static constexpr size_t INDICATOR_PERIODS[] = {9, 21, 50, 200};

    // Render EMAs
    for (const auto& period : INDICATOR_PERIODS) {
        std::string ema_key = "EMA_" + std::to_string(period);
        if (indicators_.find(ema_key) == indicators_.end()) continue;
        const auto& indicator = indicators_.at(ema_key);
        
        if (!indicator.enabled || indicator.values.empty()) continue;
        
        size_t start_idx = indicator.values.size() > MAX_VISIBLE ?
                          indicator.values.size() - MAX_VISIBLE : 0;
        
        if (indicator.values.size() <= start_idx + 1) continue;
        
        float candle_width = chart_size_.x / std::min(indicator.values.size() - start_idx, (size_t)MAX_CANDLES);
        
        for (size_t i = start_idx; i < indicator.values.size() - 1; i++) {
            if (indicator.values[i] <= 0 || indicator.values[i+1] <= 0) continue;
            
            float x1 = chart_pos_.x + ((i - start_idx) + 0.5f) * candle_width;
            float x2 = chart_pos_.x + ((i + 1 - start_idx) + 0.5f) * candle_width;
            
            float y1 = chart_pos_.y + chart_size_.y - 
                       ((indicator.values[i] - price_min_) / (price_max_ - price_min_)) * chart_size_.y;
            float y2 = chart_pos_.y + chart_size_.y - 
                       ((indicator.values[i+1] - price_min_) / (price_max_ - price_min_)) * chart_size_.y;
            
            draw_list->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), 
                              indicator.color, indicator.line_thickness);
        }
    }
    
    // Render Bollinger Bands as filled area
    if (indicators_.find("BB_Upper") == indicators_.end() || indicators_.find("BB_Lower") == indicators_.end()) return;
    const auto& bb_upper = indicators_.at("BB_Upper");
    const auto& bb_lower = indicators_.at("BB_Lower");
    
    if (bb_upper.enabled && bb_lower.enabled && 
        bb_upper.values.size() == bb_lower.values.size() && 
        !bb_upper.values.empty()) {
        
        size_t start_idx = bb_upper.values.size() > MAX_VISIBLE ?
                          bb_upper.values.size() - MAX_VISIBLE : 0;
        float candle_width = chart_size_.x / std::min(bb_upper.values.size() - start_idx, (size_t)MAX_VISIBLE);
        
        // Draw band as polygon
        std::vector<ImVec2> points;
        
        // Upper band (left to right)
        for (size_t i = start_idx; i < bb_upper.values.size(); i++) {
            if (bb_upper.values[i] <= 0) continue;
            float x = chart_pos_.x + ((i - start_idx) + 0.5f) * candle_width;
            float y = chart_pos_.y + chart_size_.y - 
                     ((bb_upper.values[i] - price_min_) / (price_max_ - price_min_)) * chart_size_.y;
            points.push_back(ImVec2(x, y));
        }
        
        // Lower band (right to left)
        for (int i = bb_lower.values.size() - 1; i >= (int)start_idx; i--) {
            if (bb_lower.values[i] <= 0) continue;
            float x = chart_pos_.x + ((i - start_idx) + 0.5f) * candle_width;
            float y = chart_pos_.y + chart_size_.y - 
                     ((bb_lower.values[i] - price_min_) / (price_max_ - price_min_)) * chart_size_.y;
            points.push_back(ImVec2(x, y));
        }
        
        if (points.size() >= 3) {
            draw_list->AddConvexPolyFilled(points.data(), points.size(), 
                                          IM_COL32(128, 255, 128, 32));
        }
    }
}

void SignalsTabController::renderSEPSignalOverlay() {
    if (!show_sep_overlay_ || sep_signals_.empty()) return;
    
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    
    static constexpr size_t MAX_VISIBLE = 240;
    size_t start_idx = sep_signals_.size() > MAX_VISIBLE ?
                       sep_signals_.size() - MAX_VISIBLE : 0;
    float signal_width = chart_size_.x / std::min(sep_signals_.size() - start_idx, (size_t)MAX_VISIBLE);
    
    // Draw SEP signal strength as overlay
    for (size_t i = start_idx; i < sep_signals_.size(); i++) {
        const auto& signal = sep_signals_[i];
        
        float x = chart_pos_.x + ((i - start_idx) + 0.5f) * signal_width;
        
        // Signal strength indicator (top of chart)
        float signal_intensity = std::abs(signal.alpha_signal);
        ImU32 signal_color = getSignalColor(signal.signal_type);
        
        float indicator_height = 20.0f * signal_intensity;
        ImVec2 signal_pos = ImVec2(x - 2, chart_pos_.y - indicator_height);
        ImVec2 signal_size = ImVec2(4, indicator_height);
        
        draw_list->AddRectFilled(signal_pos, 
                                ImVec2(signal_pos.x + signal_size.x, signal_pos.y + signal_size.y), 
                                signal_color);
        
        // Draw trend line
        if (i > start_idx && i < sep_signals_.size()) {
            const auto& prev_signal = sep_signals_[i-1];
            float prev_x = chart_pos_.x + ((i - 1 - start_idx) + 0.5f) * signal_width;
            
            // Map trend strength to chart height
            float trend_y = chart_pos_.y + chart_size_.y * 0.1f + 
                           (chart_size_.y * 0.8f * (1.0f - (signal.trend_strength + 1.0f) / 2.0f));
            float prev_trend_y = chart_pos_.y + chart_size_.y * 0.1f + 
                                (chart_size_.y * 0.8f * (1.0f - (prev_signal.trend_strength + 1.0f) / 2.0f));
            
            draw_list->AddLine(ImVec2(prev_x, prev_trend_y), ImVec2(x, trend_y), 
                              IM_COL32(255, 255, 255, 180), 2.0f);
        }
    }
}

void SignalsTabController::renderVolumeChart() {
    if (candle_data_.empty()) return;
    
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    
    // Volume chart area (bottom 20% of chart)
    float volume_height = chart_size_.y * 0.2f;
    ImVec2 volume_pos = ImVec2(chart_pos_.x, chart_pos_.y + chart_size_.y - volume_height);
    
    // Volume background
    draw_list->AddRectFilled(volume_pos, 
                            ImVec2(volume_pos.x + chart_size_.x, volume_pos.y + volume_height), 
                            IM_COL32(10, 10, 15, 200));
    
    static constexpr size_t MAX_VISIBLE = 240;
    size_t start_idx = candle_data_.size() > MAX_VISIBLE ?
                       candle_data_.size() - MAX_VISIBLE : 0;
    float candle_width = chart_size_.x / std::min(candle_data_.size() - start_idx, (size_t)MAX_VISIBLE);
    
    // Find max volume for scaling
    float max_vol = 1.0f;
    for (size_t i = start_idx; i < candle_data_.size(); i++) {
        max_vol = std::max(max_vol, (float)candle_data_[i].volume);
    }
    
    // Draw volume bars
    for (size_t i = start_idx; i < candle_data_.size(); i++) {
        const auto& candle = candle_data_[i];
        
        float x = chart_pos_.x + ((i - start_idx) + 0.5f) * candle_width;
        float vol_height = (candle.volume / max_vol) * volume_height * 0.9f;
        
        bool is_bullish = candle.close > candle.open;
        ImU32 vol_color = is_bullish ? IM_COL32(46, 204, 113, 128) : IM_COL32(231, 76, 60, 128);
        
        ImVec2 bar_min = ImVec2(x - candle_width * 0.4f, volume_pos.y + volume_height - vol_height);
        ImVec2 bar_max = ImVec2(x + candle_width * 0.4f, volume_pos.y + volume_height);
        
        draw_list->AddRectFilled(bar_min, bar_max, vol_color);
    }
}

void SignalsTabController::renderMetricsGraphs() {
    if (!metrics_monitor_) return;

    const auto& roll = metrics_monitor_->getRollingMetrics();
    coherence_history_1h_.push_back(roll.coherence_1h_avg);
    stability_history_1h_.push_back(roll.stability_1h_avg);
    entropy_history_1h_.push_back(roll.entropy_1h_avg);
    coherence_history_4h_.push_back(roll.coherence_4h_avg);
    stability_history_4h_.push_back(roll.stability_4h_avg);
    entropy_history_4h_.push_back(roll.entropy_4h_avg);

    const size_t MAX_POINTS = 240; // roughly 4 hours if updated each minute
    if (coherence_history_1h_.size() > MAX_POINTS) coherence_history_1h_.pop_front();
    if (stability_history_1h_.size() > MAX_POINTS) stability_history_1h_.pop_front();
    if (entropy_history_1h_.size() > MAX_POINTS) entropy_history_1h_.pop_front();
    if (coherence_history_4h_.size() > MAX_POINTS) coherence_history_4h_.pop_front();
    if (stability_history_4h_.size() > MAX_POINTS) stability_history_4h_.pop_front();
    if (entropy_history_4h_.size() > MAX_POINTS) entropy_history_4h_.pop_front();

    const auto toVec = [](const std::deque<float>& d) {
        return std::vector<float>(d.begin(), d.end());
    };

    auto xs = std::vector<float>(coherence_history_1h_.size());
    for (size_t i = 0; i < xs.size(); ++i) xs[i] = static_cast<float>(i);
    auto coh1 = toVec(coherence_history_1h_);
    auto coh4 = toVec(coherence_history_4h_);
    auto stab1 = toVec(stability_history_1h_);
    auto stab4 = toVec(stability_history_4h_);
    auto ent1 = toVec(entropy_history_1h_);
    auto ent4 = toVec(entropy_history_4h_);

    if (ImPlot::BeginPlot("Rolling Metrics", ImVec2(-1,150), ImPlotFlags_NoLegend | ImPlotFlags_NoMenus)) {
        ImPlot::SetNextLineStyle(ImVec4(0.0f, 0.9f, 0.0f, 1.0f));
        ImPlot::PlotLine("Coherence 1h", xs.data(), coh1.data(), static_cast<int>(coh1.size()));
        ImPlot::SetNextLineStyle(ImVec4(0.0f, 0.7f, 0.0f, 1.0f));
        ImPlot::PlotLine("Coherence 4h", xs.data(), coh4.data(), static_cast<int>(coh4.size()));
        ImPlot::SetNextLineStyle(ImVec4(0.9f, 0.9f, 0.0f, 1.0f));
        ImPlot::PlotLine("Stability 1h", xs.data(), stab1.data(), static_cast<int>(stab1.size()));
        ImPlot::SetNextLineStyle(ImVec4(0.7f, 0.7f, 0.0f, 1.0f));
        ImPlot::PlotLine("Stability 4h", xs.data(), stab4.data(), static_cast<int>(stab4.size()));
        ImPlot::SetNextLineStyle(ImVec4(0.9f, 0.0f, 0.0f, 1.0f));
        ImPlot::PlotLine("Entropy 1h", xs.data(), ent1.data(), static_cast<int>(ent1.size()));
        ImPlot::SetNextLineStyle(ImVec4(0.7f, 0.0f, 0.0f, 1.0f));
        ImPlot::PlotLine("Entropy 4h", xs.data(), ent4.data(), static_cast<int>(ent4.size()));
        ImPlot::EndPlot();
    }
}

void SignalsTabController::renderTrendLines() {
    if (trend_lines_.empty()) return;
    
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    
    for (const auto& trend_line : trend_lines_) {
        ImVec2 start_screen = priceToScreen(trend_line.start_price, trend_line.start_time);
        ImVec2 end_screen = priceToScreen(trend_line.end_price, trend_line.end_time);
        
        if (start_screen.x >= chart_pos_.x && end_screen.x <= chart_pos_.x + chart_size_.x) {
            draw_list->AddLine(start_screen, end_screen, trend_line.color, trend_line.thickness);
            
            ImVec2 mid_point = ImVec2((start_screen.x + end_screen.x) * 0.5f, 
                                     (start_screen.y + end_screen.y) * 0.5f);
            const char* label = trend_line.is_support ? "S" : "R";
            draw_list->AddText(mid_point, trend_line.color, label);
        }
    }
}

void SignalsTabController::renderHoverInfo() {
    if (!hover_info_.active) return;
    
    ImGui::SetNextWindowPos(ImVec2(hover_info_.position.x + 10, hover_info_.position.y + 10));
    ImGui::SetNextWindowSize(ImVec2(350, 0), ImGuiCond_Always);
    
    if (ImGui::Begin("Enhanced Price Info", nullptr, 
                     ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | 
                     ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize)) {
        
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Price: %.5f", hover_info_.price);
        
        ImVec4 regime_color = hover_info_.market_regime == "Trending" ? 
            ImVec4(0.0f, 1.0f, 0.0f, 1.0f) : ImVec4(1.0f, 0.8f, 0.0f, 1.0f);
        ImGui::TextColored(regime_color, "[%s]", hover_info_.market_regime.c_str());
        
        if (hover_info_.nearest_candle) {
            const auto& candle = *hover_info_.nearest_candle;
            ImGui::Separator();
            ImGui::TextColored(ImVec4(0.8f, 0.8f, 1.0f, 1.0f), "OHLC Data:");
            
            ImVec4 ohlc_color = candle.close > candle.open ? 
                ImVec4(0.0f, 1.0f, 0.0f, 1.0f) : ImVec4(1.0f, 0.3f, 0.3f, 1.0f);
                
            ImGui::TextColored(ohlc_color, "O: %.5f  H: %.5f", candle.open, candle.high);
            ImGui::TextColored(ohlc_color, "L: %.5f  C: %.5f", candle.low, candle.close);
            ImGui::Text("Volume: %.0f (%.1fx avg)", candle.volume, hover_info_.volume_profile);
            
            ImGui::Spacing();
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "Momentum & Analysis:");
            ImGui::Text("Price Momentum: %+.6f pips/min", hover_info_.price_momentum * 10000);
            ImGui::Text("SEP Divergence: %.3f", hover_info_.sep_divergence);
            ImGui::Text("Market Correlation: %.2f", hover_info_.market_correlation);
        }
        
        if (hover_info_.nearest_sep_signal) {
            const auto& sep = *hover_info_.nearest_sep_signal;
            ImGui::Separator();
            ImGui::TextColored(ImVec4(1.0f, 0.6f, 1.0f, 1.0f), "SEP Engine:");
            
            ImVec4 coherence_color = sep.coherence > 0.7f ? 
                ImVec4(0.0f, 1.0f, 0.0f, 1.0f) : ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
            ImGui::TextColored(coherence_color, "Coherence: %.3f", sep.coherence);
            
            ImVec4 stability_color = sep.stability > 0.6f ? 
                ImVec4(0.0f, 1.0f, 0.0f, 1.0f) : ImVec4(1.0f, 0.8f, 0.0f, 1.0f);
            ImGui::TextColored(stability_color, "Stability: %.3f", sep.stability);
            
            ImVec4 entropy_color = sep.entropy < 0.3f ? 
                ImVec4(0.0f, 1.0f, 0.0f, 1.0f) : ImVec4(1.0f, 0.3f, 0.3f, 1.0f);
            ImGui::TextColored(entropy_color, "Entropy:   %.3f", sep.entropy);
            
            ImGui::Text("Alpha:     %.3f", sep.alpha_signal);
            ImGui::Text("Trend:     %.3f", sep.trend_strength);
            
            ImGui::Spacing();
            ImGui::TextColored(ImVec4(0.8f, 1.0f, 0.8f, 1.0f), "Multi-Timeframe:");
            for (const auto& [timeframe, coherence] : hover_info_.mtf_coherence) {
                ImVec4 mtf_color = coherence > 0.7f ? 
                    ImVec4(0.0f, 1.0f, 0.0f, 1.0f) : ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
                ImGui::TextColored(mtf_color, "%s: %.3f", timeframe.c_str(), coherence);
                if (timeframe != "1h") ImGui::SameLine();
            }
        }
        
        if (hover_info_.nearest_support > 0 || hover_info_.nearest_resistance > 0) {
            ImGui::Separator();
            ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.4f, 1.0f), "Support/Resistance:");
            if (hover_info_.nearest_support > 0) {
                double support_distance = hover_info_.price - hover_info_.nearest_support;
                ImGui::Text("Support: %.5f (%.1f pips)", 
                    hover_info_.nearest_support, support_distance * 10000);
            }
            if (hover_info_.nearest_resistance > 0) {
                double resistance_distance = hover_info_.nearest_resistance - hover_info_.price;
                ImGui::Text("Resistance: %.5f (%.1f pips)", 
                    hover_info_.nearest_resistance, resistance_distance * 10000);
            }
            ImGui::Text("Level Strength: %.1f%%", hover_info_.s_r_strength * 100);
        }
        
        ImGui::Separator();
        ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), "Traditional Indicators:");
        
        ImVec4 rsi_color = hover_info_.rsi_value > 70 ? ImVec4(1.0f, 0.3f, 0.3f, 1.0f) :
                          (hover_info_.rsi_value < 30 ? ImVec4(0.0f, 1.0f, 0.0f, 1.0f) : 
                           ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
        ImGui::TextColored(rsi_color, "RSI: %.1f", hover_info_.rsi_value);
        
        ImGui::Text("MACD: %+.2f", hover_info_.macd_value);
        ImGui::Text("BB Position: %.1f%%", hover_info_.bb_position * 100);
    }
    ImGui::End();
}

void SignalsTabController::renderCrosshair() {
    if (!show_crosshair_ || !ImPlot::IsPlotHovered()) return;

    ImDrawList* draw_list = ImPlot::GetPlotDrawList();
    ImVec2 plot_pos = ImPlot::GetPlotPos();
    ImVec2 plot_size = ImPlot::GetPlotSize();
    ImVec2 mouse_pos = crosshair_pos_;

    draw_list->AddLine(ImVec2(mouse_pos.x, plot_pos.y),
                       ImVec2(mouse_pos.x, plot_pos.y + plot_size.y),
                       IM_COL32(128, 128, 128, 128), 1.0f);
    draw_list->AddLine(ImVec2(plot_pos.x, mouse_pos.y),
                       ImVec2(plot_pos.x + plot_size.x, mouse_pos.y),
                       IM_COL32(128, 128, 128, 128), 1.0f);

    ImPlotPoint plot = ImPlot::PixelsToPlot(mouse_pos);
    char price_text[32];
    (void)snprintf(price_text, sizeof(price_text), "%.5f", plot.y);

    ImVec2 label_pos = ImVec2(plot_pos.x + plot_size.x + 5, mouse_pos.y - 10);
    draw_list->AddRectFilled(label_pos,
                             ImVec2(label_pos.x + 60, label_pos.y + 20),
                             IM_COL32(40, 40, 40, 200));
    draw_list->AddText(ImVec2(label_pos.x + 5, label_pos.y + 3),
                       IM_COL32(255, 255, 255, 255), price_text);

    size_t idx = static_cast<size_t>(plot.x + 0.5);
    idx = std::clamp(idx, size_t(0), candle_data_.empty() ? 0 : candle_data_.size() - 1);
    auto t = candle_data_.empty() ? std::chrono::system_clock::now() : candle_data_[idx].timestamp;
    std::time_t tt = std::chrono::system_clock::to_time_t(t);
    char time_buf[16];
    std::strftime(time_buf, sizeof(time_buf), "%H:%M", std::localtime(&tt));
    ImVec2 time_pos = ImVec2(mouse_pos.x - 30, plot_pos.y + plot_size.y + 5);
    draw_list->AddRectFilled(ImVec2(time_pos.x - 2, time_pos.y - 2),
                             ImVec2(time_pos.x + 34, time_pos.y + 16),
                             IM_COL32(40,40,40,200));
    draw_list->AddText(time_pos, IM_COL32(255,255,255,255), time_buf);
}

void SignalsTabController::renderChartGrid() {
    ImDrawList* draw_list = ImPlot::GetPlotDrawList();

    const int num_price_lines = 8;
    ImU32 grid_color = IM_COL32(40, 40, 50, 128);
    ImPlotRect limits = ImPlot::GetPlotLimits();

    for (int i = 0; i <= num_price_lines; i++) {
        double price = ImLerp(limits.Y.Min, limits.Y.Max, i / (double)num_price_lines);
        ImVec2 p1 = ImPlot::PlotToPixels(ImPlotPoint(limits.X.Min, price));
        ImVec2 p2 = ImPlot::PlotToPixels(ImPlotPoint(limits.X.Max, price));
        drawDashedHorizontal(draw_list, p1.x, p2.x, p1.y, grid_color);

        char price_text[32];
        (void)snprintf(price_text, sizeof(price_text), "%.5f", price);
        draw_list->AddText(ImVec2(p1.x - 60, p1.y - 8), IM_COL32(150,150,160,255), price_text);
    }

    const int num_time_lines = 6;
    for (int i = 0; i <= num_time_lines; i++) {
        double xval = ImLerp(limits.X.Min, limits.X.Max, i / (double)num_time_lines);
        ImVec2 p1 = ImPlot::PlotToPixels(ImPlotPoint(xval, limits.Y.Min));
        ImVec2 p2 = ImPlot::PlotToPixels(ImPlotPoint(xval, limits.Y.Max));
        draw_list->AddLine(p1, p2, grid_color);

        size_t idx = static_cast<size_t>(xval + 0.5);
        idx = std::clamp(idx, size_t(0), candle_data_.empty() ? 0 : candle_data_.size() - 1);
        auto t = candle_data_.empty() ? std::chrono::system_clock::now() : candle_data_[idx].timestamp;
        char time_text[16];
        std::time_t tt = std::chrono::system_clock::to_time_t(t);
        std::strftime(time_text, sizeof(time_text), "%H:%M", std::localtime(&tt));
        draw_list->AddText(ImVec2(p1.x - 20, ImPlot::GetPlotPos().y + ImPlot::GetPlotSize().y + 5),
                           IM_COL32(150,150,160,255), time_text);
    }
}

ImVec2 SignalsTabController::priceToScreen(double price, std::chrono::system_clock::time_point time) {
    if (candle_data_.empty() || price_max_ <= price_min_) {
        return ImVec2(chart_pos_.x, chart_pos_.y);
    }
    
    float y = chart_pos_.y + chart_size_.y - 
              ((price - price_min_) / (price_max_ - price_min_)) * chart_size_.y;
    
    float x = chart_pos_.x;
    if (!candle_data_.empty()) {
        auto time_range = candle_data_.back().timestamp - candle_data_.front().timestamp;
        auto time_offset = time - candle_data_.front().timestamp;
        
        if (time_range.count() > 0) {
            x = chart_pos_.x + (static_cast<float>(time_offset.count()) / 
                               static_cast<float>(time_range.count())) * chart_size_.x;
        }
    }
    
    return ImVec2(x, y);
}

double SignalsTabController::screenToPrice(float y) {
    if (price_max_ <= price_min_) return 0.0;
    
    float normalized_y = (chart_pos_.y + chart_size_.y - y) / chart_size_.y;
    return price_min_ + (price_max_ - price_min_) * normalized_y;
}

std::chrono::system_clock::time_point SignalsTabController::screenToTime(float x) {
    if (candle_data_.empty()) {
        return std::chrono::system_clock::now();
    }
    
    float normalized_x = (x - chart_pos_.x) / chart_size_.x;
    auto time_range = candle_data_.back().timestamp - candle_data_.front().timestamp;
    auto time_offset = std::chrono::duration_cast<std::chrono::system_clock::duration>(
        time_range * normalized_x);
    
    return candle_data_.front().timestamp + time_offset;
}

void SignalsTabController::updatePriceRange() {
    if (candle_data_.empty()) return;

    price_min_ = std::numeric_limits<double>::max();
    price_max_ = std::numeric_limits<double>::lowest();
    
    static constexpr size_t MAX_CANDLES = 1440;
    size_t start_idx = candle_data_.size() > MAX_CANDLES ? 
                       candle_data_.size() - MAX_CANDLES : 0;
    
    for (size_t i = start_idx; i < candle_data_.size(); i++) {
        const auto& candle = candle_data_[i];
        price_min_ = std::min(price_min_, candle.low);
        price_max_ = std::max(price_max_, candle.high);
    }
    
    double range = price_max_ - price_min_;
    double padding = range * 0.1;
    price_min_ -= padding;
    price_max_ += padding;
}

void SignalsTabController::setupChartArea() {
    chart_pos_ = ImGui::GetCursorScreenPos();
    chart_size_ = ImGui::GetContentRegionAvail();
    chart_size_.y -= 30; 
}

void SignalsTabController::handleMouseInput() {
    if (ImGui::IsKeyPressed(ImGuiKey_Space)) {
        show_crosshair_ = !show_crosshair_;
    }

    if (ImPlot::IsPlotHovered()) {
        hover_info_.active = true;
        crosshair_pos_ = ImGui::GetMousePos();
        hover_info_.position = crosshair_pos_;
        updateHoverInfo();
    } else {
        hover_info_.active = false;
    }
}

void SignalsTabController::updateHoverInfo() {
    if (!hover_info_.active || candle_data_.empty()) return;

    ImPlotPoint plot_pos = ImPlot::PixelsToPlot(hover_info_.position);

    hover_info_.price = plot_pos.y;

    size_t idx = static_cast<size_t>(plot_pos.x + 0.5);
    idx = std::clamp(idx, size_t(0), candle_data_.size() - 1);
    hover_info_.time = candle_data_[idx].timestamp;
    hover_info_.nearest_candle = &candle_data_[idx];

    auto nearest_sep = sep_signals_.end();
    auto min_diff = std::chrono::system_clock::duration::max();
    for (auto it = sep_signals_.begin(); it != sep_signals_.end(); ++it) {
        auto diff = it->timestamp > hover_info_.time ? it->timestamp - hover_info_.time : hover_info_.time - it->timestamp;
        if (diff < min_diff) {
            min_diff = diff;
            nearest_sep = it;
        }
    }
    hover_info_.nearest_sep_signal = nearest_sep != sep_signals_.end() ? &*nearest_sep : nullptr;

    calculateEnhancedHoverMetrics();
}

void SignalsTabController::calculateEnhancedHoverMetrics() {
    if (!hover_info_.nearest_candle || !hover_info_.nearest_sep_signal) {
        return;
    }
    
    if (candle_data_.size() >= 2) {
        auto current_it = std::find_if(candle_data_.begin(), candle_data_.end(),
            [this](const sep::common::CandleData& c) { return &c == hover_info_.nearest_candle; });
        
        if (current_it != candle_data_.begin()) {
            auto prev_it = std::prev(current_it);
            double price_change = hover_info_.nearest_candle->close - prev_it->close;
            auto time_diff = std::chrono::duration_cast<std::chrono::minutes>(
                hover_info_.nearest_candle->timestamp - prev_it->timestamp).count();
            hover_info_.price_momentum = time_diff > 0 ? 
                static_cast<float>(price_change / time_diff) : 0.0f;
        }
    }
    
    if (!candle_data_.empty()) {
        double avg_volume = 0.0;
        int count = 0;
        for (const auto& candle : candle_data_) {
            avg_volume += candle.volume;
            count++;
        }
        avg_volume /= count;
        hover_info_.volume_profile = avg_volume > 0 ? 
            static_cast<float>(hover_info_.nearest_candle->volume / avg_volume) : 1.0f;
    }
    
    if (sep_signals_.size() >= 2) {
        auto sep_it = std::find_if(sep_signals_.begin(), sep_signals_.end(),
            [this](const sep::common::SEPSignalData& s) { return &s == hover_info_.nearest_sep_signal; });
        
        if (sep_it != sep_signals_.begin()) {
            auto prev_sep_it = std::prev(sep_it);
            float sep_change = hover_info_.nearest_sep_signal->coherence - prev_sep_it->coherence;
            double price_change_normalized = (hover_info_.nearest_candle->close - 
                hover_info_.nearest_candle->open) / hover_info_.nearest_candle->open;
            hover_info_.sep_divergence = std::abs(sep_change - static_cast<float>(price_change_normalized));
        }
    }
    
    hover_info_.mtf_coherence["1m"] = hover_info_.nearest_sep_signal->coherence;
    hover_info_.mtf_coherence["5m"] = hover_info_.nearest_sep_signal->coherence;
    hover_info_.mtf_coherence["15m"] = hover_info_.nearest_sep_signal->coherence;
    hover_info_.mtf_coherence["1h"] = hover_info_.nearest_sep_signal->coherence;
    
    float coherence = hover_info_.nearest_sep_signal->coherence;
    float stability = hover_info_.nearest_sep_signal->stability;
    
    if (coherence > 0.7f && stability > 0.6f) {
        hover_info_.market_regime = "Trending";
    } else if (coherence > 0.4f && stability > 0.3f) {
        hover_info_.market_regime = "Mixed";
    } else {
        hover_info_.market_regime = "Ranging";
    }
}

void SignalsTabController::detectTrendLines() {
    if (candle_data_.size() < 20) return;
    
    trend_lines_.clear();
    
    std::vector<size_t> swing_highs, swing_lows;
    
    for (size_t i = 2; i < candle_data_.size() - 2; i++) {
        bool is_swing_high = candle_data_[i].high > candle_data_[i-1].high && 
                            candle_data_[i].high > candle_data_[i-2].high &&
                            candle_data_[i].high > candle_data_[i+1].high && 
                            candle_data_[i].high > candle_data_[i+2].high;
        
        bool is_swing_low = candle_data_[i].low < candle_data_[i-1].low && 
                           candle_data_[i].low < candle_data_[i-2].low &&
                           candle_data_[i].low < candle_data_[i+1].low && 
                           candle_data_[i].low < candle_data_[i+2].low;
        
        if (is_swing_high) swing_highs.push_back(i);
        if (is_swing_low) swing_lows.push_back(i);
    }
    
    for (size_t i = 0; i < swing_highs.size() - 1; i++) {
        for (size_t j = i + 1; j < swing_highs.size(); j++) {
            size_t idx1 = swing_highs[i];
            size_t idx2 = swing_highs[j];
            
            TrendLine resistance;
            resistance.start_price = candle_data_[idx1].high;
            resistance.end_price = candle_data_[idx2].high;
            resistance.start_time = candle_data_[idx1].timestamp;
            resistance.end_time = candle_data_[idx2].timestamp;
            resistance.is_support = false;
            resistance.color = IM_COL32(255, 100, 100, 255);
            
            if (std::abs(resistance.start_price - resistance.end_price) / resistance.start_price < 0.02) {
                trend_lines_.push_back(resistance);
            }
        }
    }
    
    for (size_t i = 0; i < swing_lows.size() - 1; i++) {
        for (size_t j = i + 1; j < swing_lows.size(); j++) {
            size_t idx1 = swing_lows[i];
            size_t idx2 = swing_lows[j];
            
            TrendLine support;
            support.start_price = candle_data_[idx1].low;
            support.end_price = candle_data_[idx2].low;
            support.start_time = candle_data_[idx1].timestamp;
            support.end_time = candle_data_[idx2].timestamp;
            support.is_support = true;
            support.color = IM_COL32(100, 255, 100, 255);
            
            if (std::abs(support.start_price - support.end_price) / support.start_price < 0.02) {
                trend_lines_.push_back(support);
            }
        }
    }
    
    if (trend_lines_.size() > 10) {
        trend_lines_.resize(10);
    }
}

ImU32 SignalsTabController::getSignalColor(sep::common::MultiTimeframeSignal signal_type) {
    switch (signal_type) {
        case sep::common::MultiTimeframeSignal::STRONG_BUY:  return IM_COL32(0, 255, 0, 255);
        case sep::common::MultiTimeframeSignal::BUY:         return IM_COL32(144, 238, 144, 255);
        case sep::common::MultiTimeframeSignal::NEUTRAL:     return IM_COL32(255, 255, 0, 255);
        case sep::common::MultiTimeframeSignal::SELL:        return IM_COL32(255, 165, 0, 255);
        case sep::common::MultiTimeframeSignal::STRONG_SELL: return IM_COL32(255, 0, 0, 255);
        default:                                             return IM_COL32(128, 128, 128, 255);
    }
}

ImU32 SignalsTabController::getCandleColor(const sep::common::CandleData& candle, bool is_body) {
    bool is_bullish = candle.close > candle.open;
    if (is_body) {
        return is_bullish ? IM_COL32(46, 204, 113, 255) : IM_COL32(231, 76, 60, 255);
    } else {
        return is_bullish ? IM_COL32(46, 204, 113, 180) : IM_COL32(231, 76, 60, 180);
    }
}

} // namespace sep::workbench
