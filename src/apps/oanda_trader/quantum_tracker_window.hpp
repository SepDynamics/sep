#pragma once

#include <deque>
#include <memory>
#include <mutex>
#include <string>
#include <vector>
#include <chrono>

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
    
    // Confidence buckets
    int high_confidence_correct{0};
    int high_confidence_total{0};
    int medium_confidence_correct{0};
    int medium_confidence_total{0};
    int low_confidence_correct{0};
    int low_confidence_total{0};
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
    void render();

    // Statistics and performance
    const QuantumTrackingStats& getStats() const { return stats_; }
    void resetStats();

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
    
    // Configuration
    static constexpr size_t MAX_HISTORY_SIZE = 200;
    static constexpr size_t MIN_HISTORY_FOR_SIGNAL = 20;
    static constexpr double HIGH_CONFIDENCE_THRESHOLD = 0.8;
    static constexpr double MEDIUM_CONFIDENCE_THRESHOLD = 0.6;
    
    // Internal methods
    void updatePredictions(const sep::connectors::MarketData& current_data);
    void makePrediction(const sep::trading::QuantumTradingSignal& signal, 
                       const sep::connectors::MarketData& current_data);
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
    
    // New GUI.md requirements
    void renderPipsDisplay();
    void renderQuantumDiagnostics();
    
    // Utility functions
    double calculateDirectionalAccuracy(const QuantumPrediction& pred, double actual_price) const;
    std::string formatDuration(std::chrono::steady_clock::time_point start) const;
    const char* actionToString(sep::trading::QuantumTradingSignal::Action action) const;
};

} // namespace sep::apps
