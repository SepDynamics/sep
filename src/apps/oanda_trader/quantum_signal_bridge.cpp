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
        
        // Initialize pattern evolution bridge
        sep::quantum::PatternEvolutionBridge::Config evo_cfg;
        evolver_ = std::make_unique<sep::quantum::PatternEvolutionBridge>(evo_cfg);
        evolver_->initializeEvolutionState();

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
        evolver_.reset();
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
        
        // Direction determination based on normalized stability [0, 1]:
        // < 0.45 = BUY (low stability favors buying), > 0.55 = SELL (high stability favors selling)
        if (signal.stability < 0.45f) {
            signal.action = QuantumTradingSignal::BUY;
        } else if (signal.stability > 0.55f) {
            signal.action = QuantumTradingSignal::SELL;
        } else {
            // HOLD zone - stability between 0.45-0.55 indicates uncertain market conditions
            signal.action = QuantumTradingSignal::HOLD;
        }
        
        // Apply strategy thresholds (from alpha analysis)
        bool meets_confidence = signal.confidence >= confidence_threshold_.load();
        bool meets_coherence = signal.coherence >= 0.6f;  // Coherence must be above 0.6
        // For normalized stability [0,1], check distance from neutral (0.5)
        bool meets_stability = std::abs(signal.stability - 0.5f) >= stability_threshold_.load();
        
        std::cout << "[QuantumSignal] Metrics - Confidence: " << signal.confidence 
                  << " (≥" << confidence_threshold_.load() << ": " << (meets_confidence ? "PASS" : "FAIL") << ")"
                  << " Coherence: " << signal.coherence 
                  << " (≥" << coherence_threshold_.load() << ": " << (meets_coherence ? "PASS" : "FAIL") << ")"
                  << " Stability: " << signal.stability 
                  << " (|0.5-val|≥" << stability_threshold_.load() << ": " << (meets_stability ? "PASS" : "FAIL") << ")"
                  << " Direction: " << (signal.action == QuantumTradingSignal::BUY ? "BUY" : 
                                       signal.action == QuantumTradingSignal::SELL ? "SELL" : "HOLD")
                  << std::endl;
        
        if (meets_confidence && meets_coherence && meets_stability && signal.action != QuantumTradingSignal::HOLD) {
            signal.should_execute = true;
            
            // Calculate risk management parameters
            signal.suggested_position_size = calculatePositionSize(signal.confidence, 10000.0); // Default balance
            signal.stop_loss_distance = calculateStopLoss(signal.coherence, current_data.mid);
            signal.take_profit_distance = calculateTakeProfit(signal.confidence, current_data.mid);
            
            std::cout << "[QuantumSignal] SIGNAL GENERATED: " << current_data.instrument
                      << " Action: " << (signal.action == QuantumTradingSignal::BUY ? "BUY" : "SELL")
                      << " Size: " << signal.suggested_position_size << std::endl;
        } else {
            signal.action = QuantumTradingSignal::HOLD;
            signal.should_execute = false;
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
    
    // Fix confidence stuck at high values - reduce coherence dominance
    // Since coherence is always ~0.999, it shouldn't drive confidence
    
    // Make confidence more responsive to market volatility and pattern quality
    float confidence = base_confidence * 0.4f +           // Reduce QBSA dominance
                      flip_stability * 0.4f +             // Increase flip influence  
                      (1.0f - rupture_penalty) * 0.3f -   // Invert rupture (low rupture = high confidence)
                      (qfh_result.entropy * 0.1f);        // Subtract entropy as uncertainty
    
    return std::max(0.2f, std::min(0.8f, confidence));    // More reasonable range
}

float QuantumSignalBridge::calculateCoherence(const sep::quantum::QFHResult& qfh_result) {
    // Coherence based on inverse of entropy and rupture ratio
    float entropy_factor = 1.0f / (1.0f + qfh_result.entropy);
    float rupture_factor = 1.0f - qfh_result.rupture_ratio;
    
    return std::min(1.0f, entropy_factor * rupture_factor);
}

