#include "trading_hud.h"
#include <algorithm>
#include <numeric>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>

namespace sep::workbench {

TradingHUD::TradingHUD() 
    : price_min_(0), price_max_(0), volume_max_(0)
    , last_update_(std::chrono::steady_clock::now())
    , last_indicator_calc_(std::chrono::steady_clock::now()) {
    
    // Initialize technical indicators
    indicators_["EMA_9"] = TechnicalIndicator("EMA 9", IM_COL32(255, 165, 0, 255));    // Orange
    indicators_["EMA_21"] = TechnicalIndicator("EMA 21", IM_COL32(255, 255, 0, 255));   // Yellow
    indicators_["EMA_50"] = TechnicalIndicator("EMA 50", IM_COL32(0, 255, 255, 255));   // Cyan
    indicators_["EMA_200"] = TechnicalIndicator("EMA 200", IM_COL32(255, 0, 255, 255)); // Magenta
    indicators_["RSI"] = TechnicalIndicator("RSI", IM_COL32(128, 128, 255, 255));       // Light Blue
    indicators_["MACD"] = TechnicalIndicator("MACD", IM_COL32(255, 128, 128, 255));     // Light Red
    indicators_["BB_Upper"] = TechnicalIndicator("BB Upper", IM_COL32(128, 255, 128, 128)); // Transparent Green
    indicators_["BB_Lower"] = TechnicalIndicator("BB Lower", IM_COL32(128, 255, 128, 128)); // Transparent Green
    indicators_["SEP_Trend"] = TechnicalIndicator("SEP Trend", IM_COL32(255, 255, 255, 255)); // White
}

TradingHUD::~TradingHUD() {
    shutdown();
}

bool TradingHUD::initialize() {
    std::cout << "[TradingHUD] Initializing advanced trading HUD..." << std::endl;
    return true;
}

void TradingHUD::render() {
    // Set default window positions on first run
    if (!window_positions_.positions_set) {
        setDefaultWindowPositions();
        window_positions_.positions_set = true;
    }
    
    // Handle mouse input for hover info
    handleMouseInput();
    
    // Main trading HUD window
    ImGui::SetNextWindowPos(window_positions_.trading_hud_pos, ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(window_positions_.trading_hud_size, ImGuiCond_FirstUseEver);
    
    if (!ImGui::Begin("SEP Trading HUD", nullptr, ImGuiWindowFlags_NoScrollbar)) {
        ImGui::End();
        return;
    }
    
    renderTopBar();
    renderTimeFrameSelector();
    
    ImGui::Separator();
    
    // Main trading layout - split horizontally
    ImVec2 content_region = ImGui::GetContentRegionAvail();
    
    // Left side: Main chart (70% width)
    float chart_width = content_region.x * 0.7f;
    float panel_width = content_region.x * 0.3f;
    
    if (ImGui::BeginChild("MainChartArea", ImVec2(chart_width, content_region.y), true)) {
        setupChartArea();
        renderMainChart();
    }
    ImGui::EndChild();
    
    ImGui::SameLine();
    
    // Right side: SEP metrics and trading controls (30% width)
    if (ImGui::BeginChild("TradingPanel", ImVec2(panel_width, content_region.y), true)) {
        renderSEPMetricsPanel();
        ImGui::Separator();
        renderTradingControls();
    }
    ImGui::EndChild();
    
    // Update indicators if needed
    if (shouldUpdateIndicators()) {
        calculateTechnicalIndicators();
        updateSEPSignals();
        if (auto_detect_trends_) {
            detectTrendLines();
        }
    }
    
    ImGui::End();
    
    // Render additional windows
    renderAccountBalance();
    renderOrderBook();
    renderPositionsPanel();
    renderOrdersPanel();
    renderTradeHistory();
    renderPerformanceMetrics();
    renderAlertsPanel();
    renderRiskManager();
    renderHoverInfo();
}

void TradingHUD::renderTopBar() {
    // Instrument selector
    ImGui::Text("Instrument:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(120);
    const char* instruments[] = {"EUR_USD", "GBP_USD", "USD_JPY", "AUD_USD", "USD_CAD", "USD_CHF"};
    static int current_instrument = 0;
    if (ImGui::Combo("##Instrument", &current_instrument, instruments, IM_ARRAYSIZE(instruments))) {
        selected_instrument_ = instruments[current_instrument];
        // TODO: Load new instrument data
    }
    
    ImGui::SameLine();
    ImGui::Text("|"); // Simple vertical separator
    ImGui::SameLine();
    
    // Live mode toggle
    if (ImGui::Checkbox("Live Mode", &live_mode_)) {
        // TODO: Start/stop live data feed
    }
    
    ImGui::SameLine();
    ImGui::Text("|"); // Simple vertical separator
    ImGui::SameLine();
    
    // Display current price and change
    if (!candle_data_.empty()) {
        const auto& last_candle = candle_data_.back();
        double price_change = 0.0;
        double price_change_pct = 0.0;
        
        if (candle_data_.size() > 1) {
            const auto& prev_candle = candle_data_[candle_data_.size() - 2];
            price_change = last_candle.close - prev_candle.close;
            price_change_pct = (price_change / prev_candle.close) * 100.0;
        }
        
        ImVec4 price_color = price_change >= 0 ? ImVec4(0, 1, 0, 1) : ImVec4(1, 0, 0, 1);
        ImGui::Text("Price:");
        ImGui::SameLine();
        ImGui::TextColored(price_color, "%.5f", last_candle.close);
        ImGui::SameLine();
        ImGui::TextColored(price_color, "(%.5f / %.2f%%)", price_change, price_change_pct);
    }
    
    ImGui::SameLine();
    ImGui::Text("|"); // Simple vertical separator
    ImGui::SameLine();
    
    // Chart options
    ImGui::Checkbox("Volume", &show_volume_);
    ImGui::SameLine();
    ImGui::Checkbox("SEP Overlay", &show_sep_overlay_);
    ImGui::SameLine();
    ImGui::Checkbox("Grid", &show_grid_);
}

void TradingHUD::renderMainChart() {
    if (candle_data_.empty()) {
        ImGui::Text("No data available. Connect to OANDA and load historical data.");
        return;
    }
    
    updatePriceRange();
    
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    
    // Chart background
    ImU32 bg_color = IM_COL32(15, 15, 25, 255);
    draw_list->AddRectFilled(chart_pos_, 
                            ImVec2(chart_pos_.x + chart_size_.x, chart_pos_.y + chart_size_.y), 
                            bg_color);
    
    // Chart border
    ImU32 border_color = IM_COL32(80, 80, 100, 255);
    draw_list->AddRect(chart_pos_, 
                      ImVec2(chart_pos_.x + chart_size_.x, chart_pos_.y + chart_size_.y), 
                      border_color);
    
    // Price grid
    if (show_grid_) {
        const int num_price_lines = 8;
        ImU32 grid_color = IM_COL32(40, 40, 50, 128);
        
        for (int i = 0; i <= num_price_lines; i++) {
            float y = chart_pos_.y + (chart_size_.y * i / num_price_lines);
            draw_list->AddLine(ImVec2(chart_pos_.x, y), 
                              ImVec2(chart_pos_.x + chart_size_.x, y), 
                              grid_color);
            
            // Price labels
            double price = price_max_ - ((price_max_ - price_min_) * i / num_price_lines);
            char price_text[32];
            sprintf(price_text, "%.5f", price);
            draw_list->AddText(ImVec2(chart_pos_.x - 70, y - 8), 
                              IM_COL32(150, 150, 160, 255), price_text);
        }
        
        // Time grid (every 4 hours for 24h view)
        const int num_time_lines = 6;
        for (int i = 0; i <= num_time_lines; i++) {
            float x = chart_pos_.x + (chart_size_.x * i / num_time_lines);
            draw_list->AddLine(ImVec2(x, chart_pos_.y), 
                              ImVec2(x, chart_pos_.y + chart_size_.y), 
                              grid_color);
            
            // Time labels (simplified)
            char time_text[16];
            sprintf(time_text, "%02d:00", (24 - (i * 4)) % 24);
            draw_list->AddText(ImVec2(x - 15, chart_pos_.y + chart_size_.y + 5), 
                              IM_COL32(150, 150, 160, 255), time_text);
        }
    }
    
    // Render chart layers
    renderTechnicalIndicators();  // Behind candlesticks
    renderCandlesticks();        // Main price action
    if (show_trend_lines_) {
        renderTrendLines();      // Trend lines over price
    }
    renderSEPSignalOverlay();    // On top of price
    
    if (show_volume_) {
        renderVolumeChart();     // Bottom section
    }
}

void TradingHUD::renderCandlesticks() {
    if (candle_data_.empty() || price_max_ <= price_min_) return;
    
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    
    float candle_width = chart_size_.x / std::min(candle_data_.size(), MAX_CANDLES);
    float body_width = std::max(1.0f, candle_width * 0.8f);
    
    size_t start_idx = candle_data_.size() > MAX_CANDLES ? 
                       candle_data_.size() - MAX_CANDLES : 0;
    
    for (size_t i = start_idx; i < candle_data_.size(); i++) {
        const auto& candle = candle_data_[i];
        
        float x = chart_pos_.x + ((i - start_idx) + 0.5f) * candle_width;
        
        // Calculate Y positions
        float high_y = chart_pos_.y + chart_size_.y - 
                       ((candle.high - price_min_) / (price_max_ - price_min_)) * chart_size_.y;
        float low_y = chart_pos_.y + chart_size_.y - 
                      ((candle.low - price_min_) / (price_max_ - price_min_)) * chart_size_.y;
        float open_y = chart_pos_.y + chart_size_.y - 
                       ((candle.open - price_min_) / (price_max_ - price_min_)) * chart_size_.y;
        float close_y = chart_pos_.y + chart_size_.y - 
                        ((candle.close - price_min_) / (price_max_ - price_min_)) * chart_size_.y;
        
        bool is_bullish = candle.close > candle.open;
        ImU32 wick_color = is_bullish ? IM_COL32(46, 204, 113, 255) : IM_COL32(231, 76, 60, 255);
        ImU32 body_color = getCandleColor(candle, true);
        
        // Draw wick
        draw_list->AddLine(ImVec2(x, high_y), ImVec2(x, low_y), wick_color, 1.0f);
        
        // Draw body
        float body_top = std::min(open_y, close_y);
        float body_bottom = std::max(open_y, close_y);
        
        if (std::abs(body_bottom - body_top) < 1.0f) {
            // Doji - draw horizontal line
            draw_list->AddLine(ImVec2(x - body_width/2, open_y), 
                              ImVec2(x + body_width/2, open_y), body_color, 2.0f);
        } else {
            ImVec2 body_min = ImVec2(x - body_width/2, body_top);
            ImVec2 body_max = ImVec2(x + body_width/2, body_bottom);
            
            if (is_bullish) {
                // Hollow body for bullish
                draw_list->AddRect(body_min, body_max, body_color, 0.0f, 0, 1.5f);
            } else {
                // Filled body for bearish
                draw_list->AddRectFilled(body_min, body_max, body_color);
            }
        }
    }
}

void TradingHUD::renderTechnicalIndicators() {
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    
    // Render EMAs
    for (const auto& period : INDICATOR_PERIODS) {
        std::string ema_key = "EMA_" + std::to_string(period);
        const auto& indicator = indicators_[ema_key];
        
        if (!indicator.enabled || indicator.values.empty()) continue;
        
        size_t start_idx = indicator.values.size() > MAX_CANDLES ? 
                          indicator.values.size() - MAX_CANDLES : 0;
        
        if (indicator.values.size() <= start_idx + 1) continue;
        
        float candle_width = chart_size_.x / std::min(indicator.values.size() - start_idx, MAX_CANDLES);
        
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
    const auto& bb_upper = indicators_["BB_Upper"];
    const auto& bb_lower = indicators_["BB_Lower"];
    
    if (bb_upper.enabled && bb_lower.enabled && 
        bb_upper.values.size() == bb_lower.values.size() && 
        !bb_upper.values.empty()) {
        
        size_t start_idx = bb_upper.values.size() > MAX_CANDLES ? 
                          bb_upper.values.size() - MAX_CANDLES : 0;
        float candle_width = chart_size_.x / std::min(bb_upper.values.size() - start_idx, MAX_CANDLES);
        
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

void TradingHUD::renderSEPSignalOverlay() {
    if (!show_sep_overlay_ || sep_signals_.empty()) return;
    
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    
    size_t start_idx = sep_signals_.size() > MAX_CANDLES ? 
                       sep_signals_.size() - MAX_CANDLES : 0;
    float signal_width = chart_size_.x / std::min(sep_signals_.size() - start_idx, MAX_CANDLES);
    
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

void TradingHUD::renderVolumeChart() {
    if (candle_data_.empty()) return;
    
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    
    // Volume chart area (bottom 20% of chart)
    float volume_height = chart_size_.y * 0.2f;
    ImVec2 volume_pos = ImVec2(chart_pos_.x, chart_pos_.y + chart_size_.y - volume_height);
    
    // Volume background
    draw_list->AddRectFilled(volume_pos, 
                            ImVec2(volume_pos.x + chart_size_.x, volume_pos.y + volume_height), 
                            IM_COL32(10, 10, 15, 200));
    
    size_t start_idx = candle_data_.size() > MAX_CANDLES ? 
                       candle_data_.size() - MAX_CANDLES : 0;
    float candle_width = chart_size_.x / std::min(candle_data_.size() - start_idx, MAX_CANDLES);
    
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

void TradingHUD::renderSEPMetricsPanel() {
    ImGui::Text("SEP Real-Time Metrics");
    ImGui::Separator();
    
    if (sep_signals_.empty()) {
        ImGui::Text("No SEP signals available");
        return;
    }
    
    const auto& latest_signal = sep_signals_.back();
    
    // Current signal display
    ImGui::Text("Current Signal:");
    ImU32 signal_color = getSignalColor(latest_signal.signal_type);
    ImVec4 signal_color_v4 = ImGui::ColorConvertU32ToFloat4(signal_color);
    
    const char* signal_names[] = {"STRONG SELL", "SELL", "NEUTRAL", "BUY", "STRONG BUY"};
    ImGui::TextColored(signal_color_v4, "%s", signal_names[latest_signal.signal_type + 2]);
    
    ImGui::Spacing();
    
    // Metrics breakdown
    ImGui::Text("Coherence: %.3f", latest_signal.coherence);
    ImGui::SameLine();
    ImGui::ProgressBar(latest_signal.coherence, ImVec2(-1, 0));
    
    ImGui::Text("Stability: %.3f", latest_signal.stability);
    ImGui::SameLine();
    ImGui::ProgressBar(latest_signal.stability, ImVec2(-1, 0));
    
    ImGui::Text("Entropy:   %.3f", latest_signal.entropy);
    ImGui::SameLine();
    ImGui::ProgressBar(1.0f - latest_signal.entropy, ImVec2(-1, 0)); // Invert for display
    
    ImGui::Spacing();
    
    // Alpha signal
    ImGui::Text("Alpha Signal: %.3f", latest_signal.alpha_signal);
    float alpha_normalized = (latest_signal.alpha_signal + 1.0f) / 2.0f; // Normalize -1 to 1 -> 0 to 1
    ImVec4 alpha_color = alpha_normalized > 0.5f ? ImVec4(0, 1, 0, 1) : ImVec4(1, 0, 0, 1);
    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, alpha_color);
    ImGui::ProgressBar(alpha_normalized, ImVec2(-1, 0));
    ImGui::PopStyleColor();
    
    // Trend strength
    ImGui::Text("Trend: %.3f", latest_signal.trend_strength);
    float trend_normalized = (latest_signal.trend_strength + 1.0f) / 2.0f;
    ImVec4 trend_color = trend_normalized > 0.5f ? ImVec4(0, 0.8f, 1, 1) : ImVec4(1, 0.5f, 0, 1);
    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, trend_color);
    ImGui::ProgressBar(trend_normalized, ImVec2(-1, 0));
    ImGui::PopStyleColor();
    
    ImGui::Spacing();
    ImGui::Separator();
    
    // Historical performance
    ImGui::Text("Signal History (Last 10):");
    if (ImGui::BeginTable("SignalHistory", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
        ImGui::TableSetupColumn("Time");
        ImGui::TableSetupColumn("Signal");
        ImGui::TableSetupColumn("Alpha");
        ImGui::TableHeadersRow();
        
        size_t start = sep_signals_.size() > 10 ? sep_signals_.size() - 10 : 0;
        for (size_t i = start; i < sep_signals_.size(); i++) {
            const auto& signal = sep_signals_[i];
            
            ImGui::TableNextRow();
            
            // Time (simplified)
            ImGui::TableSetColumnIndex(0);
            auto time_t = std::chrono::system_clock::to_time_t(signal.timestamp);
            auto tm = *std::localtime(&time_t);
            ImGui::Text("%02d:%02d", tm.tm_hour, tm.tm_min);
            
            // Signal type
            ImGui::TableSetColumnIndex(1);
            ImVec4 color = ImGui::ColorConvertU32ToFloat4(getSignalColor(signal.signal_type));
            ImGui::TextColored(color, "%s", signal_names[signal.signal_type + 2]);
            
            // Alpha value
            ImGui::TableSetColumnIndex(2);
            ImGui::Text("%.3f", signal.alpha_signal);
        }
        
        ImGui::EndTable();
    }
}

void TradingHUD::renderTradingControls() {
    ImGui::Text("Trading Controls");
    ImGui::Separator();
    
    // Position size
    static float position_size = 1000.0f;
    ImGui::InputFloat("Position Size", &position_size, 100.0f, 1000.0f, "%.0f");
    
    // Stop loss and take profit
    static float stop_loss_pips = 20.0f;
    static float take_profit_pips = 40.0f;
    
    ImGui::InputFloat("Stop Loss (pips)", &stop_loss_pips, 1.0f, 10.0f, "%.1f");
    ImGui::InputFloat("Take Profit (pips)", &take_profit_pips, 1.0f, 10.0f, "%.1f");
    
    ImGui::Spacing();
    
    // Manual trading buttons
    if (ImGui::Button("BUY", ImVec2(-1, 30))) {
        // TODO: Execute buy order
        std::cout << "[TradingHUD] Manual BUY order requested" << std::endl;
    }
    
    if (ImGui::Button("SELL", ImVec2(-1, 30))) {
        // TODO: Execute sell order
        std::cout << "[TradingHUD] Manual SELL order requested" << std::endl;
    }
    
    ImGui::Spacing();
    ImGui::Separator();
    
    // Auto-trading settings
    static bool auto_trading_enabled = false;
    ImGui::Checkbox("Enable Auto Trading", &auto_trading_enabled);
    
    if (auto_trading_enabled) {
        static float min_signal_strength = 0.7f;
        ImGui::SliderFloat("Min Signal Strength", &min_signal_strength, 0.1f, 1.0f, "%.2f");
        
        // Show current signal strength vs threshold
        if (!sep_signals_.empty()) {
            float current_strength = std::abs(sep_signals_.back().alpha_signal);
            ImVec4 strength_color = current_strength >= min_signal_strength ? 
                                   ImVec4(0, 1, 0, 1) : ImVec4(1, 0, 0, 1);
            ImGui::TextColored(strength_color, "Current: %.3f", current_strength);
        }
    }
    
    ImGui::Spacing();
    ImGui::Separator();
    
    // Performance metrics
    ImGui::Text("Session Performance");
    ImGui::Text("P&L: +$0.00");  // TODO: Calculate from actual trades
    ImGui::Text("Win Rate: 0.0%%");
    ImGui::Text("Trades: 0");
}

// Technical indicator calculations
void TradingHUD::calculateTechnicalIndicators() {
    calculateMovingAverages();
    calculateRSI();
    calculateMACD();
    calculateBollingerBands();
    calculateSEPTrendLines();
    
    last_indicator_calc_ = std::chrono::steady_clock::now();
}

void TradingHUD::calculateMovingAverages() {
    if (candle_data_.size() < 2) return;
    
    for (const auto& period : INDICATOR_PERIODS) {
        std::string ema_key = "EMA_" + std::to_string(period);
        auto& indicator = indicators_[ema_key];
        
        indicator.values.clear();
        indicator.values.reserve(candle_data_.size());
        
        if (candle_data_.size() < period) continue;
        
        // Calculate EMA
        double multiplier = 2.0 / (period + 1.0);
        double ema = candle_data_[period - 1].close; // Start with SMA
        
        for (size_t i = 0; i < period - 1; i++) {
            indicator.values.push_back(0); // Placeholder
        }
        indicator.values.push_back(ema);
        
        for (size_t i = period; i < candle_data_.size(); i++) {
            ema = (candle_data_[i].close * multiplier) + (ema * (1 - multiplier));
            indicator.values.push_back(ema);
        }
    }
}

void TradingHUD::calculateRSI() {
    const int period = 14;
    if (candle_data_.size() < period + 1) return;
    
    auto& rsi_indicator = indicators_["RSI"];
    rsi_indicator.values.clear();
    rsi_indicator.values.reserve(candle_data_.size());
    
    // Calculate price changes
    std::vector<double> gains, losses;
    for (size_t i = 1; i < candle_data_.size(); i++) {
        double change = candle_data_[i].close - candle_data_[i-1].close;
        gains.push_back(change > 0 ? change : 0);
        losses.push_back(change < 0 ? -change : 0);
    }
    
    // Initial average gain and loss
    double avg_gain = 0, avg_loss = 0;
    for (int i = 0; i < period; i++) {
        avg_gain += gains[i];
        avg_loss += losses[i];
    }
    avg_gain /= period;
    avg_loss /= period;
    
    for (size_t i = 0; i < period; i++) {
        rsi_indicator.values.push_back(0); // Placeholder
    }
    
    // Calculate RSI
    for (size_t i = period; i < gains.size(); i++) {
        avg_gain = ((avg_gain * (period - 1)) + gains[i]) / period;
        avg_loss = ((avg_loss * (period - 1)) + losses[i]) / period;
        
        double rs = avg_gain / (avg_loss > 0 ? avg_loss : 0.0001);
        double rsi = 100.0 - (100.0 / (1.0 + rs));
        
        rsi_indicator.values.push_back(rsi / 100.0); // Normalize to 0-1
    }
}

void TradingHUD::calculateMACD() {
    if (candle_data_.size() < 26) return;
    
    // Calculate 12 and 26 period EMAs
    std::vector<double> ema12, ema26;
    
    // EMA 12
    double multiplier12 = 2.0 / 13.0;
    double ema_val12 = candle_data_[11].close;
    for (size_t i = 12; i < candle_data_.size(); i++) {
        ema_val12 = (candle_data_[i].close * multiplier12) + (ema_val12 * (1 - multiplier12));
        ema12.push_back(ema_val12);
    }
    
    // EMA 26
    double multiplier26 = 2.0 / 27.0;
    double ema_val26 = candle_data_[25].close;
    for (size_t i = 26; i < candle_data_.size(); i++) {
        ema_val26 = (candle_data_[i].close * multiplier26) + (ema_val26 * (1 - multiplier26));
        ema26.push_back(ema_val26);
    }
    
    // Calculate MACD line
    auto& macd_indicator = indicators_["MACD"];
    macd_indicator.values.clear();
    macd_indicator.values.reserve(candle_data_.size());
    
    for (size_t i = 0; i < 26; i++) {
        macd_indicator.values.push_back(0); // Placeholder
    }
    
    for (size_t i = 0; i < ema26.size(); i++) {
        double macd = ema12[i + 14] - ema26[i]; // Align indices
        macd_indicator.values.push_back(macd);
    }
}

void TradingHUD::calculateBollingerBands() {
    const int period = 20;
    const double std_dev_multiplier = 2.0;
    
    if (candle_data_.size() < period) return;
    
    auto& bb_upper = indicators_["BB_Upper"];
    auto& bb_lower = indicators_["BB_Lower"];
    
    bb_upper.values.clear();
    bb_lower.values.clear();
    bb_upper.values.reserve(candle_data_.size());
    bb_lower.values.reserve(candle_data_.size());
    
    for (size_t i = 0; i < period - 1; i++) {
        bb_upper.values.push_back(0);
        bb_lower.values.push_back(0);
    }
    
    for (size_t i = period - 1; i < candle_data_.size(); i++) {
        // Calculate SMA
        double sum = 0;
        for (size_t j = i - period + 1; j <= i; j++) {
            sum += candle_data_[j].close;
        }
        double sma = sum / period;
        
        // Calculate standard deviation
        double variance = 0;
        for (size_t j = i - period + 1; j <= i; j++) {
            double diff = candle_data_[j].close - sma;
            variance += diff * diff;
        }
        double std_dev = std::sqrt(variance / period);
        
        bb_upper.values.push_back(sma + (std_dev_multiplier * std_dev));
        bb_lower.values.push_back(sma - (std_dev_multiplier * std_dev));
    }
}

void TradingHUD::calculateSEPTrendLines() {
    if (sep_signals_.size() < 10) return;
    
    auto& trend_indicator = indicators_["SEP_Trend"];
    trend_indicator.values.clear();
    trend_indicator.values.reserve(sep_signals_.size());
    
    // Simple trend line based on SEP signal strength
    for (const auto& signal : sep_signals_) {
        trend_indicator.values.push_back(signal.trend_strength);
    }
}

// Utility functions
void TradingHUD::setupChartArea() {
    chart_pos_ = ImGui::GetCursorScreenPos();
    chart_size_ = ImGui::GetContentRegionAvail();
    chart_size_.y -= 30; // Leave space for bottom margin
}

void TradingHUD::updatePriceRange() {
    if (candle_data_.empty()) return;
    
    price_min_ = std::numeric_limits<double>::max();
    price_max_ = std::numeric_limits<double>::lowest();
    
    size_t start_idx = candle_data_.size() > MAX_CANDLES ? 
                       candle_data_.size() - MAX_CANDLES : 0;
    
    for (size_t i = start_idx; i < candle_data_.size(); i++) {
        const auto& candle = candle_data_[i];
        price_min_ = std::min(price_min_, candle.low);
        price_max_ = std::max(price_max_, candle.high);
    }
    
    // Add padding
    double range = price_max_ - price_min_;
    double padding = range * 0.1;
    price_min_ -= padding;
    price_max_ += padding;
}

bool TradingHUD::shouldUpdateIndicators() {
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - last_indicator_calc_);
    return elapsed.count() > 5 || data_changed_; // Update every 5 seconds or when data changes
}

ImU32 TradingHUD::getSignalColor(SEPSignalData::SignalType signal_type) {
    switch (signal_type) {
        case SEPSignalData::STRONG_BUY:  return IM_COL32(0, 255, 0, 255);     // Bright Green
        case SEPSignalData::BUY:         return IM_COL32(144, 238, 144, 255);  // Light Green
        case SEPSignalData::NEUTRAL:     return IM_COL32(255, 255, 0, 255);    // Yellow
        case SEPSignalData::SELL:        return IM_COL32(255, 165, 0, 255);    // Orange
        case SEPSignalData::STRONG_SELL: return IM_COL32(255, 0, 0, 255);      // Red
        default:                         return IM_COL32(128, 128, 128, 255);  // Gray
    }
}

ImU32 TradingHUD::getCandleColor(const CandleData& candle, bool is_body) {
    bool is_bullish = candle.close > candle.open;
    if (is_body) {
        return is_bullish ? IM_COL32(46, 204, 113, 255) : IM_COL32(231, 76, 60, 255);
    } else {
        return is_bullish ? IM_COL32(46, 204, 113, 180) : IM_COL32(231, 76, 60, 180);
    }
}

// Data management functions
void TradingHUD::updateCandleData(const std::vector<sep::connectors::OandaCandle>& oanda_candles) {
    candle_data_.clear();
    
    for (const auto& oanda_candle : oanda_candles) {
        // Convert OANDA candle to internal format
        auto timestamp = std::chrono::system_clock::now(); // TODO: Parse actual timestamp
        
        candle_data_.emplace_back(
            oanda_candle.open,
            oanda_candle.high,
            oanda_candle.low,
            oanda_candle.close,
            oanda_candle.volume,
            timestamp
        );
    }
    
    trimDataToTimeWindow();
    data_changed_ = true;
}

void TradingHUD::updateSEPSignals() {
    if (!metrics_monitor_) return;
    
    // Get current metrics from SEP engine
    const auto& system_metrics = metrics_monitor_->getSystemMetrics();
    
    SEPSignalData signal;
    signal.coherence = system_metrics.avg_coherence;
    signal.stability = system_metrics.avg_stability;
    signal.entropy = system_metrics.avg_entropy;
    signal.timestamp = std::chrono::system_clock::now();
    
    // Calculate derived metrics
    signal.alpha_signal = calculateAlphaSignal(signal);
    signal.signal_type = interpretSEPSignal(signal.coherence, signal.stability, signal.entropy);
    signal.trend_strength = calculateTrendStrength(sep_signals_);
    
    sep_signals_.push_back(signal);
    
    // Keep only recent signals
    while (sep_signals_.size() > MAX_CANDLES) {
        sep_signals_.pop_front();
    }
}

SEPSignalData::SignalType TradingHUD::interpretSEPSignal(float coherence, float stability, float entropy) {
    // SEP signal interpretation logic
    float combined_strength = (coherence * 0.4f) + (stability * 0.3f) - (entropy * 0.3f);
    
    if (combined_strength > 0.8f) return SEPSignalData::STRONG_BUY;
    if (combined_strength > 0.4f) return SEPSignalData::BUY;
    if (combined_strength > -0.4f) return SEPSignalData::NEUTRAL;
    if (combined_strength > -0.8f) return SEPSignalData::SELL;
    return SEPSignalData::STRONG_SELL;
}

float TradingHUD::calculateAlphaSignal(const SEPSignalData& signal) {
    // Alpha calculation based on SEP metrics
    return (signal.coherence * 0.5f) + (signal.stability * 0.3f) - (signal.entropy * 0.2f);
}

float TradingHUD::calculateTrendStrength(const std::deque<SEPSignalData>& recent_signals) {
    if (recent_signals.size() < 5) return 0.0f;
    
    // Calculate trend based on recent alpha signals
    float trend = 0.0f;
    for (size_t i = recent_signals.size() - 5; i < recent_signals.size() - 1; i++) {
        trend += recent_signals[i+1].alpha_signal - recent_signals[i].alpha_signal;
    }
    
    return std::clamp(trend / 4.0f, -1.0f, 1.0f);
}

void TradingHUD::trimDataToTimeWindow() {
    // Keep only last 24 hours worth of data
    while (candle_data_.size() > MAX_CANDLES) {
        candle_data_.pop_front();
    }
}

void TradingHUD::setOandaConnector(std::shared_ptr<sep::connectors::OandaConnector> connector) {
    oanda_connector_ = connector;
}

void TradingHUD::setMetricsMonitor(std::shared_ptr<MetricsMonitor> monitor) {
    metrics_monitor_ = monitor;
}

void TradingHUD::shutdown() {
    candle_data_.clear();
    sep_signals_.clear();
    indicators_.clear();
    trend_lines_.clear();
}

void TradingHUD::renderTimeFrameSelector() {
    ImGui::Text("Time Frame:");
    ImGui::SameLine();
    const char* timeframes[] = {"24H", "6H", "3H"};
    int current_tf = static_cast<int>(current_timeframe_);
    if (ImGui::Combo("##TimeFrame", &current_tf, timeframes, 3)) {
        current_timeframe_ = static_cast<TimeFrame>(current_tf);
        // Trigger data reload for new timeframe
        data_changed_ = true;
    }
    
    ImGui::SameLine();
    ImGui::Checkbox("Show Trends", &show_trend_lines_);
    ImGui::SameLine();
    ImGui::Checkbox("Auto Detect", &auto_detect_trends_);
}

void TradingHUD::renderTrendLines() {
    if (trend_lines_.empty()) return;
    
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    
    for (const auto& trend_line : trend_lines_) {
        // Convert price coordinates to screen coordinates
        ImVec2 start_screen = priceToScreen(trend_line.start_price, trend_line.start_time);
        ImVec2 end_screen = priceToScreen(trend_line.end_price, trend_line.end_time);
        
        // Only draw if within chart bounds
        if (start_screen.x >= chart_pos_.x && end_screen.x <= chart_pos_.x + chart_size_.x) {
            draw_list->AddLine(start_screen, end_screen, trend_line.color, trend_line.thickness);
            
            // Add trend line label
            ImVec2 mid_point = ImVec2((start_screen.x + end_screen.x) * 0.5f, 
                                     (start_screen.y + end_screen.y) * 0.5f);
            const char* label = trend_line.is_support ? "S" : "R";
            draw_list->AddText(mid_point, trend_line.color, label);
        }
    }
}

void TradingHUD::renderHoverInfo() {
    if (!hover_info_.active) return;
    
    ImGui::SetNextWindowPos(ImVec2(hover_info_.position.x + 10, hover_info_.position.y + 10));
    ImGui::SetNextWindowSize(ImVec2(250, 0), ImGuiCond_Always);
    
    if (ImGui::Begin("Price Info", nullptr, 
                     ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | 
                     ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize)) {
        
        ImGui::Text("Price: %.5f", hover_info_.price);
        
        if (hover_info_.nearest_candle) {
            const auto& candle = *hover_info_.nearest_candle;
            ImGui::Separator();
            ImGui::Text("OHLC Data:");
            ImGui::Text("Open:  %.5f", candle.open);
            ImGui::Text("High:  %.5f", candle.high);
            ImGui::Text("Low:   %.5f", candle.low);
            ImGui::Text("Close: %.5f", candle.close);
            ImGui::Text("Volume: %d", candle.volume);
        }
        
        if (hover_info_.nearest_sep_signal) {
            const auto& sep = *hover_info_.nearest_sep_signal;
            ImGui::Separator();
            ImGui::Text("SEP Metrics:");
            ImGui::Text("Coherence: %.3f", sep.coherence);
            ImGui::Text("Stability: %.3f", sep.stability);
            ImGui::Text("Entropy:   %.3f", sep.entropy);
            ImGui::Text("Alpha:     %.3f", sep.alpha_signal);
            ImGui::Text("Trend:     %.3f", sep.trend_strength);
        }
    }
    ImGui::End();
}

void TradingHUD::renderAccountBalance() {
    ImGui::SetNextWindowPos(window_positions_.account_balance_pos, ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(window_positions_.account_balance_size, ImGuiCond_FirstUseEver);
    
    if (ImGui::Begin("Account Balance", nullptr)) {
        ImGui::Text("Account Information");
        ImGui::Separator();
        
        ImGui::Text("Balance:    %.2f %s", account_info_.balance, account_info_.currency.c_str());
        ImGui::Text("Equity:     %.2f %s", account_info_.equity, account_info_.currency.c_str());
        ImGui::Text("Margin Used: %.2f %s", account_info_.margin_used, account_info_.currency.c_str());
        ImGui::Text("Available:  %.2f %s", account_info_.margin_available, account_info_.currency.c_str());
        
        ImGui::Separator();
        
        // P&L with color coding
        ImVec4 pl_color = account_info_.unrealized_pl >= 0 ? 
                         ImVec4(0.0f, 1.0f, 0.0f, 1.0f) : ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
        ImGui::TextColored(pl_color, "Unrealized P&L:");
        ImGui::SameLine();
        ImGui::TextColored(pl_color, "%.2f %s", 
                          account_info_.unrealized_pl, account_info_.currency.c_str());
        
        pl_color = account_info_.realized_pl >= 0 ? 
                  ImVec4(0.0f, 1.0f, 0.0f, 1.0f) : ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
        ImGui::TextColored(pl_color, "Realized P&L:");
        ImGui::SameLine();
        ImGui::TextColored(pl_color, "%.2f %s", 
                          account_info_.realized_pl, account_info_.currency.c_str());
    }
    ImGui::End();
}

void TradingHUD::detectTrendLines() {
    if (candle_data_.size() < 20) return; // Need minimum data
    
    trend_lines_.clear();
    
    // Simple trend line detection using swing highs and lows
    std::vector<size_t> swing_highs, swing_lows;
    
    // Find swing points (local extrema)
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
    
    // Create resistance lines from swing highs
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
            resistance.color = IM_COL32(255, 100, 100, 255); // Red for resistance
            
            // Only add if trend line has some validity (similar price levels)
            if (std::abs(resistance.start_price - resistance.end_price) / resistance.start_price < 0.02) {
                trend_lines_.push_back(resistance);
            }
        }
    }
    
    // Create support lines from swing lows
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
            support.color = IM_COL32(100, 255, 100, 255); // Green for support
            
            // Only add if trend line has some validity
            if (std::abs(support.start_price - support.end_price) / support.start_price < 0.02) {
                trend_lines_.push_back(support);
            }
        }
    }
    
    // Limit number of trend lines to avoid clutter
    if (trend_lines_.size() > 10) {
        trend_lines_.resize(10);
    }
}

void TradingHUD::handleMouseInput() {
    ImVec2 mouse_pos = ImGui::GetMousePos();
    
    // Check if mouse is over chart area
    if (mouse_pos.x >= chart_pos_.x && mouse_pos.x <= chart_pos_.x + chart_size_.x &&
        mouse_pos.y >= chart_pos_.y && mouse_pos.y <= chart_pos_.y + chart_size_.y) {
        
        hover_info_.active = true;
        hover_info_.position = mouse_pos;
        updateHoverInfo();
    } else {
        hover_info_.active = false;
    }
}

void TradingHUD::updateHoverInfo() {
    if (!hover_info_.active) return;
    
    // Convert screen position to price and time
    hover_info_.price = screenToPrice(hover_info_.position.y);
    hover_info_.time = screenToTime(hover_info_.position.x);
    
    // Find nearest candle
    hover_info_.nearest_candle = nullptr;
    hover_info_.nearest_sep_signal = nullptr;
    
    auto time_diff_min = std::chrono::duration_cast<std::chrono::minutes>(
        std::chrono::system_clock::duration::max()).count();
    
    // Find closest candle by time
    for (auto& candle : candle_data_) {
        auto diff = std::abs(std::chrono::duration_cast<std::chrono::minutes>(
            hover_info_.time - candle.timestamp).count());
        if (diff < time_diff_min) {
            time_diff_min = diff;
            hover_info_.nearest_candle = &candle;
        }
    }
    
    // Find closest SEP signal
    time_diff_min = std::chrono::duration_cast<std::chrono::minutes>(
        std::chrono::system_clock::duration::max()).count();
    
    for (auto& signal : sep_signals_) {
        auto diff = std::abs(std::chrono::duration_cast<std::chrono::minutes>(
            hover_info_.time - signal.timestamp).count());
        if (diff < time_diff_min) {
            time_diff_min = diff;
            hover_info_.nearest_sep_signal = &signal;
        }
    }
}

void TradingHUD::setDefaultWindowPositions() {
    // Main trading window takes most of the screen
    window_positions_.trading_hud_pos = ImVec2(10, 10);
    window_positions_.trading_hud_size = ImVec2(1200, 800);
    
    // Account balance in top-right
    window_positions_.account_balance_pos = ImVec2(1220, 10);
    window_positions_.account_balance_size = ImVec2(300, 200);
    
    // SEP metrics panel below account balance
    window_positions_.sep_metrics_pos = ImVec2(1220, 220);
    window_positions_.sep_metrics_size = ImVec2(300, 400);
}

ImVec2 TradingHUD::priceToScreen(double price, std::chrono::system_clock::time_point time) {
    if (candle_data_.empty() || price_max_ <= price_min_) {
        return ImVec2(chart_pos_.x, chart_pos_.y);
    }
    
    // Convert price to Y coordinate
    float y = chart_pos_.y + chart_size_.y - 
              ((price - price_min_) / (price_max_ - price_min_)) * chart_size_.y;
    
    // Convert time to X coordinate
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

double TradingHUD::screenToPrice(float y) {
    if (price_max_ <= price_min_) return 0.0;
    
    float normalized_y = (chart_pos_.y + chart_size_.y - y) / chart_size_.y;
    return price_min_ + (price_max_ - price_min_) * normalized_y;
}

std::chrono::system_clock::time_point TradingHUD::screenToTime(float x) {
    if (candle_data_.empty()) {
        return std::chrono::system_clock::now();
    }
    
    float normalized_x = (x - chart_pos_.x) / chart_size_.x;
    auto time_range = candle_data_.back().timestamp - candle_data_.front().timestamp;
    auto time_offset = std::chrono::duration_cast<std::chrono::system_clock::duration>(
        time_range * normalized_x);
    
    return candle_data_.front().timestamp + time_offset;
}

// =============================================================================
// PROFESSIONAL TRADING PLATFORM ENHANCEMENTS
// =============================================================================

void TradingHUD::renderOrderBook() {
    static bool show_order_book = true;
    if (!show_order_book) return;
    
    ImGui::SetNextWindowPos(ImVec2(1540, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 400), ImGuiCond_FirstUseEver);
    
    if (ImGui::Begin("Order Book", &show_order_book)) {
        ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "📊 %s Order Book", selected_instrument_.c_str());
        ImGui::Separator();
        
        // Ask levels (sell orders) - displayed in reverse order
        ImGui::TextColored(ImVec4(1.0f, 0.2f, 0.2f, 1.0f), "ASKS (Sell Orders)");
        ImGui::Columns(3, "AsksColumns", true);
        ImGui::Text("Price"); ImGui::NextColumn();
        ImGui::Text("Size"); ImGui::NextColumn(); 
        ImGui::Text("Orders"); ImGui::NextColumn();
        ImGui::Separator();
        
        for (int i = std::min(5, (int)current_order_book_.asks.size()) - 1; i >= 0; i--) {
            const auto& ask = current_order_book_.asks[i];
            ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "%.5f", ask.price);
            ImGui::NextColumn();
            ImGui::Text("%.2f", ask.size);
            ImGui::NextColumn();
            ImGui::Text("%d", ask.order_count);
            ImGui::NextColumn();
        }
        ImGui::Columns(1);
        
        // Current spread
        if (!current_order_book_.asks.empty() && !current_order_book_.bids.empty()) {
            double spread = current_order_book_.asks[0].price - current_order_book_.bids[0].price;
            ImGui::Separator();
            ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), "Spread: %.5f", spread);
            ImGui::Separator();
        }
        
        // Bid levels (buy orders)
        ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "BIDS (Buy Orders)");
        ImGui::Columns(3, "BidsColumns", true);
        ImGui::Text("Price"); ImGui::NextColumn();
        ImGui::Text("Size"); ImGui::NextColumn();
        ImGui::Text("Orders"); ImGui::NextColumn();
        ImGui::Separator();
        
