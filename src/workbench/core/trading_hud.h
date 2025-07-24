#pragma once

#include <vector>
#include <unordered_map>
#include <chrono>
#include <deque>
#include <memory>
#include <thread>
#include <atomic>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <imgui.h>
#include "connectors/oanda_connector.h"
#include "metrics_monitor.h"

namespace sep::workbench {

// Enhanced real-time data structures
struct TickData {
    double bid, ask, spread;
    std::chrono::system_clock::time_point timestamp;
    int volume_tick = 0;
    
    TickData(double b, double a, std::chrono::system_clock::time_point t)
        : bid(b), ask(a), spread(a - b), timestamp(t) {}
};

struct OrderBookLevel {
    double price;
    double size;
    int order_count;
    
    OrderBookLevel(double p, double s, int c = 1) : price(p), size(s), order_count(c) {}
};

struct OrderBook {
    std::vector<OrderBookLevel> bids;
    std::vector<OrderBookLevel> asks;
    std::chrono::system_clock::time_point timestamp;
    
    void clear() { bids.clear(); asks.clear(); }
};

struct PendingOrder {
    enum Type { MARKET, LIMIT, STOP, STOP_LIMIT };
    enum Side { BUY, SELL };
    
    std::string id;
    Type type;
    Side side;
    double size;
    double price;
    double stop_loss = 0.0;
    double take_profit = 0.0;
    std::chrono::system_clock::time_point created_time;
    std::string status = "PENDING";
    
    PendingOrder(Type t, Side s, double sz, double p) 
        : type(t), side(s), size(sz), price(p), 
          created_time(std::chrono::system_clock::now()) {}
};

struct Position {
    std::string instrument;
    double size; // positive for long, negative for short
    double average_price;
    double current_price;
    double unrealized_pl;
    double realized_pl;
    std::chrono::system_clock::time_point open_time;
    
    Position(const std::string& inst, double sz, double avg_price)
        : instrument(inst), size(sz), average_price(avg_price), 
          current_price(avg_price), unrealized_pl(0), realized_pl(0),
          open_time(std::chrono::system_clock::now()) {}
};

struct TradeHistory {
    std::string id;
    std::string instrument;
    PendingOrder::Side side;
    double size;
    double entry_price;
    double exit_price;
    double pl;
    std::chrono::system_clock::time_point entry_time;
    std::chrono::system_clock::time_point exit_time;
    std::string strategy = "Manual";
    
    TradeHistory(const std::string& inst, PendingOrder::Side s, double sz, 
                 double entry, double exit, std::chrono::system_clock::time_point et,
                 std::chrono::system_clock::time_point ext)
        : instrument(inst), side(s), size(sz), entry_price(entry), exit_price(exit),
          pl((s == PendingOrder::BUY ? (exit - entry) : (entry - exit)) * sz),
          entry_time(et), exit_time(ext) {}
};

struct ChartZoom {
    double price_min = 0, price_max = 0;
    std::chrono::system_clock::time_point time_start, time_end;
    bool is_zoomed = false;
    ImVec2 zoom_start, zoom_end;
    bool is_selecting = false;
};

struct AlertCondition {
    enum Type { PRICE_ABOVE, PRICE_BELOW, SEP_SIGNAL, INDICATOR_CROSS };
    enum Status { ACTIVE, TRIGGERED, DISABLED };
    
    std::string id;
    Type type;
    Status status = ACTIVE;
    std::string instrument;
    double price_level = 0.0;
    std::string message;
    bool sound_alert = true;
    bool popup_alert = true;
    std::chrono::system_clock::time_point created_time;
    std::chrono::system_clock::time_point triggered_time;
    
    AlertCondition(Type t, const std::string& inst, const std::string& msg)
        : type(t), instrument(inst), message(msg), 
          created_time(std::chrono::system_clock::now()) {}
};

struct TechnicalIndicator {
    std::string name;
    std::vector<float> values;
    ImU32 color;
    float line_thickness = 1.5f;
    bool enabled = true;
    
    TechnicalIndicator() : name(""), color(IM_COL32(255, 255, 255, 255)) {}
    TechnicalIndicator(const std::string& n, ImU32 c) : name(n), color(c) {}
};

struct TrendLine {
    ImVec2 start_point;
    ImVec2 end_point;
    double start_price;
    double end_price;
    std::chrono::system_clock::time_point start_time;
    std::chrono::system_clock::time_point end_time;
    ImU32 color;
    float thickness = 2.0f;
    bool is_support = true; // true for support, false for resistance
    float strength = 1.0f; // number of touches/confirmations
    