float QuantumSignalBridge::calculateStability(const std::vector<sep::connectors::MarketData>& history) {
    if (history.size() < 10) return 0.0f;
    
    // Calculate directional stability based on recent price trend
    // Use a stable window from recent data instead of the full shifting history
    size_t window_size = std::min(20UL, history.size());  // Use last 20 points or available data
    size_t start_idx = history.size() - window_size;
    
    double price_start = history[start_idx].mid;
    double price_end = history.back().mid;
    double price_change = (price_end - price_start) * 10000; // Convert to pips
    
    // DEBUG: Track stability calculation details
    static int debug_count = 0;
    if (debug_count++ < 10) {
        std::cout << "[QuantumSignal] STABILITY DEBUG #" << debug_count 
                  << " - History size: " << history.size() << " Window: " << window_size
                  << " Start[" << start_idx << "]: " << price_start << " End: " << price_end 
                  << " Change: " << price_change << " pips" << std::endl;
    }
    
    // Calculate volatility for normalization
    double price_sum = 0.0, price_sq_sum = 0.0;
    for (const auto& data : history) {
        price_sum += data.mid;
        price_sq_sum += data.mid * data.mid;
    }
    
    double mean = price_sum / history.size();
    double variance = (price_sq_sum / history.size()) - (mean * mean);
    double volatility = std::sqrt(variance) * 10000; // Scale to pips
    
    // Stability = directional change scaled by inverse volatility
    // High volatility reduces the magnitude of stability
    float stability_factor = 1.0f / (1.0f + static_cast<float>(volatility));
    float directional_stability = static_cast<float>(price_change) * stability_factor;
    
    // Clamp to reasonable range [-5.0, 5.0] then normalize to [0, 1]
    float clamped_stability = std::max(-5.0f, std::min(5.0f, directional_stability));
    
    // Normalize to [0, 1] range where:
    // 0.0 = maximum bearish (-5.0)
    // 0.5 = neutral (0.0) 
    // 1.0 = maximum bullish (+5.0)
    return (clamped_stability + 5.0f) / 10.0f;
}

