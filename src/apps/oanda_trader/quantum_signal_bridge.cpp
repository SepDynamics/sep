#include "quantum_signal_bridge.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

#include "../../quantum/types_serialization.h"

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

QuantumIdentifiers QuantumSignalBridge::calculateConvergedIdentifiers(
    const std::vector<sep::connectors::MarketData>& forward_window,
    size_t window_size) {
    
    QuantumIdentifiers identifiers;
    
    if (forward_window.size() < window_size) {
        std::cout << "[QuantumSignal] Insufficient forward window: " << forward_window.size() 
                  << " (need " << window_size << ")" << std::endl;
        return identifiers;
    }
    
    // Convert forward window to bits for analysis
    auto forward_bits = convertPriceToBits(forward_window);
    
    if (forward_bits.size() < 10) {
        std::cout << "[QuantumSignal] Insufficient forward bit data: " << forward_bits.size() << std::endl;
        return identifiers;
    }
    
    // Calculate converged identifiers using iterative convergence
    identifiers = calculateIdentifiersWithConvergence(forward_bits);
    
    std::cout << "[QuantumSignal] Converged identifiers: confidence=" << identifiers.confidence 
              << ", coherence=" << identifiers.coherence 
              << ", stability=" << identifiers.stability 
              << " (converged=" << identifiers.converged 
              << ", iterations=" << identifiers.iterations << ")" << std::endl;
    
    return identifiers;
}

QuantumIdentifiers QuantumSignalBridge::calculateIdentifiersWithConvergence(
    const std::vector<uint8_t>& forward_bits,
    int max_iterations,
    float convergence_threshold) {
    
    QuantumIdentifiers identifiers;
    identifiers.convergence_threshold = convergence_threshold;
    
    if (forward_bits.size() < 10) {
        std::cout << "[QuantumSignal] Insufficient bits for convergence: " << forward_bits.size() << std::endl;
        return identifiers;
    }
    
    // Initialize previous values
    float prev_confidence = 0.5f;
    float prev_coherence = 0.5f;
    float prev_stability = 0.5f;
    
    // Iterative convergence calculation
    for (int iteration = 0; iteration < max_iterations; ++iteration) {
        // Run QFH analysis on current bit window
        auto qfh_result = qfh_processor_->analyze(forward_bits);
        
        // Generate probe/expectation for QBSA (using proper indices, not values)
        std::vector<uint32_t> probe_indices;
        std::vector<uint32_t> expectations;
        
        // Generate probe indices every 4th position for efficiency
        for (size_t i = 0; i < std::min(forward_bits.size(), 64UL); i += 4) {
            probe_indices.push_back(static_cast<uint32_t>(i));  // Actual indices, not values
            
            // Generate expectation based on trend analysis around this index
            if (i >= 16) {
                // Look at bits around this index for pattern prediction
                int recent_ones = 0;
                size_t lookback_start = std::max(int(i) - 8, 0);
                for (size_t j = lookback_start; j < i; ++j) {
                    if (j < forward_bits.size() && forward_bits[j] == 1) recent_ones++;
                }
                
                size_t lookback_count = i - lookback_start;
                float trend_ratio = lookback_count > 0 ? float(recent_ones) / float(lookback_count) : 0.5f;
                
                // Predict based on trend
                if (trend_ratio >= 0.7f) {
                    expectations.push_back(static_cast<uint32_t>(i + 1));  // Expect trend continuation at next index
                } else if (trend_ratio <= 0.3f) {
                    expectations.push_back(static_cast<uint32_t>(i + 2));  // Expect reversal at later index
                } else {
                    expectations.push_back(static_cast<uint32_t>(i));  // Expect current pattern
                }
            } else {
                expectations.push_back(static_cast<uint32_t>(i + 1));  // Simple momentum
            }
        }
        
        // Run QBSA analysis with proper indices
        auto qbsa_result = qbsa_processor_->analyze(probe_indices, expectations);
        
        // Calculate new identifier values using convergence damping
        float damping_factor = 0.1f;  // Control convergence speed
        
        // Confidence: inverse of correction ratio with damping
        float new_confidence = prev_confidence * (1.0f - damping_factor) + 
                              (1.0f - qbsa_result.correction_ratio) * damping_factor;
        
        // Coherence: QFH coherence with damping  
        float new_coherence = prev_coherence * (1.0f - damping_factor) + 
                             qfh_result.coherence * damping_factor;
        
        // Stability: entropy-based with damping
        float entropy_stability = std::clamp(1.0f - qfh_result.entropy, 0.0f, 1.0f);
        float new_stability = prev_stability * (1.0f - damping_factor) + 
                             entropy_stability * damping_factor;
        
        // Check for convergence
        float confidence_diff = std::abs(new_confidence - prev_confidence);
        float coherence_diff = std::abs(new_coherence - prev_coherence);
        float stability_diff = std::abs(new_stability - prev_stability);
        
        bool converged = (confidence_diff < convergence_threshold && 
                         coherence_diff < convergence_threshold && 
                         stability_diff < convergence_threshold);
        
        // Update values
        prev_confidence = new_confidence;
        prev_coherence = new_coherence;
        prev_stability = new_stability;
        identifiers.iterations = iteration + 1;
        
        if (converged) {
            identifiers.converged = true;
            std::cout << "[QuantumSignal] Converged after " << (iteration + 1) << " iterations" << std::endl;
            break;
        }
    }
    
    // Store final converged values
    identifiers.confidence = prev_confidence;
    identifiers.coherence = prev_coherence;
    identifiers.stability = prev_stability;
    
    std::cout << "[QuantumSignal] Convergence result: confidence=" << identifiers.confidence 
              << ", coherence=" << identifiers.coherence 
              << ", stability=" << identifiers.stability 
              << " (converged=" << identifiers.converged 
              << ", iterations=" << identifiers.iterations << ")" << std::endl;
    
    return identifiers;
}