        for (int i = 0; i < std::min(5, (int)current_order_book_.bids.size()); i++) {
            const auto& bid = current_order_book_.bids[i];
            ImGui::TextColored(ImVec4(0.4f, 1.0f, 0.4f, 1.0f), "%.5f", bid.price);
            ImGui::NextColumn();
            ImGui::Text("%.2f", bid.size);
            ImGui::NextColumn();
            ImGui::Text("%d", bid.order_count);
            ImGui::NextColumn();
        }
        ImGui::Columns(1);
    }
    ImGui::End();
}

void TradingHUD::renderPositionsPanel() {
    static bool show_positions = true;
    if (!show_positions) return;
    
    ImGui::SetNextWindowPos(ImVec2(1540, 420), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_FirstUseEver);
    
    if (ImGui::Begin("Positions", &show_positions)) {
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "💼 Open Positions");
        ImGui::Separator();
        
        if (positions_.empty()) {
            ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "No open positions");
        } else {
            ImGui::Columns(4, "PositionsColumns", true);
            ImGui::Text("Instrument"); ImGui::NextColumn();
            ImGui::Text("Size"); ImGui::NextColumn();
            ImGui::Text("P&L"); ImGui::NextColumn();
            ImGui::Text("Price"); ImGui::NextColumn();
            ImGui::Separator();
            
            for (const auto& position : positions_) {
                ImGui::Text("%s", position.instrument.c_str());
                ImGui::NextColumn();
                
                ImVec4 size_color = position.size > 0 ? ImVec4(0.2f, 1.0f, 0.2f, 1.0f) : ImVec4(1.0f, 0.2f, 0.2f, 1.0f);
                ImGui::TextColored(size_color, "%.2f %s", std::abs(position.size), position.size > 0 ? "LONG" : "SHORT");
                ImGui::NextColumn();
                
                ImVec4 pl_color = position.unrealized_pl >= 0 ? ImVec4(0.2f, 1.0f, 0.2f, 1.0f) : ImVec4(1.0f, 0.2f, 0.2f, 1.0f);
                ImGui::TextColored(pl_color, "$%.2f", position.unrealized_pl);
                ImGui::NextColumn();
                
                ImGui::Text("%.5f", position.current_price);
                ImGui::NextColumn();
            }
            ImGui::Columns(1);
        }
    }
    ImGui::End();
}