QuantumTradingSignal::Action QuantumSignalBridge::determineDirection(
    const sep::quantum::QFHResult& qfh,
    const sep::quantum::QBSAResult& qbsa) {
    
    // Direction determination based on test data analysis
    // Stability is the primary indicator: positive = BUY, negative = SELL
    
    // Get the latest stability from the signal (calculated externally)
    // Note: We'll use this through the main analysis function where stability is calculated
    
    std::cout << "[QuantumSignal] Direction analysis - Flip: " << qfh.flip_ratio 
              << " Correction: " << qbsa.correction_ratio << " Rupture: " << qfh.rupture_ratio << std::endl;
    
    // For now, use QFH metrics as directional indicators until we can access stability here
    // This is a simplified approach - the main logic should be in analyzeMarketData
    
    // Strong coherence patterns with decent confidence
    if (qfh.coherence > 0.3f && qbsa.correction_ratio > 0.3f) {
        // Use rupture ratio as primary direction indicator
        // Low rupture = stable conditions = BUY tendency  
        // High rupture = unstable conditions = SELL tendency
        if (qfh.rupture_ratio < 0.3f) {
            return QuantumTradingSignal::BUY;
        } else if (qfh.rupture_ratio > 0.4f) {
            return QuantumTradingSignal::SELL;
        }
    }
    
    // Secondary: Flip ratio analysis
    if (qbsa.correction_ratio > 0.5f) {
        if (qfh.flip_ratio < 0.4f) {
            // Low flip ratio suggests stable upward trend
            return QuantumTradingSignal::BUY;
        } else if (qfh.flip_ratio > 0.6f) {
            // High flip ratio suggests volatility/downward pressure
            return QuantumTradingSignal::SELL;
        }
    }
    
    // Quantum collapse detection for reversal signals
    if (qfh.collapse_detected) {
        return (qfh.flip_ratio > 0.5f) ? QuantumTradingSignal::SELL : QuantumTradingSignal::BUY;
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
    if (!file.is_open()) {
        return;
    }

    try {
        nlohmann::json patterns_json;
        file >> patterns_json;
        if (patterns_json.is_array()) {
            active_patterns_.clear();
            for (const auto& pj : patterns_json) {
                sep::quantum::Pattern p = pj.get<sep::quantum::Pattern>();
                active_patterns_[p.id] = p;
                active_pattern_scores_[p.id] = p.quantum_state.stability;
            }
        }
        std::cout << "[QuantumSignal] Loaded patterns from " << patterns_file_path_ << std::endl;
    } catch (const std::exception& e) {
        std::cout << "[QuantumSignal] Could not load patterns: " << e.what() << std::endl;
    }
}

void QuantumSignalBridge::savePatterns() {
    try {
        nlohmann::json patterns_json = nlohmann::json::array();
        for (const auto& kv : active_patterns_) {
            patterns_json.push_back(kv.second);
        }
        std::ofstream file(patterns_file_path_);
        file << patterns_json.dump(2);
        std::cout << "[QuantumSignal] Saved patterns to " << patterns_file_path_ << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "[QuantumSignal] Could not save patterns: " << e.what() << std::endl;
    }
}

void QuantumSignalBridge::evolvePatternsWithFeedback(const std::string& pattern_id, bool profitable) {
    std::lock_guard<std::mutex> lock(analysis_mutex_);

    auto it = active_patterns_.find(pattern_id);
    if (it == active_patterns_.end()) {
        sep::quantum::Pattern p;
        p.id = pattern_id;
        p.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
        p.quantum_state.coherence = 0.5f;
        p.quantum_state.stability = 0.5f;
        active_patterns_[pattern_id] = p;
        it = active_patterns_.find(pattern_id);
    }

    float adjustment = profitable ? 0.05f : -0.05f;
    it->second.quantum_state.stability += adjustment;

    active_pattern_scores_[pattern_id] += profitable ? 0.1f : -0.1f;

    std::vector<sep::quantum::Pattern> patterns;
    patterns.reserve(active_patterns_.size());
    for (auto& kv : active_patterns_) {
        patterns.push_back(kv.second);
    }

    if (evolver_) {
        auto result = evolver_->evolvePatterns(patterns, 1.0f);
        active_patterns_.clear();
        for (auto& p : result.evolved_patterns) {
            active_patterns_[p.id] = p;
        }
    }

    savePatterns();
}

std::string QuantumSignalBridge::generatePatternId(const std::string& instrument, uint64_t timestamp) {
    return "pattern_" + instrument + "_" + std::to_string(timestamp);
}

void QuantumSignalBridge::addManagedPosition(const QuantumTradingSignal& signal, double current_price) {
    ManagedPosition pos;
    pos.id = generatePatternId(signal.instrument, signal.timestamp);
    pos.instrument = signal.instrument;
    pos.units = signal.action == QuantumTradingSignal::BUY ? signal.suggested_position_size : -signal.suggested_position_size;
    pos.entry_price = current_price;
    pos.stop_loss = signal.action == QuantumTradingSignal::BUY
                        ? current_price - signal.stop_loss_distance
                        : current_price + signal.stop_loss_distance;
    pos.take_profit = signal.action == QuantumTradingSignal::BUY
                          ? current_price + signal.take_profit_distance
                          : current_price - signal.take_profit_distance;
    pos.open_time = signal.timestamp;
    managed_positions_.push_back(pos);
}

void QuantumSignalBridge::updatePositions(const sep::connectors::MarketData& data) {
    for (auto it = managed_positions_.begin(); it != managed_positions_.end();) {
        if (it->instrument != data.instrument) {
            ++it;
            continue;
        }

        bool close = false;
        if (it->units > 0) {
            if (data.mid <= it->stop_loss || data.mid >= it->take_profit) close = true;
        } else {
            if (data.mid >= it->stop_loss || data.mid <= it->take_profit) close = true;
        }

        if (close) {
            std::cout << "[Position] Closed " << it->instrument << " at " << data.mid << std::endl;
            it = managed_positions_.erase(it);
        } else {
            ++it;
        }
    }
}

} // namespace sep::trading