QuantumTradingSignal QuantumSignalBridge::analyzeMarketData(
    const sep::connectors::MarketData& current_data,
    const std::vector<sep::connectors::MarketData>& history) {
    
    std::lock_guard<std::mutex> lock(analysis_mutex_);
    
    QuantumTradingSignal signal;
    signal.instrument = current_data.instrument;
    signal.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    signal.source_candle_timestamp = current_data.timestamp;
    
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
        std::vector<uint32_t> probe_values;
        std::vector<uint32_t> expectations;

        // Generate probe values (every 4th bit for efficiency)
        for (size_t i = 0; i < std::min(bits.size(), 64UL); i += 4) {
            probe_values.push_back(bits[i]);
            
            // Create realistic expectations based on market pattern analysis
            // QBSA compares probe_values[x] vs expectations[x] for same index
            if (i >= 16) {  // Need enough history for meaningful prediction
                // Look at recent actual bits around this position for trend
                int recent_ones = 0;
                size_t lookback_start = std::max(int(i) - 8, 0);  // Look back 8 bits
                for (size_t j = lookback_start; j < i; ++j) {
                    if (j < bits.size() && bits[j] == 1) recent_ones++;
                }
                
                size_t lookback_count = i - lookback_start;
                float trend_ratio = lookback_count > 0 ? float(recent_ones) / float(lookback_count) : 0.5f;
                
                // Trend-based prediction 
                if (trend_ratio >= 0.7f) {
                    expectations.push_back(1);  // Strong uptrend, expect continuation
                } else if (trend_ratio <= 0.3f) {
                    expectations.push_back(0);  // Strong downtrend, expect continuation  
                } else {
                    // Neutral - use simple momentum from nearest bit
                    size_t nearest_bit = std::min(i - 1, bits.size() - 1);
                    expectations.push_back(bits[nearest_bit]);  // Momentum continuation
                }
            } else {
                // For early positions, use simple momentum
                if (i > 0 && i < bits.size()) {
                    expectations.push_back(bits[i - 1]);  // Previous bit momentum
                } else {
                    expectations.push_back(0);  // Default
                }
            }
        }

        // Debug: Show first few probe vs expectation pairs
        if (probe_values.size() >= 3) {
            std::cout << "[DEBUG] QBSA probe vs expect: (" << probe_values[0] << "vs" << expectations[0] 
                      << ") (" << probe_values[1] << "vs" << expectations[1] 
                      << ") (" << probe_values[2] << "vs" << expectations[2] << ")" << std::endl;
        }
        
        auto qbsa_result = qbsa_processor_->analyze(probe_values, expectations);
        last_qbsa_result_ = qbsa_result;
        
        // For now, use legacy calculation until convergence is implemented
        // TODO: Replace with calculateConvergedIdentifiers() using forward window
        signal.identifiers.confidence = calculateConfidence(qfh_result, qbsa_result);
        signal.identifiers.coherence = calculateCoherence(qfh_result); 
        signal.identifiers.stability = calculateStability(history);
        
        // Store debug metrics in identifiers
        signal.identifiers.entropy = qfh_result.entropy;
        signal.identifiers.flip_ratio = qfh_result.flip_ratio;
        signal.identifiers.rupture_ratio = qfh_result.rupture_ratio;
        signal.identifiers.quantum_collapse_detected = qfh_result.collapse_detected;

        // Direction determination based on normalized stability [0, 1]:
        // < 0.45 = SELL (low stability = volatile conditions), > 0.55 = BUY (high stability =
        // trending conditions)
        if (signal.identifiers.stability < 0.45f)
        {
            signal.action = QuantumTradingSignal::SELL;
        }
        else if (signal.identifiers.stability > 0.55f)
        {
            signal.action = QuantumTradingSignal::BUY;
        }
        else
        {
            // HOLD zone - stability between 0.45-0.55 indicates uncertain market conditions
            signal.action = QuantumTradingSignal::HOLD;
        }

        // Apply strategy thresholds (from alpha analysis)
        bool meets_confidence = signal.identifiers.confidence >= confidence_threshold_.load();
        bool meets_coherence = signal.identifiers.coherence >= coherence_threshold_.load();
        // For normalized stability [0,1], check distance from neutral (0.5)
        bool meets_stability = std::abs(signal.identifiers.stability - 0.5f) >= stability_threshold_.load();
        
        std::cout << "[QuantumSignal] Metrics - Confidence: " << signal.identifiers.confidence 
                  << " (≥" << confidence_threshold_.load() << ": " << (meets_confidence ? "PASS" : "FAIL") << ")"
                  << " Coherence: " << signal.identifiers.coherence 
                  << " (≥" << coherence_threshold_.load() << ": " << (meets_coherence ? "PASS" : "FAIL") << ")"
                  << " Stability: " << signal.identifiers.stability 
                  << " (|0.5-val|≥" << stability_threshold_.load() << ": " << (meets_stability ? "PASS" : "FAIL") << ")"
                  << " Direction: " << (signal.action == QuantumTradingSignal::BUY ? "BUY" : 
                                       signal.action == QuantumTradingSignal::SELL ? "SELL" : "HOLD")
                  << std::endl;
        
        if (meets_confidence && meets_coherence && meets_stability && signal.action != QuantumTradingSignal::HOLD) {
            signal.should_execute = true;
            
            // Calculate risk management parameters
            signal.suggested_position_size = calculatePositionSize(signal.identifiers.confidence, 10000.0); // Default balance
            signal.stop_loss_distance = calculateStopLoss(signal.identifiers.coherence, current_data.mid);
            signal.take_profit_distance = calculateTakeProfit(signal.identifiers.confidence, current_data.mid);
            
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
        
        // Enhanced bit generation with sensitivity to small moves
        uint8_t direction_bit;
        
        if (std::abs(pip_change) >= 2) {
            // Strong directional move
            direction_bit = (pip_change > 0) ? 1 : 0;
        } else if (std::abs(pip_change) == 1) {
            // Weak move - add some uncertainty based on recent volatility
            direction_bit = (pip_change > 0) ? 1 : 0;
            // Add variation for consecutive weak moves
            if (i >= 3) {
                int recent_weak_moves = 0;
                for (size_t j = i-2; j < i; ++j) {
                    if (j < history.size() - 1) {
                        double recent_change = history[j+1].mid - history[j].mid;
                        if (std::abs(static_cast<int>(recent_change * 10000)) <= 1) {
                            recent_weak_moves++;
                        }
                    }
                }
                // If too many consecutive weak moves, introduce pattern break
                if (recent_weak_moves >= 2 && (i % 7) == 0) {
                    direction_bit = 1 - direction_bit; // Flip
                }
            }
        } else {
            // No change - use previous direction but add noise
            direction_bit = bits.empty() ? 0 : bits.back();
            // Add periodic noise for flat markets
            if ((i % 11) == 0) {
                direction_bit = 1 - direction_bit;
            }
        }
        
        bits.push_back(direction_bit);
    }
    
    std::cout << "[QuantumSignal] Converted " << history.size() 
              << " price points to " << bits.size() << " bits" << std::endl;
    
    return bits;
}

