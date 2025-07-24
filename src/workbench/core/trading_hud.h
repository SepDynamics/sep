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
#include "trade_manager.h"
#include "forex_pattern_generator.h"
#include "ui_layout_manager.h"
#include "multi_timeframe_analyzer.h"

// SEP Engine Components
#include "engine/engine.h"
#include "engine/metrics_collector.h"
#include "quantum/pattern_metric_engine.h"
#include "quantum/coherence_manager.h"
#include "quantum/quantum_processor.h"

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
    
    // Real market data from OANDA
    float atr;              // Average True Range
    int volatility_level;   // Volatility level (1-4)
    float bid;              // Current bid price
    float ask;              // Current ask price
    float spread;           // Bid-ask spread
    
    // Trading signal interpretation
    enum SignalType {
        STRONG_BUY,
        BUY,
        NEUTRAL,
        SELL,
        STRONG_SELL
    } signal_type;
    
    SEPSignalData() : coherence(0), stability(0), entropy(0), alpha_signal(0), 
                      trend_strength(0), atr(0), volatility_level(1), bid(0), ask(0), 
                      spread(0), signal_type(NEUTRAL) {}
};

struct CandleData {
    double open, high, low, close;
    int volume;
    std::chrono::system_clock::time_point timestamp;
    
    CandleData(double o, double h, double l, double c, int v, 
               std::chrono::system_clock::time_point t)
        : open(o), high(h), low(l), close(c), volume(v), timestamp(t) {}
};

struct EnhancedHoverInfo {
    bool active = false;
    ImVec2 position;
    double price;
    std::chrono::system_clock::time_point time;
    CandleData* nearest_candle = nullptr;
    SEPSignalData* nearest_sep_signal = nullptr;
    
    // Enhanced derived metrics
    float price_momentum = 0.0f;      // Rate of price change
    float volume_profile = 0.0f;      // Volume at this price level
    float sep_divergence = 0.0f;      // Divergence between price and SEP signal
    
    // Multi-timeframe data
    std::map<std::string, float> mtf_coherence; // 1m, 5m, 15m, 1h coherence
    
    // Support/Resistance proximity
    float nearest_support = 0.0f;
    float nearest_resistance = 0.0f;
    float s_r_strength = 0.0f;
    
    // Traditional indicators at hover point
    float rsi_value = 0.0f;
    float macd_value = 0.0f;
    float bb_position = 0.0f;  // Position within Bollinger Bands (0-1)
    
    // Market context
    float market_correlation = 0.0f;  // Correlation with other pairs
    std::string market_regime = "Unknown";  // Trending, Ranging, Breakout
};

// Coherence Trading Strategy
struct CoherenceStrategy {
    bool enabled = false;
    float coherence_threshold = 0.5f;  // Base coherence level (50%)
    float buy_threshold_offset = 0.1f;  // Buy when coherence > 50% + offset
    float sell_threshold_offset = 0.1f; // Sell when coherence < 50% - offset
    
    // Risk management
    double position_size_units = 1000.0;  // Base position size
    double stop_loss_pips = 20.0;         // Stop loss in pips
    double max_positions = 3;             // Maximum concurrent positions
    
    // Current state
    float last_coherence = 0.0f;
    int current_positions = 0;
    std::chrono::system_clock::time_point last_trade_time;
    
    // Strategy statistics
    int total_trades = 0;
    int winning_trades = 0;
    double total_pnl = 0.0;
    
    bool shouldBuy(float coherence) const {
        return coherence > (coherence_threshold + buy_threshold_offset);
    }
    
    bool shouldSell(float coherence) const {
        return coherence < (coherence_threshold - sell_threshold_offset);
    }
    
    bool canTrade() const {
        return enabled && current_positions < max_positions;
    }
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

// SEP Engine Integration
void initializeSEPEngine();
void updateEngineMetrics();
void ingestMarketDataToEngine(const std::vector<sep::connectors::OandaCandle>& oanda_candles);
void processQuantumSignals();
void updateCoherenceMetrics();
std::vector<sep::quantum::PatternMetrics> getCurrentPatternMetrics();
sep::quantum::CoherenceManager::CoherenceMetrics getCoherenceMetrics();
void resetEngineState();
    
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
    
