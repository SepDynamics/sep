#pragma once

#include <vector>
#include <string>
#include <memory>
#include <mutex>
#include <cstdint>

#include "connectors/oanda_connector.h"
#include "quantum/qfh.h"
#include "quantum/qbsa.h"
#include "engine/standard_includes.h"

namespace sep::trading {

// Trading signal action
enum class QuantumTradingAction : uint8_t {
    HOLD = 0,
    BUY = 1,
    SELL = 2
};

// Comprehensive quantum trading signal
struct QuantumTradingSignal {
    std::string instrument;
    QuantumTradingAction action{QuantumTradingAction::HOLD};
    float confidence{0.0f};          // QBSA correction_ratio
    float coherence{0.0f};           // QFH coherence metric
    float stability{0.0f};           // Pattern stability
    float entropy{0.0f};             // QFH entropy
    bool should_execute{false};      // Whether to execute trade
    
    // Trading parameters
    double suggested_position_size{0.0};
    double stop_loss_distance{0.0};
    double take_profit_distance{0.0};
    
    // Quantum metrics for analysis
    float rupture_ratio{0.0f};
    float flip_ratio{0.0f};
    bool quantum_collapse_detected{false};
};

// Configuration for quantum signal generation
struct QuantumSignalConfig {
    // Strategy thresholds from alpha analysis
    float confidence_threshold{0.6f};
    float coherence_threshold{0.9f};
    float stability_threshold{0.0f};
    
    // Risk management parameters
    float max_position_ratio{0.02f};     // 2% of account balance max
    float stop_loss_atr_multiplier{2.0f}; // Stop loss = 2 * ATR
    float take_profit_atr_multiplier{3.0f}; // Take profit = 3 * ATR
    
    // Pattern analysis parameters
    size_t min_history_size{20};         // Minimum data points for analysis
    size_t max_history_size{100};        // Maximum data points to keep
    size_t pattern_length{16};           // Bit pattern length for quantum analysis
};

// Bridge between quantum engine and trading execution
class QuantumSignalBridge {
public:
    explicit QuantumSignalBridge(const QuantumSignalConfig& config = {});
    ~QuantumSignalBridge() = default;

    // Initialization
    bool initialize();
    void shutdown();

    // Configuration
    void setConfidenceThreshold(float threshold);
    void setCoherenceThreshold(float threshold);
    void setStabilityThreshold(float threshold);
    const QuantumSignalConfig& getConfig() const;

    // Main analysis entry point
    QuantumTradingSignal analyzeMarketData(
        const sep::connectors::MarketData& current_data,
        const std::vector<sep::connectors::MarketData>& history
    );

    // Pattern evolution feedback (for future implementation)
    void evolvePatternsWithFeedback(const std::string& pattern_id, bool profitable);

    // Position sizing based on quantum metrics
    double calculatePositionSize(float confidence, double account_balance) const;

    // Risk management calculations
    double calculateStopLoss(double current_price, double atr, QuantumTradingAction action) const;
    double calculateTakeProfit(double current_price, double atr, QuantumTradingAction action) const;

private:
    // Configuration
    QuantumSignalConfig config_;
    mutable std::mutex config_mutex_;

    // Quantum processors
    std::unique_ptr<sep::quantum::QFHBasedProcessor> qfh_processor_;
    std::unique_ptr<sep::quantum::QBSAProcessor> qbsa_processor_;

    // Internal analysis methods
    std::vector<uint8_t> convertPriceToBits(const std::vector<sep::connectors::MarketData>& history) const;
    std::vector<uint32_t> generateProbeIndices(size_t pattern_length) const;
    std::vector<uint32_t> generateExpectations(const std::vector<uint8_t>& bits) const;
    
    // Signal generation
    QuantumTradingAction determineAction(
        const sep::quantum::QFHResult& qfh_result,
        const sep::quantum::QBSAResult& qbsa_result,
        const std::vector<sep::connectors::MarketData>& history
    ) const;
    
    float calculateStability(const std::vector<sep::connectors::MarketData>& history) const;
    
    // Validation
    bool validateSignal(const QuantumTradingSignal& signal) const;
    bool hasMinimumHistory(const std::vector<sep::connectors::MarketData>& history) const;
};

} // namespace sep::trading