float QuantumSignalBridge::calculateConfidence(const sep::quantum::QFHResult& qfh_result, 
                                              const sep::quantum::QBSAResult& qbsa_result) {
    // Confidence based on QBSA correction_ratio (per documentation)
    // "The QBSA algorithm calculates a correction_ratio that measures pattern stability"
    // "This is represented as signal_confidence in the data"
    
    std::cout << "[DEBUG] QFH input: entropy=" << qfh_result.entropy 
              << " flip_ratio=" << qfh_result.flip_ratio 
              << " rupture_ratio=" << qfh_result.rupture_ratio 
              << " coherence=" << qfh_result.coherence
              << " QBSA correction_ratio=" << qbsa_result.correction_ratio << std::endl;
    
    // Primary confidence is inverse of correction ratio
    // High correction ratio = low confidence (more corrections needed)
    // Low correction ratio = high confidence (fewer corrections needed)
    float confidence = 1.0f - qbsa_result.correction_ratio;
    
    std::cout << "[DEBUG] Confidence calc: correction_ratio=" << qbsa_result.correction_ratio 
              << " -> confidence=" << confidence << std::endl;

    return std::clamp(confidence, 0.0f, 1.0f);
}

float QuantumSignalBridge::calculateCoherence(const sep::quantum::QFHResult& qfh_result) {
    // Use the QFH coherence directly (per documentation)
    // QFH already calculates coherence properly in the pattern analysis
    float coherence = qfh_result.coherence;
    
    std::cout << "[DEBUG] Coherence calc: using QFH coherence directly=" << coherence << std::endl;
    
    return std::clamp(coherence, 0.0f, 1.0f);
}