    TrendLine() : start_price(0), end_price(0), color(IM_COL32(255, 255, 0, 255)) {}
};

struct SEPSignalData {
    float coherence;
    float stability;
    float entropy;
    float alpha_signal;
    float trend_strength;
    std::chrono::system_clock::time_point timestamp;
    
    // Trading signal interpretation
    enum SignalType {
        STRONG_BUY,
        BUY,
        NEUTRAL,
        SELL,
        STRONG_SELL
    } signal_type;
    
    SEPSignalData() : coherence(0), stability(0), entropy(0), alpha_signal(0), 
                      trend_strength(0), signal_type(NEUTRAL) {}
};

struct CandleData {
    double open, high, low, close;
    int volume;
    std::chrono::system_clock::time_point timestamp;
    
    CandleData(double o, double h, double l, double c, int v, 
               std::chrono::system_clock::time_point t)
        : open(o), high(h), low(l), close(c), volume(v), timestamp(t) {}
};

struct HoverInfo {
    bool active = false;
    ImVec2 position;
    double price;
    std::chrono::system_clock::time_point time;
    CandleData* nearest_candle = nullptr;
    SEPSignalData* nearest_sep_signal = nullptr;
};

class TradingHUD {
public:
TradingHUD();
~TradingHUD();

bool initialize();
void render();
void shutdown();

void setOandaConnector(std::shared_ptr<sep::connectors::OandaConnector> connector);
void setMetricsMonitor(std::shared_ptr<MetricsMonitor> monitor);

// Data management
void updateCandleData(const std::vector<sep::connectors::OandaCandle>& oanda_candles);
void updateSEPSignals();
void calculateTechnicalIndicators();
    
    // Real-time data streaming
void startRealtimeStreaming();
void stopRealtimeStreaming();
void onTickReceived(const TickData& tick);
void onOrderBookUpdate(const OrderBook& order_book);

// Order management
std::string placeOrder(PendingOrder::Type type, PendingOrder::Side side, 
                      double size, double price = 0.0);
bool modifyOrder(const std::string& order_id, double new_price, double new_size);
bool cancelOrder(const std::string& order_id);
    void updatePositions();
    
    // Alert management
    void addAlert(AlertCondition::Type type, double price_level, const std::string& message);
    void checkAlerts();
    void removeAlert(const std::string& alert_id);

private:
    // Core components
    std::shared_ptr<sep::connectors::OandaConnector> oanda_connector_;
    std::shared_ptr<MetricsMonitor> metrics_monitor_;
    
    // Chart data
    std::deque<CandleData> candle_data_;
    std::deque<SEPSignalData> sep_signals_;
    std::unordered_map<std::string, TechnicalIndicator> indicators_;
    std::vector<TrendLine> trend_lines_;
    HoverInfo hover_info_;
    
    // Real-time data
    std::deque<TickData> tick_data_;
    OrderBook current_order_book_;
    std::atomic<bool> streaming_active_{false};
    std::thread streaming_thread_;
    std::mutex data_mutex_;
    std::condition_variable data_cv_;
    std::queue<TickData> tick_queue_;
    
    // Trading data
    std::vector<PendingOrder> pending_orders_;
    std::vector<Position> positions_;
    std::vector<TradeHistory> trade_history_;
    std::vector<AlertCondition> active_alerts_;
    
    // Chart interaction
    ChartZoom chart_zoom_;
    ImVec2 crosshair_pos_;
    bool show_crosshair_ = true;
    bool is_panning_ = false;
    ImVec2 pan_start_pos_;
    
    // Performance tracking
    struct PerformanceMetrics {
        double total_pl = 0.0;
        double daily_pl = 0.0;
        double weekly_pl = 0.0;
        double monthly_pl = 0.0;
        int total_trades = 0;
        int winning_trades = 0;
        int losing_trades = 0;
        double win_rate = 0.0;
        double avg_win = 0.0;
        double avg_loss = 0.0;
        double profit_factor = 0.0;
        double max_drawdown = 0.0;
        double sharpe_ratio = 0.0;
    } performance_metrics_;
    
