#pragma once

#include <vector>
#include <memory>
#include <deque>
#include <atomic>
#include <mutex>

#include "connectors/oanda_connector.h"
#include "quantum/qfh.h"
#include "quantum/qbsa.h"

namespace sep::trading {

/**
 * Trading signal generated from quantum analysis
 * Based on QFH/QBSA patent-backed algorithms
 */
struct QuantumTradingSignal {
    enum Action { HOLD, BUY, SELL };
    
    std::string instrument;
    Action action = HOLD;
    bool should_execute = false;
    
    // Quantum metrics (from patent analysis)
    float confidence = 0.0f;        // QBSA correction_ratio
    float coherence = 0.0f;         // Calculated from QFH
    float stability = 0.0f;         // Pattern stability
    float entropy = 0.0f;           // QFH entropy
    
    // QFH specific metrics
    float flip_ratio = 0.0f;
    float rupture_ratio = 0.0f;
    bool quantum_collapse_detected = false;
    
    // Trading parameters
    double suggested_position_size = 0.0;
    double stop_loss_distance = 0.0;
    double take_profit_distance = 0.0;
    
    // Timing
    uint64_t timestamp = 0;
};

/**
 * Bridge between quantum engine and trading execution
 * Implements patent-backed QFH/QBSA analysis for signal generation
 */
class QuantumSignalBridge {
public:
    QuantumSignalBridge();
    ~QuantumSignalBridge();
    
    bool initialize();
    void shutdown();
    
    // Main analysis function - converts market data to trading signals
    QuantumTradingSignal analyzeMarketData(
        const sep::connectors::MarketData& current_data,
        const std::vector<sep::connectors::MarketData>& history
    );
    
    // Strategy threshold configuration (from alpha analysis)
    void setConfidenceThreshold(float threshold) { confidence_threshold_ = threshold; }
    void setCoherenceThreshold(float threshold) { coherence_threshold_ = threshold; }
    void setStabilityThreshold(float threshold) { stability_threshold_ = threshold; }
    
    // Pattern evolution feedback
    void evolvePatternsWithFeedback(const std::string& pattern_id, bool profitable);
    
    // Risk management
    double calculatePositionSize(float confidence, double account_balance);
    
    // Diagnostics
    const std::vector<uint8_t>& getLastBitPattern() const { return last_bits_; }
    const sep::quantum::QFHResult& getLastQFHResult() const { return last_qfh_result_; }
    const sep::quantum::QBSAResult& getLastQBSAResult() const { return last_qbsa_result_; }

private:
    // Quantum processors (patent-backed)
    std::unique_ptr<sep::quantum::QFHBasedProcessor> qfh_processor_;
    std::unique_ptr<sep::quantum::QBSAProcessor> qbsa_processor_;
    
    // Strategy thresholds (based on POC results showing coherence ~0.47)
    std::atomic<float> confidence_threshold_{0.6f};
    std::atomic<float> coherence_threshold_{0.4f};  // Realistic threshold based on POC data
    std::atomic<float> stability_threshold_{0.0f};
    
    // Data processing
    std::vector<uint8_t> convertPriceToBits(const std::vector<sep::connectors::MarketData>& history);
    float calculateConfidence(const sep::quantum::QFHResult& qfh_result, const sep::quantum::QBSAResult& qbsa_result);
    float calculateCoherence(const sep::quantum::QFHResult& qfh_result);
    float calculateStability(const std::vector<sep::connectors::MarketData>& history);
    QuantumTradingSignal::Action determineDirection(
        const sep::quantum::QFHResult& qfh,
        const sep::quantum::QBSAResult& qbsa
    );
    
    // Risk management
    double calculateStopLoss(float coherence, double current_price);
    double calculateTakeProfit(float confidence, double current_price);
    
    // Pattern management
    void loadPatterns();
    void savePatterns();
    std::string generatePatternId(const std::string& instrument, uint64_t timestamp);
    
    // Debug and diagnostics
    void debugDataFormat(const std::vector<sep::connectors::MarketData>& history);
    std::vector<uint8_t> last_bits_;
    sep::quantum::QFHResult last_qfh_result_;
    sep::quantum::QBSAResult last_qbsa_result_;
    
    // Thread safety
    mutable std::mutex analysis_mutex_;
    
    // Patterns storage (simplified for now)
    std::map<std::string, float> active_pattern_scores_;
    std::string patterns_file_path_;
    
    bool initialized_ = false;
};

} // namespace sep::trading