void TradingHUD::renderOrdersPanel() {
    static bool show_orders = true;
    if (!show_orders) return;
    
    ImGui::SetNextWindowPos(ImVec2(1540, 630), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 180), ImGuiCond_FirstUseEver);
    
    if (ImGui::Begin("Pending Orders", &show_orders)) {
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "📋 Pending Orders");
        ImGui::Separator();
        
        if (pending_orders_.empty()) {
            ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "No pending orders");
        } else {
            for (const auto& order : pending_orders_) {
                ImGui::PushID(order.id.c_str());
                
                const char* type_str[] = {"MARKET", "LIMIT", "STOP", "STOP_LIMIT"};
                const char* side_str[] = {"BUY", "SELL"};
                
                ImVec4 side_color = order.side == PendingOrder::BUY ? 
                    ImVec4(0.2f, 1.0f, 0.2f, 1.0f) : ImVec4(1.0f, 0.2f, 0.2f, 1.0f);
                
                ImGui::TextColored(side_color, "%s %s", side_str[order.side], type_str[order.type]);
                ImGui::SameLine();
                ImGui::Text("%.2f @ %.5f", order.size, order.price);
                
                if (ImGui::Button("Cancel")) {
                    cancelOrder(order.id);
                }
                
                ImGui::PopID();
            }
        }
    }
    ImGui::End();
}

