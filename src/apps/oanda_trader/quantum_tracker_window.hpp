#pragma once

#include <deque>
#include <memory>
#include <mutex>
#include <string>
#include <vector>
#include <chrono>
#include <cmath>
#include <optional>

#include "connectors/oanda_connector.h"
#include "quantum_signal_bridge.hpp"

namespace sep::apps {

// Prediction tracking structure
struct QuantumPrediction {
    std::chrono::steady_clock::time_point timestamp;
    std::string instrument;
    sep::trading::QuantumTradingSignal::Action predicted_direction;
    double prediction_price;
    double confidence;
    double coherence;
    double stability;
    
    // Actual outcome tracking
    bool resolved{false};
    bool correct{false};
    double actual_price_after_period{0.0};
    std::chrono::seconds evaluation_period{60}; // 1 minute default
};

// Pips tracking for 48-hour window (from GUI.md)
struct PipsTracker {
    std::deque<double> pip_history_;    // 48h of pip changes
    std::deque<double> price_history_;  // 48h of prices
    double total_pips_48h_{0.0};
    double current_price_{0.0};
    double start_price_48h_{0.0};
    
    void updatePips(double new_price) {
        if (!std::isfinite(new_price)) {
            return; // Basic data-quality check
        }

        if (!price_history_.empty()) {
            double pip_change = (new_price - current_price_) * 10000; // Convert to pips
            pip_history_.push_back(pip_change);

            // Maintain 48h window (assuming 1-minute data = 2880 points)
            if (pip_history_.size() > 2880) {
                pip_history_.pop_front();
                price_history_.pop_front();
            }
        }

        price_history_.push_back(new_price);
        current_price_ = new_price;

        // Calculate 48h total
        if (!price_history_.empty()) {
            start_price_48h_ = price_history_.front();
            total_pips_48h_ = (current_price_ - start_price_48h_) * 10000;
        }
    }

    double calculateSharpeRatio() const {
        if (pip_history_.size() < 2) return 0.0;
        double mean = 0.0;
        for (double p : pip_history_) mean += p;
        mean /= static_cast<double>(pip_history_.size());

        double var = 0.0;
        for (double p : pip_history_) {
            double diff = p - mean;
            var += diff * diff;
        }
        var /= static_cast<double>(pip_history_.size());
        double stddev = std::sqrt(var);
        if (stddev < 1e-6) return 0.0;
        return (mean / stddev) * std::sqrt(1440.0); // Approx. minutes per day
    }

    double calculateMaxDrawdown() const {
        if (price_history_.empty()) return 0.0;
        double peak = price_history_.front();
        double max_dd = 0.0;
        for (double p : price_history_) {
            peak = std::max(peak, p);
            double dd = (p - peak) / peak;
            if (dd < max_dd) max_dd = dd;
        }
        return max_dd;
    }
};

// Live quantum signal tracking stats
struct QuantumTrackingStats {
    int total_predictions{0};
    int correct_predictions{0};
    int incorrect_predictions{0};
    int pending_predictions{0};
    
    double accuracy_percentage{0.0};
    double average_confidence{0.0};
    double average_coherence{0.0};
    double average_stability{0.0};
    
    // Recent performance windows
    double last_hour_accuracy{0.0};
    double last_24h_accuracy{0.0};
    double overall_accuracy{0.0};
    
    // Confidence buckets
    int high_confidence_correct{0};
    int high_confidence_total{0};
    int medium_confidence_correct{0};
    int medium_confidence_total{0};
    int low_confidence_correct{0};
    int low_confidence_total{0};
    
    // Live trading performance
    double total_pnl{0.0};
    int trades_executed{0};
    int winning_trades{0};
    int losing_trades{0};
    double win_rate{0.0};
    double max_drawdown{0.0};
    double current_drawdown{0.0};
    double peak_equity{0.0};
};

class QuantumTrackerWindow {
public:
    QuantumTrackerWindow();
    ~QuantumTrackerWindow() = default;

    // Initialize quantum tracking
    bool initialize();
    void shutdown();

    // Main tracking interface
    void processNewMarketData(const sep::connectors::MarketData& data);
    void processNewMarketData(const sep::connectors::MarketData& data, 
                             const std::string& historical_timestamp);
    void render();

    // Statistics and performance
    const QuantumTrackingStats& getStats() const { return stats_; }
    void resetStats();
    
    // Bridge access for initialization
    sep::trading::QuantumSignalBridge* getQuantumBridge() const { return quantum_bridge_.get(); }
    
    // Latest signal access for trading decisions
    const sep::trading::QuantumTradingSignal& getLatestSignal() const { return latest_signal_; }
    bool hasLatestSignal() const { return has_latest_signal_; }

private:
    // Quantum signal processing
    std::unique_ptr<sep::trading::QuantumSignalBridge> quantum_bridge_;
    std::deque<sep::connectors::MarketData> market_history_;
    std::mutex data_mutex_;
    
    // Prediction tracking
    std::vector<QuantumPrediction> predictions_;
    std::mutex predictions_mutex_;
    QuantumTrackingStats stats_;
    
    // Latest signal for display
    sep::trading::QuantumTradingSignal latest_signal_;
    bool has_latest_signal_{false};
    
    // Metric history for plotting
    std::deque<float> confidence_history_;
    std::deque<float> coherence_history_;
    std::deque<float> stability_history_;
    std::deque<float> price_history_plot_;
    std::deque<double> timestamp_history_;
    static constexpr size_t MAX_PLOT_POINTS = 1440;  // 24 hours at 1 minute intervals
    
    // Configuration
    static constexpr size_t MAX_HISTORY_SIZE = 1500;  // Support 24+ hours of data
    static constexpr size_t MIN_HISTORY_FOR_SIGNAL = 20;
    static constexpr double HIGH_CONFIDENCE_THRESHOLD = 0.8;
    static constexpr double MEDIUM_CONFIDENCE_THRESHOLD = 0.6;

    // Runtime-adjustable thresholds
    float conf_threshold_{0.6f};
    float coh_threshold_{0.4f};
    float stab_threshold_{0.0f};
    
    // Internal methods
    void updatePredictions(const sep::connectors::MarketData& current_data);
    void makePrediction(const sep::trading::QuantumTradingSignal& signal, 
                       const sep::connectors::MarketData& current_data,
                       std::optional<std::chrono::steady_clock::time_point> historical_time = std::nullopt);
    void evaluatePendingPredictions(const sep::connectors::MarketData& current_data);
    void updateStatistics();
    
    // Pips tracker (from GUI.md)
    PipsTracker pips_tracker_;
    
    // UI rendering helpers
    void renderPredictionStats();
    void renderLatestSignal();
    void renderAccuracyMetrics();
    void renderConfidenceBuckets();
    void renderRecentPredictions();
    void renderMultiTimeframeConfirmation();
    void renderLiveTradingPerformance();
    
    // New GUI.md requirements
    void renderPipsDisplay();
    void renderQuantumDiagnostics();
    void renderMetricPlots();
    void renderThresholdControls();
    
    // Utility functions
    double calculateDirectionalAccuracy(const QuantumPrediction& pred, double actual_price) const;
    std::string formatDuration(std::chrono::steady_clock::time_point start) const;
    const char* actionToString(sep::trading::QuantumTradingSignal::Action action) const;
};

} // namespace sep::apps