float QuantumSignalBridge::calculateStability(const std::vector<sep::connectors::MarketData>& history) {
    if (history.size() < 10) {
        return 0.5f; // Neutral if insufficient history
    }

    size_t window = std::min<size_t>(30, history.size());
    size_t start = history.size() - window;

    std::vector<double> diffs;
    diffs.reserve(window - 1);
    double prev = history[start].mid;
    double sum = 0.0;
    for (size_t i = start + 1; i < history.size(); ++i) {
        double diff = history[i].mid - prev;
        diffs.push_back(diff);
        sum += diff;
        prev = history[i].mid;
    }

    double mean = sum / diffs.size();
    double var = 0.0;
    for (double d : diffs) {
        double delta = d - mean;
        var += delta * delta;
    }
    var /= diffs.size();
    double stddev = std::sqrt(var);
    if (!std::isfinite(stddev)) {
        return 0.5f;
    }

    double score = std::tanh(std::abs(mean) / (stddev + 1e-6));
    double normalized = mean >= 0.0 ? 0.5 + score * 0.5 : 0.5 - score * 0.5;
    return static_cast<float>(normalized);
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
                sep::quantum::Pattern p;
                sep::quantum::from_json(pj, p);
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
            nlohmann::json pattern_json;
            sep::quantum::to_json(pattern_json, kv.second);
            patterns_json.push_back(pattern_json);
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