void TradingHUD::renderTradeHistory() {
    static bool show_history = false; // Hidden by default to save space
    if (!show_history) return;
    
    ImGui::SetNextWindowPos(ImVec2(200, 500), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(600, 300), ImGuiCond_FirstUseEver);
    
    if (ImGui::Begin("Trade History", &show_history)) {
        ImGui::TextColored(ImVec4(0.8f, 0.6f, 1.0f, 1.0f), "📈 Trade History");
        ImGui::Separator();
        
        if (trade_history_.empty()) {
            ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "No completed trades");
        } else {
            ImGui::Columns(6, "HistoryColumns", true);
            ImGui::Text("Time"); ImGui::NextColumn();
            ImGui::Text("Instrument"); ImGui::NextColumn();
            ImGui::Text("Side"); ImGui::NextColumn();
            ImGui::Text("Size"); ImGui::NextColumn();
            ImGui::Text("Price"); ImGui::NextColumn();
            ImGui::Text("P&L"); ImGui::NextColumn();
            ImGui::Separator();
            
            for (const auto& trade : trade_history_) {
                auto time_t = std::chrono::system_clock::to_time_t(trade.exit_time);
                ImGui::Text("%02d:%02d", 
                    static_cast<int>((time_t % 86400) / 3600),
                    static_cast<int>((time_t % 3600) / 60));
                ImGui::NextColumn();
                
                ImGui::Text("%s", trade.instrument.c_str());
                ImGui::NextColumn();
                
                ImVec4 side_color = trade.side == PendingOrder::BUY ? 
                    ImVec4(0.2f, 1.0f, 0.2f, 1.0f) : ImVec4(1.0f, 0.2f, 0.2f, 1.0f);
                ImGui::TextColored(side_color, "%s", trade.side == PendingOrder::BUY ? "BUY" : "SELL");
                ImGui::NextColumn();
                
                ImGui::Text("%.2f", trade.size);
                ImGui::NextColumn();
                
                ImGui::Text("%.5f", trade.exit_price);
                ImGui::NextColumn();
                
                ImVec4 pl_color = trade.pl >= 0 ? ImVec4(0.2f, 1.0f, 0.2f, 1.0f) : ImVec4(1.0f, 0.2f, 0.2f, 1.0f);
                ImGui::TextColored(pl_color, "$%.2f", trade.pl);
                ImGui::NextColumn();
            }
            ImGui::Columns(1);
        }
    }
    ImGui::End();
}

