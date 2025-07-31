#pragma once

#include <vector>
#include <memory>
#include <deque>
#include <atomic>
#include <mutex>

#include "connectors/oanda_connector.h"
#include "quantum/bitspace/qfh.h"
#include "quantum/bitspace/qbsa.h"
#include "quantum/pattern_evolution_bridge.h"
#include "quantum/bitspace/pattern_processor.h"
#include "quantum/types.h"
#include "apps/oanda_trader/cuda_types.cuh"
#include "apps/oanda_trader/forward_window_kernels.cuh"

namespace sep::trading {

/**
 * Converged quantum identifiers for each OANDA package
 * Based on forward window analysis and patent-backed algorithms
 */
struct QuantumIdentifiers {
    // Core identifiers (converged values from forward window)
    float confidence = 0.0f;        // QBSA convergence value
    float coherence = 0.0f;         // QFH convergence value  
    float stability = 0.0f;         // Stability convergence value
    
    // Convergence metadata
    bool converged = false;         // Whether values stabilized
    int iterations = 0;             // Iterations to convergence
    float convergence_threshold = 1e-6f;  // Convergence tolerance
    
    // Raw metrics (for debugging)
    float entropy = 0.0f;
    float flip_ratio = 0.0f;
    float rupture_ratio = 0.0f;
    bool quantum_collapse_detected = false;
};

/**
 * Trading signal generated from quantum identifiers
 * Based on QFH/QBSA patent-backed algorithms
 */
struct QuantumTradingSignal {
    enum Action { HOLD, BUY, SELL };
    
    std::string instrument;
    Action action = HOLD;
    bool should_execute = false;
    
    // Quantum identifiers (converged from forward analysis)
    QuantumIdentifiers identifiers;
    
    // Trading parameters
    double suggested_position_size = 0.0;
    double stop_loss_distance = 0.0;
    double take_profit_distance = 0.0;
    
    // Timing and source data
    uint64_t timestamp = 0;
    uint64_t source_candle_timestamp = 0;  // Original OANDA package timestamp
};

struct ManagedPosition {
    std::string id;
    std::string instrument;
    double units{0.0};
    double entry_price{0.0};
    double stop_loss{0.0};
    double take_profit{0.0};
    uint64_t open_time{0};
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
        const std::vector<sep::connectors::MarketData>& history,
        const std::vector<sep::apps::cuda::ForwardWindowResult>& forward_window_results);

    // Per-candle forward window analysis - calculates converged identifiers
    QuantumIdentifiers calculateConvergedIdentifiers(
        const std::vector<sep::connectors::MarketData>& forward_window,
        size_t window_size = 50
    );
    
    // Strategy threshold configuration (from alpha analysis)
    void setConfidenceThreshold(float threshold) { confidence_threshold_ = threshold; }
    void setCoherenceThreshold(float threshold) { coherence_threshold_ = threshold; }
    void setStabilityThreshold(float threshold) { stability_threshold_ = threshold; }
    
    // Pattern evolution feedback
    void evolvePatternsWithFeedback(const std::string& pattern_id, bool profitable);
    
    // Risk management
    double calculatePositionSize(float confidence, double account_balance);
    void addManagedPosition(const QuantumTradingSignal& signal, double current_price);
    void updatePositions(const sep::connectors::MarketData& data);
    
    // Diagnostics
    const std::vector<uint8_t>& getLastBitPattern() const { return last_bits_; }
    const sep::quantum::QFHResult& getLastQFHResult() const { return last_qfh_result_; }
    const sep::quantum::QBSAResult& getLastQBSAResult() const { return last_qbsa_result_; }

private:
    // Quantum processors (patent-backed)
    std::unique_ptr<sep::quantum::bitspace::PatternProcessor> pattern_processor_;
    
    // Strategy thresholds (dynamically determined from convergence patterns)
    std::atomic<float> confidence_threshold_{0.6f};
    std::atomic<float> coherence_threshold_{0.9f};
    std::atomic<float> stability_threshold_{0.05f};
    
    // Convergence calculation core
    QuantumIdentifiers calculateIdentifiersWithConvergence(
        const std::vector<uint8_t>& forward_bits,
        int max_iterations = 1000,
        float convergence_threshold = 1e-6f
    );
    
    // Data processing (legacy - will be replaced with convergence)
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
    
    // Patterns storage
    std::map<std::string, float> active_pattern_scores_;
    std::unordered_map<std::string, sep::quantum::Pattern> active_patterns_;
    std::unique_ptr<sep::quantum::PatternEvolutionBridge> evolver_;
    std::string patterns_file_path_;

    // Managed positions
    std::vector<ManagedPosition> managed_positions_;
    
    bool initialized_ = false;
};

} // namespace sep::trading
