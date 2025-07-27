#pragma once

#include <deque>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <map>
#include <mutex>

#include "common/financial_data_types.h"
#include "imgui.h"
#include "implot.h"
#include "connectors/oanda_connector.h"
#include "signal_generator/quantum_signal_generator.h"
#include "apps/workbench/config.hpp"
#include "apps/workbench/core/common_structs.h"
#include "apps/workbench/core/multi_timeframe_analyzer.h"

namespace sep::workbench {

#include "../core/metrics_monitor.h"

#include "../core/workbench_core.hpp"

class SignalsTabController {
public:
    SignalsTabController();
    ~SignalsTabController();

    bool initialize();
    void render();
    void renderThresholdControlPanel();
    void shutdown();

    void setOandaConnector(sep::connectors::OandaConnector* connector);
    void setQuantumSignalGenerator(QuantumSignalGenerator* generator);
    void setMetricsMonitor(std::shared_ptr<MetricsMonitor> monitor);
    void setWorkbenchEngine(WorkbenchEngine* engine);
    void setLatestMetrics(const std::map<std::string, TimeframeMetrics>& metrics);
    void setMultiTimeframeAnalyzer(MultiTimeframeAnalyzer* analyzer) { mtf_analyzer_ = analyzer; }

        void setCandleData(const std::deque<sep::common::CandleData>& data);
        void setCandleData(const std::vector<sep::common::CandleData>& data);
        void addCandle(const sep::common::CandleData& candle);
        const std::deque<sep::common::CandleData>& getCandleData() const { return candle_data_; }
        void setSEPSignals(const std::deque<sep::common::SEPSignalData>& signals);

private:
    sep::connectors::OandaConnector* oanda_connector_ = nullptr;
    QuantumSignalGenerator* signal_generator_ = nullptr;
    std::shared_ptr<MetricsMonitor> metrics_monitor_;
    WorkbenchEngine* workbench_engine_ = nullptr;
    MultiTimeframeAnalyzer* mtf_analyzer_ = nullptr;
    // Chart data
        std::deque<sep::common::CandleData> candle_data_;
        std::deque<sep::common::SEPSignalData> sep_signals_;
    std::unordered_map<std::string, TechnicalIndicator> indicators_;
    std::vector<TrendLine> trend_lines_;
    EnhancedHoverInfo hover_info_;
    std::map<std::string, TimeframeMetrics> latest_tf_metrics_;
    std::mutex metrics_mutex_;
    bool metrics_updated_{false};

    // Chart interaction
    ChartZoom chart_zoom_;
    ImVec2 crosshair_pos_;
    double crosshair_index_ = 0.0;
    double crosshair_price_ = 0.0;
    bool show_crosshair_ = true; // toggled with Space key
    bool is_panning_ = false;
    ImVec2 pan_start_pos_;

    // Settings
    std::string selected_instrument_ = "EUR_USD";
    bool show_volume_ = true;
    bool show_sep_overlay_ = true;
    bool show_grid_ = true;
    bool show_trend_lines_ = true;
    bool auto_detect_trends_ = true;

    // Threshold settings for signal generation
    float buy_min_coherence_ = 0.7f;
    float buy_min_stability_ = 0.6f;
    float buy_max_entropy_ = 0.3f;
    float sell_max_stability_ = 0.3f;
    float sell_min_entropy_ = 0.7f;

    // Chart dimensions and state
    ImVec2 chart_size_;
    ImVec2 chart_pos_;
    double price_min_, price_max_;
    float volume_max_;
    ImPlotFlags plot_flags_ = ImPlotFlags_None;
    bool candle_data_updated_ = false;
    bool sep_signals_updated_ = false;

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
    void renderMetricPlot(const char* label,
                          const std::deque<float>& short_hist,
                          const std::deque<float>& long_hist,
                          const ImVec4& short_color,
                          const ImVec4& long_color);
    void renderMetricsGraphs();

    std::deque<float> coherence_history_;
    std::deque<float> stability_history_;
    std::deque<float> entropy_history_;
    std::deque<float> coherence_history_1h_;
    std::deque<float> coherence_history_4h_;
    std::deque<float> stability_history_1h_;
    std::deque<float> stability_history_4h_;
    std::deque<float> entropy_history_1h_;
    std::deque<float> entropy_history_4h_;

    float metrics_scale_ = 1.0f;
    float metrics_offset_ = 0.0f;
    bool metrics_panning_ = false;
    ImVec2 metrics_pan_start_{};

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
    ImU32 getSignalColor(sep::common::MultiTimeframeSignal signal_type);
    ImU32 getCandleColor(const sep::common::CandleData& candle, bool is_body = true);
};

} // namespace sep::workbench