void TradingHUD::renderPerformanceMetrics() {
    static bool show_performance = true;
    if (!show_performance) return;
    
    ImGui::SetNextWindowPos(ImVec2(1220, 630), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_FirstUseEver);
    
    if (ImGui::Begin("Performance", &show_performance)) {
        ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.2f, 1.0f), "📊 Performance Metrics");
        ImGui::Separator();
        
        // P&L Summary
        ImVec4 total_pl_color = performance_metrics_.total_pl >= 0 ? 
            ImVec4(0.2f, 1.0f, 0.2f, 1.0f) : ImVec4(1.0f, 0.2f, 0.2f, 1.0f);
        ImGui::TextColored(total_pl_color, "Total P&L: $%.2f", performance_metrics_.total_pl);
        
        ImVec4 daily_pl_color = performance_metrics_.daily_pl >= 0 ? 
            ImVec4(0.2f, 1.0f, 0.2f, 1.0f) : ImVec4(1.0f, 0.2f, 0.2f, 1.0f);
        ImGui::TextColored(daily_pl_color, "Daily P&L: $%.2f", performance_metrics_.daily_pl);
        
        ImGui::Separator();
        
        // Trade Statistics
        ImGui::Text("Total Trades: %d", performance_metrics_.total_trades);
        ImGui::Text("Win Rate: %.1f%%", performance_metrics_.win_rate * 100.0);
        
        if (performance_metrics_.total_trades > 0) {
            ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "Winners: %d (Avg: $%.2f)", 
                performance_metrics_.winning_trades, performance_metrics_.avg_win);
            ImGui::TextColored(ImVec4(1.0f, 0.2f, 0.2f, 1.0f), "Losers: %d (Avg: $%.2f)", 
                performance_metrics_.losing_trades, performance_metrics_.avg_loss);
        }
        
        ImGui::Separator();
        
        // Risk Metrics
        ImGui::Text("Profit Factor: %.2f", performance_metrics_.profit_factor);
        ImGui::Text("Max Drawdown: %.2f%%", performance_metrics_.max_drawdown);
        ImGui::Text("Sharpe Ratio: %.2f", performance_metrics_.sharpe_ratio);
    }
    ImGui::End();
}

