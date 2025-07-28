#include "quantum_signal_bridge.hpp"
#include "quantum/qbsa_qfh.h"
#include <algorithm>
#include <cmath>
#include <random>
#include <iostream>

namespace sep::trading {

QuantumSignalBridge::QuantumSignalBridge(const QuantumSignalConfig& config)
    : config_(config) {
}

bool QuantumSignalBridge::initialize() {
    try {
        // Initialize QFH processor with appropriate thresholds
        sep::quantum::QFHOptions qfh_options;
        qfh_options.collapse_threshold = 0.3f;  // 30% rupture ratio indicates collapse
        qfh_options.flip_threshold = 0.7f;      // 70% flip ratio indicates instability
        
        qfh_processor_ = std::make_unique<sep::quantum::QFHBasedProcessor>(qfh_options);
        
        // Initialize QBSA processor with quantum collapse detection
        sep::quantum::QBSAOptions qbsa_options;
        qbsa_options.collapse_threshold = config_.confidence_threshold;
        
        qbsa_processor_ = sep::quantum::createQFHBasedQBSAProcessor(qbsa_options);
        
        if (!qfh_processor_ || !qbsa_processor_) {
            std::cerr << "[QuantumBridge] Failed to initialize quantum processors" << std::endl;
            return false;
        }
        
        std::cout << "[QuantumBridge] Initialized successfully" << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "[QuantumBridge] Initialization error: " << e.what() << std::endl;
        return false;
    }
}

void QuantumSignalBridge::shutdown() {
    qfh_processor_.reset();
    qbsa_processor_.reset();
}

void QuantumSignalBridge::setConfidenceThreshold(float threshold) {
    std::lock_guard<std::mutex> lock(config_mutex_);
    config_.confidence_threshold = threshold;
}

void QuantumSignalBridge::setCoherenceThreshold(float threshold) {
    std::lock_guard<std::mutex> lock(config_mutex_);
    config_.coherence_threshold = threshold;
}

void QuantumSignalBridge::setStabilityThreshold(float threshold) {
    std::lock_guard<std::mutex> lock(config_mutex_);
    config_.stability_threshold = threshold;
}

const QuantumSignalConfig& QuantumSignalBridge::getConfig() const {
    return config_;
}

QuantumTradingSignal QuantumSignalBridge::analyzeMarketData(
    const sep::connectors::MarketData& current_data,
    const std::vector<sep::connectors::MarketData>& history) {
    
    QuantumTradingSignal signal;
    signal.instrument = current_data.instrument;
    
    try {
        // Validate minimum history requirement
        if (!hasMinimumHistory(history)) {
            signal.confidence = 0.0f;
            signal.should_execute = false;
            return signal;
        }
        
        // Convert price data to bit patterns for quantum analysis
        auto bits = convertPriceToBits(history);
        if (bits.empty()) {
            signal.confidence = 0.0f;
            signal.should_execute = false;
            return signal;
        }
        
        // Run QFH analysis on bit patterns
        auto qfh_result = qfh_processor_->analyze(bits);
        
        // Generate probe indices and expectations for QBSA
        auto probe_indices = generateProbeIndices(config_.pattern_length);
        auto expectations = generateExpectations(bits);
        
        // Run QBSA analysis
        auto qbsa_result = qbsa_processor_->analyze(probe_indices, expectations);
        
        // Build quantum trading signal
        signal.confidence = qbsa_result.correction_ratio;
        signal.coherence = qfh_result.coherence;
        signal.stability = calculateStability(history);
        signal.entropy = qfh_result.entropy;
        signal.rupture_ratio = qfh_result.rupture_ratio;
        signal.flip_ratio = qfh_result.flip_ratio;
        signal.quantum_collapse_detected = qbsa_result.collapse_detected || qfh_result.collapse_detected;
        
        // Determine trading action based on quantum analysis
        signal.action = determineAction(qfh_result, qbsa_result, history);
        
        // Apply strategy thresholds to determine execution
        std::lock_guard<std::mutex> lock(config_mutex_);
        signal.should_execute = (signal.confidence >= config_.confidence_threshold &&
                                signal.coherence >= config_.coherence_threshold &&
                                signal.stability >= config_.stability_threshold &&
                                signal.action != QuantumTradingAction::HOLD);
        
        // Calculate trading parameters if signal should execute
        if (signal.should_execute) {
            signal.suggested_position_size = calculatePositionSize(signal.confidence, 10000.0); // Default account balance
            signal.stop_loss_distance = calculateStopLoss(current_data.mid, current_data.atr, signal.action);
            signal.take_profit_distance = calculateTakeProfit(current_data.mid, current_data.atr, signal.action);
        }
        
        // Validate final signal
        if (!validateSignal(signal)) {
            signal.should_execute = false;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "[QuantumBridge] Analysis error: " << e.what() << std::endl;
        signal.confidence = 0.0f;
        signal.should_execute = false;
    }
    
    return signal;
}

std::vector<uint8_t> QuantumSignalBridge::convertPriceToBits(
    const std::vector<sep::connectors::MarketData>& history) const {
    
    if (history.size() < 2) {
        return {};
    }
    
    std::vector<uint8_t> bits;
    bits.reserve(history.size() - 1);
    
    // Convert price movements to binary: 1 if price goes up, 0 if down
    for (size_t i = 1; i < history.size(); ++i) {
        double prev_price = history[i-1].mid;
        double curr_price = history[i].mid;
        
        if (curr_price > prev_price) {
            bits.push_back(1);
        } else {
            bits.push_back(0);
        }
    }
    
    return bits;
}

std::vector<uint32_t> QuantumSignalBridge::generateProbeIndices(size_t pattern_length) const {
    std::vector<uint32_t> indices;
    
    // Generate systematic probe indices for pattern analysis
    for (uint32_t i = 0; i < pattern_length; ++i) {
        indices.push_back(i);
    }
    
    // Add some random probe indices for enhanced detection
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint32_t> dist(0, pattern_length - 1);
    
    for (int i = 0; i < 5; ++i) {
        indices.push_back(dist(gen));
    }
    
    return indices;
}

std::vector<uint32_t> QuantumSignalBridge::generateExpectations(
    const std::vector<uint8_t>& bits) const {
    
    std::vector<uint32_t> expectations;
    
    if (bits.empty()) {
        return expectations;
    }
    
    // Generate expectations based on recent bit patterns
    size_t pattern_size = std::min(bits.size(), config_.pattern_length);
    
    for (size_t i = 0; i < pattern_size; ++i) {
        // Simple expectation: expect the most recent pattern to continue
        expectations.push_back(static_cast<uint32_t>(bits[bits.size() - 1 - i]));
    }
    
    return expectations;
}

QuantumTradingAction QuantumSignalBridge::determineAction(
    const sep::quantum::QFHResult& qfh_result,
    const sep::quantum::QBSAResult& qbsa_result,
    const std::vector<sep::connectors::MarketData>& history) const {
    
    // Don't trade if quantum collapse is detected (too much uncertainty)
    if (qbsa_result.collapse_detected || qfh_result.collapse_detected) {
        return QuantumTradingAction::HOLD;
    }
    
    // Analyze recent price trend
    if (history.size() < 3) {
        return QuantumTradingAction::HOLD;
    }
    
    double recent_trend = 0.0;
    for (size_t i = history.size() - 3; i < history.size() - 1; ++i) {
        recent_trend += (history[i+1].mid - history[i].mid);
    }
    
    // Use QFH rupture/flip ratios to determine directional bias
    float directional_bias = qfh_result.flip_ratio - qfh_result.rupture_ratio;
    
    // Combine trend and quantum bias
    if (recent_trend > 0 && directional_bias > 0.1f) {
        return QuantumTradingAction::BUY;
    } else if (recent_trend < 0 && directional_bias < -0.1f) {
        return QuantumTradingAction::SELL;
    }
    
    return QuantumTradingAction::HOLD;
}

float QuantumSignalBridge::calculateStability(
    const std::vector<sep::connectors::MarketData>& history) const {
    
    if (history.size() < 5) {
        return 0.0f;
    }
    
    // Calculate price volatility over recent history
    double mean_price = 0.0;
    size_t window = std::min(history.size(), static_cast<size_t>(10));
    
    for (size_t i = history.size() - window; i < history.size(); ++i) {
        mean_price += history[i].mid;
    }
    mean_price /= window;
    
    double variance = 0.0;
    for (size_t i = history.size() - window; i < history.size(); ++i) {
        double diff = history[i].mid - mean_price;
        variance += diff * diff;
    }
    variance /= window;
    
    double std_dev = std::sqrt(variance);
    
    // Stability is inverse of relative volatility
    double relative_volatility = std_dev / mean_price;
    return static_cast<float>(1.0 / (1.0 + relative_volatility * 100.0));
}

double QuantumSignalBridge::calculatePositionSize(float confidence, double account_balance) const {
    std::lock_guard<std::mutex> lock(config_mutex_);
    
    // Base position size on account balance and risk ratio
    double base_size = account_balance * config_.max_position_ratio;
    
    // Scale by confidence level
    double confidence_multiplier = std::min(confidence / config_.confidence_threshold, 2.0f);
    
    return base_size * confidence_multiplier;
}

double QuantumSignalBridge::calculateStopLoss(double current_price, double atr, QuantumTradingAction action) const {
    std::lock_guard<std::mutex> lock(config_mutex_);
    
    double stop_distance = atr * config_.stop_loss_atr_multiplier;
    
    if (action == QuantumTradingAction::BUY) {
        return stop_distance; // Distance below current price
    } else if (action == QuantumTradingAction::SELL) {
        return stop_distance; // Distance above current price
    }
    
    return 0.0;
}

double QuantumSignalBridge::calculateTakeProfit(double current_price, double atr, QuantumTradingAction action) const {
    std::lock_guard<std::mutex> lock(config_mutex_);
    
    double profit_distance = atr * config_.take_profit_atr_multiplier;
    
    if (action == QuantumTradingAction::BUY) {
        return profit_distance; // Distance above current price
    } else if (action == QuantumTradingAction::SELL) {
        return profit_distance; // Distance below current price
    }
    
    return 0.0;
}

bool QuantumSignalBridge::validateSignal(const QuantumTradingSignal& signal) const {
    // Basic validation checks
    if (signal.confidence < 0.0f || signal.confidence > 1.0f) {
        return false;
    }
    
    if (signal.coherence < 0.0f || signal.coherence > 1.0f) {
        return false;
    }
    
    if (signal.should_execute && signal.suggested_position_size <= 0.0) {
        return false;
    }
    
    return true;
}

bool QuantumSignalBridge::hasMinimumHistory(const std::vector<sep::connectors::MarketData>& history) const {
    return history.size() >= config_.min_history_size;
}

void QuantumSignalBridge::evolvePatternsWithFeedback(const std::string& pattern_id, bool profitable) {
    // Future implementation: Apply evolutionary feedback to patterns
    // This would integrate with the pattern evolution system mentioned in the report
    std::cout << "[QuantumBridge] Pattern feedback: " << pattern_id 
              << (profitable ? " profitable" : " unprofitable") << std::endl;
}

} // namespace sep::trading
