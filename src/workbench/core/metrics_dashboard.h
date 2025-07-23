#pragma once

#include "metrics_monitor.h"
#include "memory_monitor.hpp"
#include "file_dialog.hpp"
#include "oanda_connector.h"
#include <memory>
#include <vector>
#include <string>
#include <mutex>
#include <deque>
#include <map>

#include "trade_manager.h"

namespace sep::workbench {
    namespace core {
        class TradeManager;
    }

// Candlestick data structure for OHLC charts
struct Candlestick {
    double timestamp;
    double open;
    double high;
    double low;
    double close;
    double volume;
    
    Candlestick(double t = 0, double o = 0, double h = 0, double l = 0, double c = 0, double v = 0)
        : timestamp(t), open(o), high(h), low(l), close(c), volume(v) {}
};

// Technical indicator calculations
struct TechnicalIndicators {
    std::vector<double> sma_20;
    std::vector<double> sma_50;
    std::vector<double> ema_12;
    std::vector<double> ema_26;
    std::vector<double> rsi;
    std::vector<double> macd;
    std::vector<double> macd_signal;
    std::vector<double> bollinger_upper;
    std::vector<double> bollinger_lower;
    std::vector<double> atr;
    std::vector<double> timestamps;
};

// Market depth data
struct MarketDepth {
    std::vector<std::pair<double, double>> bids; // price, volume
    std::vector<std::pair<double, double>> asks; // price, volume
};

/**
 * @brief ImGui-based dashboard for real-time SEP metrics visualization
 *
 * Provides an investor-ready interface showing:
 * - Real-time coherence, stability, entropy graphs
 * - Pattern detection statistics
 * - Data source selection and control
 * - Export and analysis tools
 * - Advanced OANDA market data visualization with candlesticks and indicators
 */
class MetricsDashboard {
public:
    // 48-hour sliding window cache
    struct Chart24HrCache {
        // Minute-by-minute data for two full days (used for a
        // 24-hour sliding window view and short-term analysis)
        std::vector<sep::connectors::OandaCandle> minute_data;  // 2880 minutes = 48 hours
        std::string instrument;
        std::chrono::system_clock::time_point last_update;
        bool is_valid{false};
        
        // Coherence metrics for this 48hr snapshot
        double coherence_metric{0.0};
        double stability_metric{0.0};
        double entropy_metric{0.0};
        std::chrono::system_clock::time_point metrics_calculated;
    };
    
    MetricsDashboard();
    ~MetricsDashboard();

    bool initialize();
    void setTradeManager(TradeManager* trade_manager);
    void shutdown();
    void render();  // Call this in ImGui render loop

    // Dashboard state
    bool isVisible() const { return show_dashboard_; }
    void setVisible(bool visible) { show_dashboard_ = visible; }

private:
    void renderControlPanel();
    void renderSystemMetrics();
    void renderPatternList();
    void renderMetricsGraphs();
    void renderDataSourceSelector();
    void renderMemoryMonitor();
    
    // Control panel actions
    void handleDataLoad();
    void handleClearData();
    void handleExportMetrics();
    void handleStartProcessing();
    void handleStopProcessing();

    std::unique_ptr<MetricsMonitor> monitor_;
    std::unique_ptr<MemoryMonitor> memory_monitor_;
    FileDialog file_dialog_;
    
    // UI State
    bool show_dashboard_{true};
    bool show_control_panel_{true};
    bool show_system_metrics_{true};
    bool show_pattern_list_{true};
    bool show_metrics_graphs_{true};
    bool show_memory_monitor_{true};
    
    // Data source selection
    char file_path_buffer_[512] = "";
    int data_source_type_{0};  // 0=File, 1=Live Stream, 2=Generated
    
    // Graph data for real-time plotting
    std::vector<float> coherence_history_;
    std::vector<float> stability_history_;
    std::vector<float> entropy_history_;
    size_t max_history_size_{1000};
    
    // Pattern filtering
    float min_coherence_filter_{0.0f};
    float min_stability_filter_{0.0f};
    float max_entropy_filter_{1.0f};
    int pattern_sort_mode_{0};  // 0=Coherence, 1=Stability, 2=Frequency
    
    // Export settings
    char export_path_buffer_[512] = "metrics_export.json";
    
    // Memory monitoring
    std::vector<float> memory_history_;
    std::vector<float> memory_growth_history_;
    bool auto_monitor_memory_{true};
    
    // OANDA integration
    std::unique_ptr<sep::connectors::OandaConnector> oanda_connector_;
    TradeManager* trade_manager_{nullptr};
    bool oanda_connected_{false};
    std::string oanda_status_{"Disconnected"};
    bool use_oanda_data_{false};
    
    // OANDA helper methods
    void initializeOandaConnection();
    void updateOandaData();
    void renderOandaPanel();
    void renderOandaMainView();
    void fetchHistoricalData();
    void renderHistoricalChart();
    void updateInstrumentCache(const std::string& instrument);
    void calculateSnapshotMetrics(Chart24HrCache& cache);
    bool needsCacheUpdate(const Chart24HrCache& cache);
    void renderCandlestickChart(const std::string& instrument);
    void renderTechnicalIndicators(const std::string& instrument);
    void renderMarketDepth(const std::string& instrument);
    void renderPatternMetrics();
    void updateCandlestickData(const std::string& instrument, const sep::connectors::MarketData& data);
    void calculateTechnicalIndicators(const std::string& instrument);
    void renderOHLCCandlesticks(const std::deque<Candlestick>& candles, float min_price, float max_price, float price_range);
    void renderSEPSignalOverlay(const std::string& instrument, float min_price, float max_price, float price_range);
    void renderSEPMetricChart(const std::string& instrument);
    
    // Market data storage
    std::mutex market_data_mutex_;
    std::map<std::string, std::deque<Candlestick>> candlestick_data_;
    std::map<std::string, TechnicalIndicators> technical_indicators_;
    std::map<std::string, MarketDepth> market_depth_;
    std::map<std::string, sep::connectors::MarketData> latest_prices_;
    
    // Chart settings
    int max_candlesticks_{500};
    bool show_sma_{true};
    bool show_ema_{true};
    bool show_rsi_{true};
    bool show_macd_{true};
    bool show_bollinger_{true};
    bool show_volume_{true};
    std::string selected_instrument_{"EUR_USD"};
    
    // Historical data and live streaming
    bool live_data_enabled_{false};
    std::vector<sep::connectors::OandaCandle> historical_data_;
    bool historical_data_loaded_{false};
    
    std::map<std::string, Chart24HrCache> instrument_cache_;
    std::chrono::minutes cache_update_interval_{5}; // Update every 5 minutes
};

} // namespace sep::workbench