void TradingHUD::renderAlertsPanel() {
    static bool show_alerts = true;
    if (!show_alerts) return;
    
    ImGui::SetNextWindowPos(ImVec2(10, 850), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(400, 150), ImGuiCond_FirstUseEver);
    
    if (ImGui::Begin("Alerts", &show_alerts)) {
        ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "🔔 Price Alerts");
        ImGui::Separator();
        
        // Add new alert section
        static float alert_price = 0.0f;
        static char alert_message[256] = "";
        static int alert_type = 0;
        
        ImGui::Combo("Type", &alert_type, "Price Above\0Price Below\0SEP Signal\0Indicator Cross\0");
        ImGui::InputFloat("Price", &alert_price, 0.00001f, 0.0001f, "%.5f");
        ImGui::InputText("Message", alert_message, sizeof(alert_message));
        
        if (ImGui::Button("Add Alert")) {
            if (alert_price > 0.0f && strlen(alert_message) > 0) {
                addAlert(static_cast<AlertCondition::Type>(alert_type), alert_price, std::string(alert_message));
                alert_price = 0.0f;
                memset(alert_message, 0, sizeof(alert_message));
            }
        }
        
        ImGui::Separator();
        
        // Active alerts list
        if (active_alerts_.empty()) {
            ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "No active alerts");
        } else {
            for (auto it = active_alerts_.begin(); it != active_alerts_.end();) {
                ImGui::PushID(it->id.c_str());
                
                const char* type_names[] = {"Above", "Below", "SEP", "Cross"};
                const char* status_names[] = {"Active", "Triggered", "Disabled"};
                
                ImVec4 status_color = ImVec4(0.2f, 1.0f, 0.2f, 1.0f);
                if (it->status == AlertCondition::TRIGGERED) status_color = ImVec4(1.0f, 1.0f, 0.2f, 1.0f);
                if (it->status == AlertCondition::DISABLED) status_color = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);
                
                ImGui::TextColored(status_color, "%s %.5f: %s", 
                    type_names[it->type], it->price_level, it->message.c_str());
                
                ImGui::SameLine();
                if (ImGui::Button("X")) {
                    it = active_alerts_.erase(it);
                    ImGui::PopID();
                    continue;
                }
                
                ImGui::PopID();
                ++it;
            }
        }
    }
    ImGui::End();
}