    // Settings
    std::string selected_instrument_ = "EUR_USD";
    bool live_mode_ = false;
    bool show_volume_ = true;
    bool show_sep_overlay_ = true;
    bool show_grid_ = true;
    bool show_trend_lines_ = true;
    bool auto_detect_trends_ = true;
    
    // Multi-timeframe settings
    enum TimeFrame { TF_24H, TF_6H, TF_3H };
    TimeFrame current_timeframe_ = TF_24H;
    
    // Chart dimensions and state
    ImVec2 chart_size_;
    ImVec2 chart_pos_;
    double price_min_, price_max_;
    float volume_max_;
    
    // Time range (24 hours of minute candles = 1440 candles)
    static constexpr size_t MAX_CANDLES = 1440;
    static constexpr size_t INDICATOR_PERIODS[] = {9, 21, 50, 200}; // EMA periods
    
    // Rendering functions
    void renderTopBar();
    void renderMainChart();
    void renderCandlesticks();
    void renderTechnicalIndicators();
    void renderSEPSignalOverlay();
    void renderVolumeChart();
    void renderSEPMetricsPanel();
    void renderTradingControls();
    void renderTrendLines();
    void renderHoverInfo();
    void renderAccountBalance();
    void renderTimeFrameSelector();
    void renderOrderBook();
    void renderPositionsPanel();
    void renderOrdersPanel();
    void renderTradeHistory();
    void renderPerformanceMetrics();
    void renderAlertsPanel();
    void renderRiskManager();
    void renderAdvancedChart();
    void renderCrosshair();
    void renderChartGrid();
    void renderFibonacciLevels();
    void renderIchimokuCloud();
    void renderCustomSEPOverlays();
    
    // Technical analysis
    void calculateMovingAverages();
    void calculateRSI();
    void calculateMACD();
    void calculateBollingerBands();
    void calculateSEPTrendLines();
    void detectTrendLines();
    void calculateSupportResistance();
    void calculateFibonacciRetracements();
    void calculateIchimokuCloud();
    void calculateVWAP();
    void calculateStochasticOscillator();
    void calculateATR();
    void calculatePivotPoints();
    
    // Advanced SEP analysis
    void calculateSEPDivergence();
    void calculateSEPMomentum();
    void calculateSEPVolatility();
    void generateSEPSignals();
    void optimizeSEPParameters();
    
    // Utility functions
    ImVec2 priceToScreen(double price, std::chrono::system_clock::time_point time);
    double screenToPrice(float y);
    std::chrono::system_clock::time_point screenToTime(float x);
    void updatePriceRange();
    void setupChartArea();
    void handleMouseInput();
    void updateHoverInfo();
    void setDefaultWindowPositions();
    
    // SEP signal processing
    SEPSignalData::SignalType interpretSEPSignal(float coherence, float stability, float entropy);
    float calculateAlphaSignal(const SEPSignalData& signal);
    float calculateTrendStrength(const std::deque<SEPSignalData>& recent_signals);
    
    // Color schemes
    ImU32 getSignalColor(SEPSignalData::SignalType signal_type);
    ImU32 getCandleColor(const CandleData& candle, bool is_body = true);
    
    // Performance optimization
    void trimDataToTimeWindow();
    bool shouldUpdateIndicators();
    
    // State tracking
    std::chrono::steady_clock::time_point last_update_;
    std::chrono::steady_clock::time_point last_indicator_calc_;
    bool data_changed_ = false;
    
    // Account data
    struct AccountInfo {
        double balance = 10000.0;
        double equity = 10000.0;
        double margin_used = 0.0;
        double margin_available = 10000.0;
        double unrealized_pl = 0.0;
        double realized_pl = 0.0;
        std::string currency = "USD";
    } account_info_;
    
    // Window management
    struct WindowPositions {
        ImVec2 trading_hud_pos = ImVec2(10, 10);
        ImVec2 trading_hud_size = ImVec2(1200, 800);
        ImVec2 account_balance_pos = ImVec2(1220, 10);
        ImVec2 account_balance_size = ImVec2(300, 200);
        ImVec2 sep_metrics_pos = ImVec2(1220, 220);
        ImVec2 sep_metrics_size = ImVec2(300, 400);
        bool positions_set = false;
    } window_positions_;
};

} // namespace sep::workbench