    // Coherence trading strategy
    void updateCoherenceStrategy();
    void executeCoherenceTrade(bool is_buy);
    float calculateCurrentCoherence();
    void resetCoherenceStrategy();
    void calculateMarketCorrelations(float correlation_matrix[6][6], const char* pairs[], int num_pairs);
    
    // Forex pattern validation
    bool validateMarketConditions(float coherence, float stability, float entropy);

private:
    // Core components
    std::shared_ptr<sep::connectors::OandaConnector> oanda_connector_;
    std::shared_ptr<MetricsMonitor> metrics_monitor_;
    std::unique_ptr<TradeManager> trade_manager_;
    
    // SEP Engine Components
    std::unique_ptr<sep::core::Engine> sep_engine_;
    std::unique_ptr<sep::core::MetricsCollector> sep_metrics_collector_;
    std::unique_ptr<sep::quantum::PatternMetricEngine> pattern_engine_;
    std::unique_ptr<sep::quantum::CoherenceManager> coherence_manager_;
    std::unique_ptr<sep::quantum::QuantumProcessor> quantum_processor_;
    
    // Chart data
    std::deque<CandleData> candle_data_;
    std::deque<SEPSignalData> sep_signals_;
    std::unordered_map<std::string, TechnicalIndicator> indicators_;
    std::vector<TrendLine> trend_lines_;
    EnhancedHoverInfo hover_info_;
    
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
    
    // Coherence trading strategy
    CoherenceStrategy coherence_strategy_;
    
    // UI Layout Manager
    std::unique_ptr<UILayoutManager> layout_manager_;
    
    // Multi-timeframe Analyzer
    std::unique_ptr<MultiTimeframeAnalyzer> mtf_analyzer_;
    MultiTimeframeSignal latest_mtf_signal_;
    
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
    void renderMarketCorrelationMatrix();
    void renderCoherenceStrategyControls();
    void renderAdvancedChart();
    void renderCrosshair();
    void renderChartGrid();
    void renderFibonacciLevels();
    void renderIchimokuCloud();
    void renderCustomSEPOverlays();
    
    // Multi-timeframe analysis rendering
    void renderMultiTimeframePanel();
    void renderTimeframeMetricsTable();
    void renderTimeframeAlignmentChart();
    void renderGenerativeAnalysisTab();
    
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
    void calculateEnhancedHoverMetrics();
    void calculateSupportResistanceLevels();
    void calculateTraditionalIndicatorsAtPoint();
    void setDefaultWindowPositions();
    void setupLayoutPanels();
    
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
        // Main chart window - 70% of screen
        ImVec2 trading_hud_pos = ImVec2(10, 10);
        ImVec2 trading_hud_size = ImVec2(1200, 800);
        
        // Right panel windows - 30% of screen
        ImVec2 account_balance_pos = ImVec2(1220, 10);
        ImVec2 account_balance_size = ImVec2(300, 200);
        ImVec2 sep_metrics_pos = ImVec2(1220, 220);
        ImVec2 sep_metrics_size = ImVec2(300, 400);
        
        // Additional trading windows
        ImVec2 order_book_pos = ImVec2(1540, 10);
        ImVec2 order_book_size = ImVec2(300, 400);
        ImVec2 positions_panel_pos = ImVec2(1540, 420);
        ImVec2 positions_panel_size = ImVec2(300, 200);
        ImVec2 orders_panel_pos = ImVec2(1540, 630);
        ImVec2 orders_panel_size = ImVec2(300, 180);
        
        // Analysis windows
        ImVec2 trade_history_pos = ImVec2(200, 500);
        ImVec2 trade_history_size = ImVec2(600, 300);
        ImVec2 performance_metrics_pos = ImVec2(1220, 630);
        ImVec2 performance_metrics_size = ImVec2(300, 200);
        
        // Bottom panels
        ImVec2 alerts_panel_pos = ImVec2(10, 850);
        ImVec2 alerts_panel_size = ImVec2(400, 150);
        ImVec2 risk_manager_pos = ImVec2(420, 850);
        ImVec2 risk_manager_size = ImVec2(350, 150);
        
        // Market correlation matrix (new)
        ImVec2 correlation_pos = ImVec2(780, 850);
        ImVec2 correlation_size = ImVec2(400, 150);
        
        bool positions_set = false;
        bool need_save = false;  // Flag to indicate positions should be saved
    } window_positions_;
};

} // namespace sep::workbench