void TradingHUD::renderRiskManager() {
    static bool show_risk_mgr = true;
    if (!show_risk_mgr) return;
    
    ImGui::SetNextWindowPos(ImVec2(420, 850), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(350, 150), ImGuiCond_FirstUseEver);
    
    if (ImGui::Begin("Risk Manager", &show_risk_mgr)) {
        ImGui::TextColored(ImVec4(1.0f, 0.6f, 0.2f, 1.0f), "⚠️ Risk Management");
        ImGui::Separator();
        
        static float risk_percent = 2.0f;
        static float stop_loss_pips = 20.0f;
        static float risk_reward_ratio = 2.0f;
        static bool use_trailing_stop = false;
        static float max_daily_loss = 500.0f;
        
        ImGui::SliderFloat("Risk %", &risk_percent, 0.5f, 5.0f, "%.1f%%");
        ImGui::SliderFloat("Stop Loss (pips)", &stop_loss_pips, 5.0f, 100.0f, "%.0f");
        ImGui::SliderFloat("Risk:Reward", &risk_reward_ratio, 1.0f, 5.0f, "1:%.1f");
        ImGui::Checkbox("Trailing Stop", &use_trailing_stop);
        ImGui::InputFloat("Max Daily Loss", &max_daily_loss, 50.0f, 100.0f, "$%.0f");
        
        ImGui::Separator();
        
        // Position size calculator
        double account_balance = account_info_.balance;
        double risk_amount = account_balance * (risk_percent / 100.0);
        double pip_value = 10.0; // Simplified for demo
        double position_size = risk_amount / (stop_loss_pips * pip_value);
        
        ImGui::Text("Calculated Position Size:");
        ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "%.2f lots (Risk: $%.2f)", 
            position_size, risk_amount);
        
        // Risk warnings
        if (performance_metrics_.daily_pl < -max_daily_loss) {
            ImGui::TextColored(ImVec4(1.0f, 0.2f, 0.2f, 1.0f), "⚠️ DAILY LOSS LIMIT EXCEEDED!");
        }
        
        if (account_info_.margin_used / account_info_.balance > 0.8) {
            ImGui::TextColored(ImVec4(1.0f, 0.6f, 0.2f, 1.0f), "⚠️ High margin usage: %.1f%%", 
                (account_info_.margin_used / account_info_.balance) * 100.0);
        }
    }
    ImGui::End();
}

void TradingHUD::renderAdvancedChart() {
    // Enhanced chart rendering with professional features
    if (candle_data_.empty()) return;
    
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    
    // Render grid lines
    renderChartGrid();
    
    // Render candlesticks with enhanced graphics
    renderCandlesticks();
    
    // Render crosshair
    if (show_crosshair_) {
        renderCrosshair();
    }
    
    // Render technical indicators
    renderTechnicalIndicators();
    renderFibonacciLevels();
    renderIchimokuCloud();
    renderCustomSEPOverlays();
    
    // Render trend lines
    if (show_trend_lines_) {
        renderTrendLines();
    }
    
    // Handle zoom and pan
    handleMouseInput();
}

void TradingHUD::renderCrosshair() {
    if (!ImGui::IsWindowHovered()) return;
    
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 mouse_pos = ImGui::GetMousePos();
    
    // Vertical line
    draw_list->AddLine(
        ImVec2(mouse_pos.x, chart_pos_.y),
        ImVec2(mouse_pos.x, chart_pos_.y + chart_size_.y),
        IM_COL32(128, 128, 128, 128), 1.0f
    );
    
    // Horizontal line
    draw_list->AddLine(
        ImVec2(chart_pos_.x, mouse_pos.y),
        ImVec2(chart_pos_.x + chart_size_.x, mouse_pos.y),
        IM_COL32(128, 128, 128, 128), 1.0f
    );
    
    // Price label
    double price = screenToPrice(mouse_pos.y - chart_pos_.y);
    std::string price_str = std::to_string(price).substr(0, 7);
    
    ImVec2 label_pos = ImVec2(chart_pos_.x + chart_size_.x + 5, mouse_pos.y - 10);
    draw_list->AddRectFilled(
        label_pos,
        ImVec2(label_pos.x + 60, label_pos.y + 20),
        IM_COL32(40, 40, 40, 200)
    );
    draw_list->AddText(ImVec2(label_pos.x + 5, label_pos.y + 3), 
        IM_COL32(255, 255, 255, 255), price_str.c_str());
}

void TradingHUD::renderChartGrid() {
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    
    // Horizontal grid lines (price levels)
    double price_range = price_max_ - price_min_;
    double grid_spacing = price_range / 10.0;
    
    for (int i = 0; i <= 10; i++) {
        double price = price_min_ + (grid_spacing * i);
        float y = chart_pos_.y + chart_size_.y - ((price - price_min_) / price_range) * chart_size_.y;
        
        draw_list->AddLine(
            ImVec2(chart_pos_.x, y),
            ImVec2(chart_pos_.x + chart_size_.x, y),
            IM_COL32(40, 40, 40, 128), 1.0f
        );
    }
    
    // Vertical grid lines (time intervals)
    size_t time_intervals = 8;
    for (size_t i = 0; i <= time_intervals; i++) {
        float x = chart_pos_.x + (chart_size_.x / time_intervals) * i;
        
        draw_list->AddLine(
            ImVec2(x, chart_pos_.y),
            ImVec2(x, chart_pos_.y + chart_size_.y),
            IM_COL32(40, 40, 40, 128), 1.0f
        );
    }
}

void TradingHUD::renderFibonacciLevels() {
    if (candle_data_.size() < 20) return;
    
    // Find significant high and low for Fibonacci retracement
    double swing_high = 0, swing_low = std::numeric_limits<double>::max();
    size_t start_idx = candle_data_.size() > 100 ? candle_data_.size() - 100 : 0;
    
    for (size_t i = start_idx; i < candle_data_.size(); i++) {
        swing_high = std::max(swing_high, candle_data_[i].high);
        swing_low = std::min(swing_low, candle_data_[i].low);
    }
    
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    double range = swing_high - swing_low;
    
    // Fibonacci levels
    const float fib_levels[] = {0.0f, 0.236f, 0.382f, 0.5f, 0.618f, 0.786f, 1.0f};
    const ImU32 fib_colors[] = {
        IM_COL32(128, 128, 128, 100), // 0%
        IM_COL32(255, 255, 0, 100),   // 23.6%
        IM_COL32(255, 165, 0, 100),   // 38.2%
        IM_COL32(255, 0, 0, 120),     // 50%
        IM_COL32(128, 0, 255, 100),   // 61.8%
        IM_COL32(0, 255, 255, 100),   // 78.6%  
        IM_COL32(128, 128, 128, 100)  // 100%
    };
    
    for (int i = 0; i < 7; i++) {
        double fib_price = swing_high - (range * fib_levels[i]);
        ImVec2 start_pos = priceToScreen(fib_price, candle_data_[start_idx].timestamp);
        ImVec2 end_pos = priceToScreen(fib_price, candle_data_.back().timestamp);
        
        draw_list->AddLine(start_pos, end_pos, fib_colors[i], 1.5f);
        
        // Label
        std::string label = std::to_string((int)(fib_levels[i] * 100)) + "%";
        draw_list->AddText(ImVec2(end_pos.x + 5, end_pos.y - 8), 
            IM_COL32(255, 255, 255, 150), label.c_str());
    }
}

void TradingHUD::renderIchimokuCloud() {
    if (candle_data_.size() < 52) return; // Need at least 52 periods for Ichimoku
    
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    std::vector<ImVec2> senkou_a_points, senkou_b_points;
    
    // Calculate simplified Ichimoku lines
    for (size_t i = 26; i < candle_data_.size(); i++) {
        // Tenkan-sen (9-period)
        double tenkan_high = 0, tenkan_low = std::numeric_limits<double>::max();
        for (size_t j = i - 8; j <= i; j++) {
            tenkan_high = std::max(tenkan_high, candle_data_[j].high);
            tenkan_low = std::min(tenkan_low, candle_data_[j].low);
        }
        double tenkan = (tenkan_high + tenkan_low) / 2.0;
        
        // Kijun-sen (26-period)
        double kijun_high = 0, kijun_low = std::numeric_limits<double>::max();
        for (size_t j = i - 25; j <= i; j++) {
            kijun_high = std::max(kijun_high, candle_data_[j].high);
            kijun_low = std::min(kijun_low, candle_data_[j].low);
        }
        double kijun = (kijun_high + kijun_low) / 2.0;
        
        // Senkou Span A (displaced 26 periods ahead)
        double senkou_a = (tenkan + kijun) / 2.0;
        if (i + 26 < candle_data_.size()) {
            senkou_a_points.push_back(priceToScreen(senkou_a, candle_data_[i + 26].timestamp));
        }
        
        // Draw Tenkan and Kijun lines
        if (i > 26) {
            ImVec2 tenkan_pos = priceToScreen(tenkan, candle_data_[i].timestamp);
            ImVec2 kijun_pos = priceToScreen(kijun, candle_data_[i].timestamp);
            
            if (i > 27) {
                double prev_tenkan = (candle_data_[i-1].high + candle_data_[i-1].low) / 2.0; // Simplified
                double prev_kijun = (candle_data_[i-1].high + candle_data_[i-1].low) / 2.0;  // Simplified
                
                ImVec2 prev_tenkan_pos = priceToScreen(prev_tenkan, candle_data_[i-1].timestamp);
                ImVec2 prev_kijun_pos = priceToScreen(prev_kijun, candle_data_[i-1].timestamp);
                
                draw_list->AddLine(prev_tenkan_pos, tenkan_pos, IM_COL32(255, 0, 0, 180), 2.0f);
                draw_list->AddLine(prev_kijun_pos, kijun_pos, IM_COL32(0, 0, 255, 180), 2.0f);
            }
        }
    }
}

