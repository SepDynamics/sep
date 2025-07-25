#pragma once

#include <deque>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "core/common_structs.h"
#include "imgui.h"

namespace sep::workbench {

class SignalsTabController {
public:
    SignalsTabController();
    ~SignalsTabController();

    bool initialize();
    void render();
    void shutdown();

    void setCandleData(const std::deque<CandleData>& data);
    void setSEPSignals(const std::deque<SEPSignalData>& signals);

private:
    // Chart data
    std::deque<CandleData> candle_data_;
    std::deque<SEPSignalData> sep_signals_;
    std::unordered_map<std::string, TechnicalIndicator> indicators_;
    std::vector<TrendLine> trend_lines_;
    EnhancedHoverInfo hover_info_;

    // Chart interaction
    ChartZoom chart_zoom_;
    ImVec2 crosshair_pos_;
    bool show_crosshair_ = true;
    bool is_panning_ = false;
    ImVec2 pan_start_pos_;

    // Settings
    std::string selected_instrument_ = "EUR_USD";
    bool show_volume_ = true;
    bool show_sep_overlay_ = true;
    bool show_grid_ = true;
    bool show_trend_lines_ = true;
    bool auto_detect_trends_ = true;

    // Chart dimensions and state
    ImVec2 chart_size_;
    ImVec2 chart_pos_;
    double price_min_, price_max_;
    float volume_max_;

    // Rendering functions
    void renderMainChart();
    void renderCandlesticks();
    void renderTechnicalIndicators();
    void renderSEPSignalOverlay();
    void renderVolumeChart();
    void renderTrendLines();
    void renderHoverInfo();
    void renderCrosshair();
    void renderChartGrid();

    // Utility functions
    ImVec2 priceToScreen(double price, std::chrono::system_clock::time_point time);
    double screenToPrice(float y);
    std::chrono::system_clock::time_point screenToTime(float x);
    void updatePriceRange();
    void setupChartArea();
    void handleMouseInput();
    void updateHoverInfo();
    void calculateEnhancedHoverMetrics();
    void detectTrendLines();
    ImU32 getSignalColor(SEPSignalData::SignalType signal_type);
    ImU32 getCandleColor(const CandleData& candle, bool is_body = true);
};

} // namespace sep::workbench
