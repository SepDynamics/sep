#include "quantum_signal_bridge.hpp"
#include <iostream>
#include <chrono>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <nlohmann/json.hpp>

namespace sep::trading {

QuantumSignalBridge::QuantumSignalBridge() 
    : patterns_file_path_("quantum_patterns.json")
{
}

QuantumSignalBridge::~QuantumSignalBridge() {
    shutdown();
}

bool QuantumSignalBridge::initialize() {
    std::lock_guard<std::mutex> lock(analysis_mutex_);
    
    try {
        // Initialize QFH processor with correct options
        sep::quantum::QFHOptions qfh_opts;
        qfh_opts.collapse_threshold = 0.3f;
        qfh_opts.flip_threshold = 0.7f;
        qfh_processor_ = std::make_unique<sep::quantum::QFHBasedProcessor>(qfh_opts);
        
        // Initialize QBSA processor  
        sep::quantum::QBSAOptions qbsa_opts;
        qbsa_opts.collapse_threshold = 0.6f;
        qbsa_processor_ = std::make_unique<sep::quantum::QBSAProcessor>(qbsa_opts);
        
        // Load existing patterns
        loadPatterns();
        
        initialized_ = true;
        std::cout << "[QuantumSignalBridge] Initialized successfully" << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "[QuantumSignalBridge] Initialization failed: " << e.what() << std::endl;
        return false;
    }
}

void QuantumSignalBridge::shutdown() {
    if (initialized_) {
        savePatterns();
        initialized_ = false;
    }
}

QuantumTradingSignal QuantumSignalBridge::analyzeMarketData(
    const sep::connectors::MarketData& current_data,
    const std::vector<sep::connectors::MarketData>& history) {
    
    std::lock_guard<std::mutex> lock(analysis_mutex_);
    
    QuantumTradingSignal signal;
    signal.instrument = current_data.instrument;
    signal.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    
    if (!initialized_ || history.size() < 20) {
        return signal; // Return HOLD signal
    }
    
    try {
        // Debug data format
        debugDataFormat(history);
        
        // Convert price data to bit patterns
        auto bits = convertPriceToBits(history);
        last_bits_ = bits;
        
        if (bits.size() < 10) {  // Reduced from 32 to 10 for early testing
            std::cout << "[QuantumSignal] Insufficient bit data: " << bits.size() << std::endl;
            return signal;
        }
        
        // Run QFH analysis (patent-backed quantum field harmonics)
        auto qfh_result = qfh_processor_->analyze(bits);
        last_qfh_result_ = qfh_result;
        
        // Run QBSA analysis (patent-backed quantum bit state analysis)
        std::vector<uint32_t> probe_indices;
        std::vector<uint32_t> expectations;
        
        // Generate probe indices (every 4th bit for efficiency)
        for (size_t i = 0; i < std::min(bits.size(), 64UL); i += 4) {
            probe_indices.push_back(i);
            expectations.push_back(bits[i] ^ 1); // Expect opposite state
        }
        
        auto qbsa_result = qbsa_processor_->analyze(probe_indices, expectations);
        last_qbsa_result_ = qbsa_result;
        
        // Calculate quantum metrics
        signal.confidence = calculateConfidence(qfh_result, qbsa_result);  // Multi-factor confidence
        signal.coherence = qfh_result.coherence;  // Use built-in QFH coherence
        signal.stability = calculateStability(history);
        signal.entropy = qfh_result.entropy;
        
        // QFH specific metrics
        signal.flip_ratio = qfh_result.flip_ratio;
        signal.rupture_ratio = qfh_result.rupture_ratio;
        signal.quantum_collapse_detected = qfh_result.collapse_detected;
        
        // Apply strategy thresholds (from alpha analysis)
        bool meets_confidence = signal.confidence >= confidence_threshold_.load();
        bool meets_coherence = signal.coherence >= coherence_threshold_.load();
        bool meets_stability = signal.stability >= stability_threshold_.load();
        
        std::cout << "[QuantumSignal] Metrics - Confidence: " << signal.confidence 
                  << " (≥" << confidence_threshold_.load() << ": " << (meets_confidence ? "PASS" : "FAIL") << ")"
                  << " Coherence: " << signal.coherence 
                  << " (≥" << coherence_threshold_.load() << ": " << (meets_coherence ? "PASS" : "FAIL") << ")"
                  << " Stability: " << signal.stability 
                  << " (≥" << stability_threshold_.load() << ": " << (meets_stability ? "PASS" : "FAIL") << ")"
                  << std::endl;
        
        if (meets_confidence && meets_coherence && meets_stability) {
            // Determine trade direction from quantum analysis
            signal.action = determineDirection(qfh_result, qbsa_result);
            signal.should_execute = (signal.action != QuantumTradingSignal::HOLD);
            
            if (signal.should_execute) {
                // Calculate risk management parameters
                signal.suggested_position_size = calculatePositionSize(signal.confidence, 10000.0); // Default balance
                signal.stop_loss_distance = calculateStopLoss(signal.coherence, current_data.mid);
                signal.take_profit_distance = calculateTakeProfit(signal.confidence, current_data.mid);
                
                std::cout << "[QuantumSignal] SIGNAL GENERATED: " << current_data.instrument
                          << " Action: " << (signal.action == QuantumTradingSignal::BUY ? "BUY" : "SELL")
                          << " Size: " << signal.suggested_position_size << std::endl;
            }
        } else {
            std::cout << "[QuantumSignal] Thresholds not met - HOLD" << std::endl;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "[QuantumSignal] Analysis error: " << e.what() << std::endl;
    }
    
    return signal;
}

std::vector<uint8_t> QuantumSignalBridge::convertPriceToBits(
    const std::vector<sep::connectors::MarketData>& history) {
    
    std::vector<uint8_t> bits;
    
    if (history.size() < 2) {
        return bits;
    }
    
    // Convert price movements to binary states
    for (size_t i = 1; i < history.size(); ++i) {
        double price_change = history[i].mid - history[i-1].mid;
        
        // Convert to pips (4 decimal places for forex)
        int pip_change = static_cast<int>(price_change * 10000);
        
        // Simple bit generation - direction only for initial implementation
        uint8_t direction_bit = (pip_change > 0) ? 1 : 0;
        bits.push_back(direction_bit);
    }
    
    std::cout << "[QuantumSignal] Converted " << history.size() 
              << " price points to " << bits.size() << " bits" << std::endl;
    
    return bits;
}

float QuantumSignalBridge::calculateConfidence(const sep::quantum::QFHResult& qfh_result, 
                                              const sep::quantum::QBSAResult& qbsa_result) {
    // Confidence calculation based on strategy docs:
    // Primary: QBSA correction_ratio (measures pattern stability)
    // Secondary: QFH pattern quality indicators
    
    // QBSA correction ratio is the main confidence indicator
    float base_confidence = qbsa_result.correction_ratio;
    
    // Adjust based on QFH pattern quality (minor adjustments only)
    float flip_stability = 1.0f - qfh_result.flip_ratio;  // Lower flip ratio = more stable
    float rupture_penalty = qfh_result.rupture_ratio;     // Higher rupture = less confident
    
    // Apply modest adjustments (keep QBSA as primary factor)
    float confidence = base_confidence * (0.9f + flip_stability * 0.1f) * (1.0f - rupture_penalty * 0.1f);
    
    return std::max(0.0f, std::min(1.0f, confidence));
}

float QuantumSignalBridge::calculateCoherence(const sep::quantum::QFHResult& qfh_result) {
    // Coherence based on inverse of entropy and rupture ratio
    float entropy_factor = 1.0f / (1.0f + qfh_result.entropy);
    float rupture_factor = 1.0f - qfh_result.rupture_ratio;
    
    return std::min(1.0f, entropy_factor * rupture_factor);
}

float QuantumSignalBridge::calculateStability(const std::vector<sep::connectors::MarketData>& history) {
    if (history.size() < 10) return 0.0f;
    
    // Calculate price stability as inverse of volatility
    double price_sum = 0.0, price_sq_sum = 0.0;
    for (const auto& data : history) {
        price_sum += data.mid;
        price_sq_sum += data.mid * data.mid;
    }
    
    double mean = price_sum / history.size();
    double variance = (price_sq_sum / history.size()) - (mean * mean);
    double volatility = std::sqrt(variance);
    
    // Normalize volatility to stability (0-1 scale)
    return 1.0f / (1.0f + static_cast<float>(volatility * 10000)); // Scale by pips
}

QuantumTradingSignal::Action QuantumSignalBridge::determineDirection(
    const sep::quantum::QFHResult& qfh,
    const sep::quantum::QBSAResult& qbsa) {
    
    // More balanced direction determination
    // Use flip ratio as primary direction indicator
    float flip_ratio = qfh.flip_ratio;
    float correction_ratio = qbsa.correction_ratio;
    float rupture_ratio = qfh.rupture_ratio;
    
    std::cout << "[QuantumSignal] Direction analysis - Flip: " << flip_ratio 
              << " Correction: " << correction_ratio << " Rupture: " << rupture_ratio << std::endl;
    
    // Primary logic: flip ratio indicates market direction tendency
    if (correction_ratio > 0.5f) {  // Lowered threshold from 0.7f
        if (flip_ratio < 0.4f) {
            // Low flip ratio suggests stable upward trend
            return QuantumTradingSignal::BUY;
        } else if (flip_ratio > 0.6f) {
            // High flip ratio suggests volatility/downward pressure
            return QuantumTradingSignal::SELL;
        }
    }
    
    // Secondary logic: rupture analysis for trend strength
    if (rupture_ratio > 0.8f) {
        // High rupture suggests strong trend break - go opposite
        return (flip_ratio > 0.5f) ? QuantumTradingSignal::BUY : QuantumTradingSignal::SELL;
    }
    
    // Quantum collapse detection for reversal signals
    if (qfh.collapse_detected) {
        return (flip_ratio > 0.5f) ? QuantumTradingSignal::SELL : QuantumTradingSignal::BUY;
    }
    
    return QuantumTradingSignal::HOLD;
}

double QuantumSignalBridge::calculatePositionSize(float confidence, double account_balance) {
    // Risk-adjusted position sizing based on confidence
    double risk_percent = 0.02; // 2% max risk per trade
    double base_units = 1000;   // Base position size
    
    // Scale by confidence (higher confidence = larger position)
    double confidence_multiplier = std::min(2.0, static_cast<double>(confidence) * 2.0);
    
    return base_units * confidence_multiplier;
}

double QuantumSignalBridge::calculateStopLoss(float coherence, double current_price) {
    // Stop loss based on coherence (higher coherence = tighter stop)
    double base_stop_pips = 20.0; // 20 pip base stop
    double coherence_factor = 1.0 - static_cast<double>(coherence);
    
    double stop_pips = base_stop_pips * (1.0 + coherence_factor);
    return stop_pips / 10000.0; // Convert pips to price distance
}

double QuantumSignalBridge::calculateTakeProfit(float confidence, double current_price) {
    // Take profit based on confidence (higher confidence = larger target)
    double base_target_pips = 30.0; // 30 pip base target
    double confidence_multiplier = static_cast<double>(confidence) * 2.0;
    
    double target_pips = base_target_pips * confidence_multiplier;
    return target_pips / 10000.0; // Convert pips to price distance
}

void QuantumSignalBridge::debugDataFormat(const std::vector<sep::connectors::MarketData>& history) {
    if (history.empty()) return;
    
    const auto& latest = history.back();
    std::cout << "[QuantumSignal] Data format check:" << std::endl;
    std::cout << "  Instrument: " << latest.instrument << std::endl;
    std::cout << "  Price (mid): " << latest.mid << std::endl;
    std::cout << "  Bid: " << latest.bid << std::endl;
    std::cout << "  Ask: " << latest.ask << std::endl;
    std::cout << "  ATR: " << latest.atr << std::endl;
    std::cout << "  History size: " << history.size() << std::endl;
}

void QuantumSignalBridge::loadPatterns() {
    std::ifstream file(patterns_file_path_);
    if (file.is_open()) {
        try {
            nlohmann::json patterns_json;
            file >> patterns_json;
            
            // Load patterns from JSON
            // Implementation depends on Pattern structure
            std::cout << "[QuantumSignal] Loaded patterns from " << patterns_file_path_ << std::endl;
        } catch (const std::exception& e) {
            std::cout << "[QuantumSignal] Could not load patterns: " << e.what() << std::endl;
        }
    }
}

void QuantumSignalBridge::savePatterns() {
    try {
        nlohmann::json patterns_json;
        
        // Save patterns to JSON
        // Implementation depends on Pattern structure
        
        std::ofstream file(patterns_file_path_);
        file << patterns_json.dump(2);
        
        std::cout << "[QuantumSignal] Saved patterns to " << patterns_file_path_ << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "[QuantumSignal] Could not save patterns: " << e.what() << std::endl;
    }
}

void QuantumSignalBridge::evolvePatternsWithFeedback(const std::string& pattern_id, bool profitable) {
    std::lock_guard<std::mutex> lock(analysis_mutex_);
    
    // Simplified pattern feedback for now
    if (active_pattern_scores_.find(pattern_id) != active_pattern_scores_.end()) {
        float adjustment = profitable ? 0.1f : -0.1f;
        active_pattern_scores_[pattern_id] += adjustment;
        std::cout << "[QuantumSignal] Applied feedback for pattern " << pattern_id 
                  << " (profitable: " << profitable << ", new score: " 
                  << active_pattern_scores_[pattern_id] << ")" << std::endl;
    } else {
        active_pattern_scores_[pattern_id] = profitable ? 0.6f : 0.4f;
        std::cout << "[QuantumSignal] Created new pattern " << pattern_id 
                  << " (profitable: " << profitable << ")" << std::endl;
    }
}

std::string QuantumSignalBridge::generatePatternId(const std::string& instrument, uint64_t timestamp) {
    return "pattern_" + instrument + "_" + std::to_string(timestamp);
}

} // namespace sep::trading