void TradingHUD::renderCustomSEPOverlays() {
    if (sep_signals_.empty() || candle_data_.empty()) return;
    
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    
    // SEP Coherence heatmap overlay
    for (size_t i = 1; i < std::min(sep_signals_.size(), candle_data_.size()); i++) {
        const auto& signal = sep_signals_[i];
        const auto& candle = candle_data_[i];
        
        // Color intensity based on coherence
        float intensity = signal.coherence;
        ImU32 coherence_color = IM_COL32(
            static_cast<int>(255 * (1.0f - intensity)),
            static_cast<int>(255 * intensity),
            0,
            static_cast<int>(80 * intensity)
        );
        
        ImVec2 candle_pos = priceToScreen(candle.close, candle.timestamp);
        
        // Draw coherence indicator as small circle
        draw_list->AddCircleFilled(
            ImVec2(candle_pos.x, candle_pos.y - 15),
            3.0f * intensity,
            coherence_color
        );
        
        // Draw trend strength as vertical bar
        float trend_height = 20.0f * signal.trend_strength;
        ImVec2 trend_start = ImVec2(candle_pos.x, chart_pos_.y + chart_size_.y - 30);
        ImVec2 trend_end = ImVec2(candle_pos.x, trend_start.y - trend_height);
        
        ImU32 trend_color = IM_COL32(255, 255, 255, static_cast<int>(150 * signal.trend_strength));
        draw_list->AddLine(trend_start, trend_end, trend_color, 2.0f);
        
        // Draw alpha signal arrows for strong signals
        if (std::abs(signal.alpha_signal) > 0.7f) {
            ImVec2 arrow_pos = ImVec2(candle_pos.x, candle_pos.y + (signal.alpha_signal > 0 ? -25 : 25));
            ImU32 arrow_color = signal.alpha_signal > 0 ? 
                IM_COL32(0, 255, 0, 200) : IM_COL32(255, 0, 0, 200);
            
            // Draw triangle arrow
            ImVec2 p1 = arrow_pos;
            ImVec2 p2 = ImVec2(arrow_pos.x - 5, arrow_pos.y + (signal.alpha_signal > 0 ? 8 : -8));
            ImVec2 p3 = ImVec2(arrow_pos.x + 5, arrow_pos.y + (signal.alpha_signal > 0 ? 8 : -8));
            
            draw_list->AddTriangleFilled(p1, p2, p3, arrow_color);
        }
    }
}

// =============================================================================
// REAL-TIME DATA STREAMING & ORDER MANAGEMENT
// =============================================================================

void TradingHUD::startRealtimeStreaming() {
    if (streaming_active_.load()) return;
    
    streaming_active_.store(true);
    streaming_thread_ = std::thread([this]() {
        while (streaming_active_.load()) {
            // Simulate real-time tick data
            if (oanda_connector_ && !candle_data_.empty()) {
                double last_price = candle_data_.back().close;
                double spread = 0.00015; // 1.5 pips for EUR_USD
                
                // Add some random movement
                double price_change = ((rand() % 1000) - 500) * 0.000001;
                double new_bid = last_price + price_change;
                double new_ask = new_bid + spread;
                
                TickData tick(new_bid, new_ask, std::chrono::system_clock::now());
                
                {
                    std::lock_guard<std::mutex> lock(data_mutex_);
                    tick_queue_.push(tick);
                }
                data_cv_.notify_one();
            }
            
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });
}

void TradingHUD::stopRealtimeStreaming() {
    streaming_active_.store(false);
    if (streaming_thread_.joinable()) {
        streaming_thread_.join();
    }
}

void TradingHUD::onTickReceived(const TickData& tick) {
    std::lock_guard<std::mutex> lock(data_mutex_);
    tick_data_.push_back(tick);
    
    // Keep only recent ticks (last 1000)
    if (tick_data_.size() > 1000) {
        tick_data_.pop_front();
    }
    
    // Update current prices in positions
    for (auto& position : positions_) {
        if (position.instrument == selected_instrument_) {
            position.current_price = (tick.bid + tick.ask) / 2.0;
            position.unrealized_pl = position.size * (position.current_price - position.average_price) * 10000; // Pip value
        }
    }
    
    // Check alerts
    checkAlerts();
}

std::string TradingHUD::placeOrder(PendingOrder::Type type, PendingOrder::Side side, 
                                  double size, double price) {
    std::string order_id = "ORD_" + std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count());
    
    PendingOrder order(type, side, size, price);
    order.id = order_id;
    
    pending_orders_.push_back(order);
    
    // For market orders, execute immediately (simulation)
    if (type == PendingOrder::MARKET && !candle_data_.empty()) {
        double execution_price = candle_data_.back().close;
        
        // Create or update position
        bool position_found = false;
        for (auto& position : positions_) {
            if (position.instrument == selected_instrument_) {
                double new_size = position.size + (side == PendingOrder::BUY ? size : -size);
                if (std::abs(new_size) < 0.001) {
                    // Position closed
                    TradeHistory trade(selected_instrument_, side, std::abs(position.size), 
                                     position.average_price, execution_price,
                                     position.open_time, std::chrono::system_clock::now());
                    trade_history_.push_back(trade);
                    
                    // Remove position
                    positions_.erase(std::remove_if(positions_.begin(), positions_.end(),
                        [&](const Position& p) { return p.instrument == selected_instrument_; }),
                        positions_.end());
                } else {
                    // Update position
                    position.average_price = ((position.average_price * std::abs(position.size)) + 
                                            (execution_price * size)) / (std::abs(position.size) + size);
                    position.size = new_size;
                }
                position_found = true;
                break;
            }
        }
        
        if (!position_found) {
            // Create new position
            positions_.emplace_back(selected_instrument_, 
                side == PendingOrder::BUY ? size : -size, execution_price);
        }
        
        // Remove executed order
        pending_orders_.erase(std::remove_if(pending_orders_.begin(), pending_orders_.end(),
            [&](const PendingOrder& o) { return o.id == order_id; }),
            pending_orders_.end());
        
        // Update performance metrics
        updatePositions();
    }
    
    return order_id;
}

bool TradingHUD::cancelOrder(const std::string& order_id) {
    auto it = std::remove_if(pending_orders_.begin(), pending_orders_.end(),
        [&](const PendingOrder& order) { return order.id == order_id; });
    
    if (it != pending_orders_.end()) {
        pending_orders_.erase(it, pending_orders_.end());
        return true;
    }
    return false;
}

void TradingHUD::updatePositions() {
    // Update performance metrics
    performance_metrics_.total_pl = 0.0;
    for (const auto& position : positions_) {
        performance_metrics_.total_pl += position.unrealized_pl;
    }
    
    for (const auto& trade : trade_history_) {
        performance_metrics_.total_pl += trade.pl;
    }
    
    // Update trade statistics
    performance_metrics_.total_trades = trade_history_.size();
    performance_metrics_.winning_trades = std::count_if(trade_history_.begin(), trade_history_.end(),
        [](const TradeHistory& t) { return t.pl > 0; });
    performance_metrics_.losing_trades = performance_metrics_.total_trades - performance_metrics_.winning_trades;
    
    if (performance_metrics_.total_trades > 0) {
        performance_metrics_.win_rate = static_cast<double>(performance_metrics_.winning_trades) / performance_metrics_.total_trades;
        
        double total_wins = 0, total_losses = 0;
        for (const auto& trade : trade_history_) {
            if (trade.pl > 0) total_wins += trade.pl;
            else total_losses += std::abs(trade.pl);
        }
        
        performance_metrics_.avg_win = performance_metrics_.winning_trades > 0 ? 
            total_wins / performance_metrics_.winning_trades : 0.0;
        performance_metrics_.avg_loss = performance_metrics_.losing_trades > 0 ? 
            total_losses / performance_metrics_.losing_trades : 0.0;
        performance_metrics_.profit_factor = total_losses > 0 ? total_wins / total_losses : 0.0;
    }
}

void TradingHUD::addAlert(AlertCondition::Type type, double price_level, const std::string& message) {
    std::string alert_id = "ALERT_" + std::to_string(active_alerts_.size());
    AlertCondition alert(type, selected_instrument_, message);
    alert.id = alert_id;
    alert.price_level = price_level;
    active_alerts_.push_back(alert);
}

void TradingHUD::checkAlerts() {
    if (candle_data_.empty()) return;
    
    double current_price = candle_data_.back().close;
    
    for (auto& alert : active_alerts_) {
        if (alert.status != AlertCondition::ACTIVE) continue;
        
        bool triggered = false;
        
        switch (alert.type) {
            case AlertCondition::PRICE_ABOVE:
                triggered = current_price > alert.price_level;
                break;
            case AlertCondition::PRICE_BELOW:
                triggered = current_price < alert.price_level;
                break;
            case AlertCondition::SEP_SIGNAL:
                // Check for strong SEP signals
                if (!sep_signals_.empty()) {
                    const auto& latest_signal = sep_signals_.back();
                    triggered = std::abs(latest_signal.alpha_signal) > 0.8f;
                }
                break;
            default:
                break;
        }
        
        if (triggered) {
            alert.status = AlertCondition::TRIGGERED;
            alert.triggered_time = std::chrono::system_clock::now();
            
            // Visual/audio notification would go here
            std::cout << "[ALERT] " << alert.message << " at price " << current_price << std::endl;
        }
    }
}

} // namespace sep::workbench
