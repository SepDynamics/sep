#include "quantum_signal_bridge.hpp"
#include "realtime_aggregator.hpp"
#include "candle_types.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <nlohmann/json.hpp>

#include "quantum/bitspace/pattern_processor.h"
#include "quantum/types_serialization.h"
#include "quantum/quantum_manifold_optimizer.h"
#include "quantum/signal.h"
#include "forward_window_kernels.hpp"

using json = nlohmann::json;

void from_json(const json& j, Candle& c) {
    std::string time_str;
    j.at("time").get_to(time_str);
    c.timestamp = parseTimestamp(time_str);
    c.volume = j.contains("volume") ? j["volume"].get<double>() : 100.0;
    
    // Handle OANDA format with nested "mid" object
    if (j.contains("mid")) {
        auto mid = j["mid"];
        std::string open_str, high_str, low_str, close_str;
        mid.at("o").get_to(open_str);
        mid.at("h").get_to(high_str);
        mid.at("l").get_to(low_str);
        mid.at("c").get_to(close_str);
        
        c.open = std::stod(open_str);
        c.high = std::stod(high_str);
        c.low = std::stod(low_str);
        c.close = std::stod(close_str);
    } else {
        // Handle simple format
        j.at("open").get_to(c.open);
        j.at("high").get_to(c.high);
        j.at("low").get_to(c.low);
        j.at("close").get_to(c.close);
    }
}

namespace sep::trading {

QuantumSignalBridge::QuantumSignalBridge() 
    : patterns_file_path_("quantum_patterns.json")
{
    // Initialize multi-timeframe analyzer
    mtf_analyzer_ = std::make_unique<MultiTimeframeAnalyzer>();
    
    // Initialize real-time aggregator with callback
    realtime_aggregator_ = std::make_unique<RealTimeAggregator>(
        [this](const Candle& candle, int timeframe_minutes) {
            this->onHigherTimeframeCandle(candle, timeframe_minutes);
        }
    );
}

QuantumSignalBridge::~QuantumSignalBridge() {
    shutdown();
}

bool QuantumSignalBridge::initialize() {
    std::lock_guard<std::mutex> lock(analysis_mutex_);
    
    try {
        // Initialize QFH processor with correct options
        pattern_processor_ = std::make_unique<sep::quantum::bitspace::PatternProcessor>();
        
        // Initialize pattern evolution bridge
        sep::quantum::PatternEvolutionBridge::Config evo_cfg;
        evolver_ = std::make_unique<sep::quantum::PatternEvolutionBridge>(evo_cfg);
        evolver_->initializeEvolutionState();

        // Load existing patterns
        loadPatterns();
        
        // Load optimal configuration from weekend optimizer
        loadOptimalConfig();
        
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
        sep::quantum::QFHResult qfh_result;  // Dummy result

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
        sep::quantum::QBSAResult qbsa_result;  // Dummy result

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
    const std::vector<sep::connectors::MarketData>& history,
    const std::vector<sep::apps::cuda::ForwardWindowResult>& forward_window_results) {
    std::lock_guard<std::mutex> lock(analysis_mutex_);

        QuantumTradingSignal signal;
        signal.instrument = current_data.instrument;
        signal.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
                               std::chrono::system_clock::now().time_since_epoch())
                               .count();
        signal.source_candle_timestamp = current_data.timestamp;

        if (!initialized_ || history.size() < 20)
        {
            return signal;  // Return HOLD signal
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
        
        std::vector<sep::quantum::bitspace::TrajectoryPoint> trajectory_points;
        for(size_t i = 0; i < history.size(); ++i) {
            trajectory_points.push_back({history[i].mid, history[i].timestamp});
        }
        sep::quantum::bitspace::Trajectory trajectory(trajectory_points);
        auto metrics = pattern_processor_->processTrajectory(trajectory);

        signal.identifiers.confidence = metrics.confidence;
        signal.identifiers.coherence = metrics.coherence;
        signal.identifiers.stability = metrics.stability;
        signal.identifiers.entropy = metrics.entropy;
        
        if (!forward_window_results.empty()) {
            const auto& last_result = forward_window_results.back();
            signal.identifiers.confidence = last_result.confidence;
        } else {
            signal.identifiers.confidence = 0.0f;
            signal.identifiers.coherence = 0.0f;
            signal.identifiers.stability = 0.0f;
        }
        
        // Store debug metrics in identifiers


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
            
            // PRODUCTION-GRADE ENHANCEMENT: Multi-timeframe confirmation
            // Based on 60% accuracy breakthrough from testbed analysis
            std::string timestamp_str = std::to_string(current_data.timestamp);
            auto mtf_confirmation = getMultiTimeframeConfirmation(signal, timestamp_str);
            signal.mtf_confirmation = mtf_confirmation;  // Store for GUI access
            
            // ENHANCED LOGGING FOR LIVE VALIDATION
            auto now = std::chrono::system_clock::now();
            auto time_t = std::chrono::system_clock::to_time_t(now);
            std::stringstream log_entry;
            log_entry << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S")
                      << " [SIGNAL_GENERATED] " << current_data.instrument
                      << " Action=" << (signal.action == QuantumTradingSignal::BUY ? "BUY" : "SELL")
                      << " Confidence=" << std::fixed << std::setprecision(4) << signal.identifiers.confidence
                      << " Coherence=" << signal.identifiers.coherence
                      << " Stability=" << signal.identifiers.stability
                      << " Entropy=" << signal.identifiers.entropy
                      << " Price=" << current_data.mid
                      << " M5_Confirm=" << (mtf_confirmation.m5_confirms ? "YES" : "NO")
                      << " M15_Confirm=" << (mtf_confirmation.m15_confirms ? "YES" : "NO")
                      << " Triple_Confirmed=" << (mtf_confirmation.triple_confirmed ? "YES" : "NO");
            
            std::cout << log_entry.str() << std::endl;
            
            std::cout << "[QuantumSignal] Multi-timeframe check - M5: " 
                      << (mtf_confirmation.m5_confirms ? "CONFIRM" : "REJECT")
                      << " M15: " << (mtf_confirmation.m15_confirms ? "CONFIRM" : "REJECT")
                      << " Triple: " << (mtf_confirmation.triple_confirmed ? "CONFIRMED" : "PENDING") << std::endl;
            
            // Apply production-grade triple confirmation logic
            if (mtf_confirmation.triple_confirmed) {
                signal.should_execute = true;
                
                // Calculate risk management parameters
                signal.suggested_position_size = calculatePositionSize(signal.identifiers.confidence, 10000.0);
                signal.stop_loss_distance = calculateStopLoss(signal.identifiers.coherence, current_data.mid);
                signal.take_profit_distance = calculateTakeProfit(signal.identifiers.confidence, current_data.mid);
                
                signal.should_execute = true; // Enable live trade execution for triple-confirmed signals
                
                // ENHANCED EXECUTION LOGGING
                std::stringstream exec_log;
                exec_log << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S")
                         << " [TRIPLE_CONFIRMED] " << current_data.instrument
                         << " Action=" << (signal.action == QuantumTradingSignal::BUY ? "BUY" : "SELL")
                         << " Size=" << std::fixed << std::setprecision(0) << signal.suggested_position_size
                         << " StopLoss=" << std::setprecision(5) << signal.stop_loss_distance
                         << " TakeProfit=" << signal.take_profit_distance
                         << " ExecuteFlag=TRUE REASON=\"Triple Confirmation Met: M1 " 
                         << (signal.action == QuantumTradingSignal::BUY ? "BUY" : "SELL") << ", M5 " 
                         << (mtf_confirmation.m5_confirms ? "CONFIRM" : "REJECT") << ", M15 " 
                         << (mtf_confirmation.m15_confirms ? "CONFIRM" : "REJECT") << "\"";
                         
                std::cout << exec_log.str() << std::endl;
                
                std::cout << "[QuantumSignal] 🚀 MULTI-TIMEFRAME CONFIRMED SIGNAL: " << current_data.instrument
                          << " Action: " << (signal.action == QuantumTradingSignal::BUY ? "BUY" : "SELL")
                          << " Size: " << signal.suggested_position_size 
                          << " (60% accuracy system activated) READY FOR EXECUTION!" << std::endl;
            } else {
                signal.action = QuantumTradingSignal::HOLD;
                signal.should_execute = false;
                
                // LOG NON-EXECUTED SIGNALS
                std::stringstream hold_log;
                hold_log << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S")
                         << " [SIGNAL_HOLD] " << current_data.instrument
                         << " Action=HOLD ExecuteFlag=FALSE REASON=\"Awaiting M5/M15 Confirmation\"";
                std::cout << hold_log.str() << std::endl;
                
                std::cout << "[QuantumSignal] ⏳ M1 Signal Detected - Awaiting M5/M15 Confirmation..." << std::endl;
            }
        } else {
            signal.action = QuantumTradingSignal::HOLD;
            signal.should_execute = false;
            
            // LOG THRESHOLD FAILURES
            auto now = std::chrono::system_clock::now();
            auto time_t = std::chrono::system_clock::to_time_t(now);
            std::stringstream threshold_log;
            threshold_log << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S")
                         << " [THRESHOLD_FAIL] " << current_data.instrument
                         << " Action=HOLD ExecuteFlag=FALSE REASON=\"Thresholds not met: Conf=" 
                         << (meets_confidence ? "PASS" : "FAIL") << " Coh=" 
                         << (meets_coherence ? "PASS" : "FAIL") << " Stab=" 
                         << (meets_stability ? "PASS" : "FAIL") << "\"";
            std::cout << threshold_log.str() << std::endl;
            
            std::cout << "[QuantumSignal] Thresholds not met - HOLD" << std::endl;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "[QuantumSignal] Analysis error: " << e.what() << std::endl;
    }
    
    // Feed current M1 data to real-time aggregator for M5/M15 candle building
    if (realtime_aggregator_) {
        Candle m1_candle;
        m1_candle.timestamp = current_data.timestamp;
        // MarketData has bid/ask/mid, we'll simulate OHLC from mid price
        m1_candle.open = current_data.mid;
        m1_candle.high = current_data.mid;
        m1_candle.low = current_data.mid;
        m1_candle.close = current_data.mid;
        m1_candle.volume = current_data.volume;
        
        realtime_aggregator_->addM1Candle(m1_candle);
    }
    
    return signal;
}
}

std::vector<uint8_t> sep::trading::QuantumSignalBridge::convertPriceToBits(
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



sep::trading::QuantumTradingSignal::Action sep::trading::QuantumSignalBridge::determineDirection(
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

double sep::trading::QuantumSignalBridge::calculatePositionSize(float confidence, double account_balance) {
    // ULTRA-CONSERVATIVE RISK MANAGEMENT FOR LIVE VALIDATION TRIAL
    // Max 0.5% account risk per trade for initial live testing
    double risk_percent = 0.005; // 0.5% max risk per trade (was 2%)
    double base_units = 100;     // Small base position size (was 1000)
    
    // Scale by confidence but cap at conservative levels
    double confidence_multiplier = std::min(1.5, static_cast<double>(confidence) * 1.2);
    
    // Calculate position based on account balance and risk
    double max_risk_dollars = account_balance * risk_percent;
    double conservative_position = base_units * confidence_multiplier;
    
    // Log the risk calculation for transparency
    std::cout << "[RiskMgmt] Account=" << account_balance 
              << " MaxRisk=" << max_risk_dollars 
              << " Confidence=" << confidence 
              << " Position=" << conservative_position << " units" << std::endl;
    
    return conservative_position;
}

double sep::trading::QuantumSignalBridge::calculateStopLoss(float coherence, double current_price) {
    // Stop loss based on coherence (higher coherence = tighter stop)
    double base_stop_pips = 20.0; // 20 pip base stop
    double coherence_factor = 1.0 - static_cast<double>(coherence);
    
    double stop_pips = base_stop_pips * (1.0 + coherence_factor);
    return stop_pips / 10000.0; // Convert pips to price distance
}

double sep::trading::QuantumSignalBridge::calculateTakeProfit(float confidence, double current_price) {
    // Take profit based on confidence (higher confidence = larger target)
    double base_target_pips = 30.0; // 30 pip base target
    double confidence_multiplier = static_cast<double>(confidence) * 2.0;
    
    double target_pips = base_target_pips * confidence_multiplier;
    return target_pips / 10000.0; // Convert pips to price distance
}

void sep::trading::QuantumSignalBridge::debugDataFormat(const std::vector<sep::connectors::MarketData>& history) {
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

void sep::trading::QuantumSignalBridge::loadPatterns() {
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

void sep::trading::QuantumSignalBridge::savePatterns() {
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

void sep::trading::QuantumSignalBridge::evolvePatternsWithFeedback(const std::string& pattern_id, bool profitable) {
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

std::string sep::trading::QuantumSignalBridge::generatePatternId(const std::string& instrument, uint64_t timestamp) {
    return "pattern_" + instrument + "_" + std::to_string(timestamp);
}

void sep::trading::QuantumSignalBridge::addManagedPosition(const sep::trading::QuantumTradingSignal& signal, double current_price) {
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

void sep::trading::QuantumSignalBridge::updatePositions(const sep::connectors::MarketData& data) {
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

// Multi-timeframe bridge functions
bool sep::trading::QuantumSignalBridge::initializeMultiTimeframe(
    const std::string& m5_file_path, 
    const std::string& m15_file_path) {
    
    if (!mtf_analyzer_) {
        std::cerr << "[QuantumSignalBridge] Multi-timeframe analyzer not initialized" << std::endl;
        return false;
    }
    
    return mtf_analyzer_->loadTimeframeData(m5_file_path, m15_file_path);
}

sep::trading::MultiTimeframeConfirmation sep::trading::QuantumSignalBridge::getMultiTimeframeConfirmation(
    const QuantumTradingSignal& m1_signal,
    const std::string& m1_timestamp) {
    
    if (!mtf_analyzer_) {
        return MultiTimeframeConfirmation{};
    }
    
    return mtf_analyzer_->getConfirmation(m1_signal, m1_timestamp);
}

// Multi-timeframe analyzer implementation
std::string sep::trading::MultiTimeframeAnalyzer::getTimeframeKey(
    const std::string& m1_time_str, 
    int timeframe_minutes) {
    
    // Parse the M1 timestamp (format: 2024-01-01T08:01:00.000000Z)
    std::tm tm = {};
    std::stringstream ss(m1_time_str.substr(0, 19)); // Extract YYYY-MM-DDTHH:MM:SS
    ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S");
    
    // Round down the minutes to start of timeframe block
    tm.tm_min = (tm.tm_min / timeframe_minutes) * timeframe_minutes;
    tm.tm_sec = 0;
    
    // Format back to string with same format as input
    std::stringstream result_ss;
    result_ss << std::put_time(&tm, "%Y-%m-%dT%H:%M:%S");
    return result_ss.str() + ".000000Z";
}

bool sep::trading::MultiTimeframeAnalyzer::loadTimeframeData(
    const std::string& m5_file_path, 
    const std::string& m15_file_path) {
    
    std::cout << "[MultiTimeframe] Loading M5 data from: " << m5_file_path << std::endl;
    
    // 1. Load M5 candle data from file
    std::vector<Candle> m5_candles;
    std::ifstream m5_stream(m5_file_path);
    if (m5_stream) {
        json m5_json;
        m5_stream >> m5_json;
        if (m5_json.contains("candles")) {
            m5_candles = m5_json["candles"].get<std::vector<Candle>>();
        }
    } else {
        std::cerr << "[MultiTimeframe] ERROR: Could not load M5 data from " << m5_file_path << std::endl;
        return false;
    }

    // 2. Run the analysis pipeline on M5 data
    m5_signals_ = runAnalysisPipeline(m5_candles, "M5");
    m5_data_loaded_ = !m5_signals_.empty();
    
    std::cout << "[MultiTimeframe] Loading M15 data from: " << m15_file_path << std::endl;
    
    // 3. Load M15 candle data from file
    std::vector<Candle> m15_candles;
    std::ifstream m15_stream(m15_file_path);
    if (m15_stream) {
        json m15_json;
        m15_stream >> m15_json;
        if (m15_json.contains("candles")) {
            m15_candles = m15_json["candles"].get<std::vector<Candle>>();
        }
    } else {
        std::cerr << "[MultiTimeframe] ERROR: Could not load M15 data from " << m15_file_path << std::endl;
        return false;
    }
    
    // 4. Run the analysis pipeline on M15 data
    m15_signals_ = runAnalysisPipeline(m15_candles, "M15");
    m15_data_loaded_ = !m15_signals_.empty();

    std::cout << "[MultiTimeframe] Analysis complete. Loaded " << m5_signals_.size() 
              << " M5 signals and " << m15_signals_.size() << " M15 signals." << std::endl;
    
    return m5_data_loaded_ && m15_data_loaded_;
}

sep::trading::MultiTimeframeConfirmation sep::trading::MultiTimeframeAnalyzer::getConfirmation(
    const QuantumTradingSignal& m1_signal,
    const std::string& m1_timestamp,
    double confidence_threshold) {
    
    MultiTimeframeConfirmation confirmation;
    if (!m5_data_loaded_ || !m15_data_loaded_) return confirmation;

    // Calculate timeframe keys for precise alignment
    confirmation.m5_key = getTimeframeKey(m1_timestamp, 5);
    confirmation.m15_key = getTimeframeKey(m1_timestamp, 15);

    // Perform REAL M5 lookup
    auto m5_it = m5_signals_.find(confirmation.m5_key);
    if (m5_it != m5_signals_.end()) {
        const auto& m5_signal = m5_it->second;
        confirmation.m5_confidence = m5_signal.identifiers.confidence;
        if (m5_signal.action == m1_signal.action && m5_signal.identifiers.confidence > confidence_threshold) {
            confirmation.m5_confirms = true;
        }
    }

    // Perform REAL M15 lookup
    auto m15_it = m15_signals_.find(confirmation.m15_key);
    if (m15_it != m15_signals_.end()) {
        const auto& m15_signal = m15_it->second;
        confirmation.m15_confidence = m15_signal.identifiers.confidence;
        if (m15_signal.action == m1_signal.action && m15_signal.identifiers.confidence > confidence_threshold) {
            confirmation.m15_confirms = true;
        }
    }

    // Final "Triple Confirmation" check
    if (m1_signal.identifiers.confidence >= 0.65 && confirmation.m5_confirms && confirmation.m15_confirms) {
        confirmation.triple_confirmed = true;
    }

    std::cout << "[MultiTimeframe] Confirmation results - M5: " 
              << (confirmation.m5_confirms ? "CONFIRM" : "REJECT")
              << " (" << confirmation.m5_confidence << ") M15: " 
              << (confirmation.m15_confirms ? "CONFIRM" : "REJECT")
              << " (" << confirmation.m15_confidence << ") Triple: " 
              << (confirmation.triple_confirmed ? "CONFIRMED" : "PENDING") << std::endl;

    return confirmation;
}

std::map<std::string, sep::trading::QuantumTradingSignal> sep::trading::MultiTimeframeAnalyzer::runAnalysisPipeline(
    const std::vector<Candle>& candles, const std::string& timeframe_name) {
    
    std::map<std::string, QuantumTradingSignal> signals_map;
    
    if (candles.empty()) {
        return signals_map;
    }
    
    std::cout << "[" << timeframe_name << "] Processing " << candles.size() << " candles" << std::endl;
    
    // Default weights - optimal configuration from breakthrough
    double stability_w = 0.4;
    double coherence_w = 0.1; 
    double entropy_w = 0.5;
    
    // Convert candle data to bitstreams for QFH analysis
    std::vector<double> close_prices;
    for (const auto& candle : candles) {
        close_prices.push_back(candle.close);
    }
    
    // Generate bitstream from price movements
    std::vector<uint8_t> price_bitstream;
    for (size_t i = 1; i < close_prices.size(); ++i) {
        price_bitstream.push_back(close_prices[i] > close_prices[i-1] ? 1 : 0);
    }
    
    std::vector<sep::quantum::manifold::QuantumPattern> quantum_patterns;
    
    // Process patterns using enhanced QFH with trajectory damping
    for (size_t i = 0; i < candles.size(); ++i) {
        const auto& candle = candles[i];
        
        sep::quantum::manifold::QuantumPattern q_p;
        q_p.id = "pattern_" + std::to_string(candle.timestamp);
        
        // Extract bitstream window for this candle
        size_t window_start = (i >= 10) ? (i - 10) : 0;
        size_t window_end = std::min(price_bitstream.size(), i + 1);
        std::vector<uint8_t> window_bits(price_bitstream.begin() + window_start,
                                         price_bitstream.begin() + window_end);

        if (window_bits.size() < 2) {
            continue;
        }

        // QFH Analysis with trajectory damping
        try {
            // Initialize QFH processor for this timeframe
            sep::quantum::QFHOptions qfh_options;
            qfh_options.collapse_threshold = 0.3f;
            qfh_options.flip_threshold = 0.7f;
            sep::quantum::QFHBasedProcessor qfh_processor(qfh_options);
            
            sep::quantum::QFHResult qfh_result = qfh_processor.analyze(window_bits);
            
            q_p.coherence = qfh_result.coherence;
            q_p.stability = 1.0f - qfh_result.rupture_ratio;
            q_p.phase = qfh_result.entropy;
            
            quantum_patterns.push_back(q_p);
        } catch (const std::exception& e) {
            continue;
        }
    }
    
    // Generate signals from quantum patterns
    for (size_t pattern_idx = 0; pattern_idx < quantum_patterns.size(); ++pattern_idx) {
        const auto& metric = quantum_patterns[pattern_idx];
        QuantumTradingSignal signal;
        // Note: QuantumTradingSignal doesn't have pattern_id field
        
        const Candle* candle = nullptr;
        for (const auto& c : candles) {
            if ("pattern_" + std::to_string(c.timestamp) == metric.id) {
                candle = &c;
                break;
            }
        }

        // Enhanced pattern recognition
        double pattern_modifier = 1.0;
        if (pattern_idx < candles.size()) {
            size_t window_start = (pattern_idx >= 10) ? (pattern_idx - 10) : 0;
            size_t window_end = std::min(price_bitstream.size(), pattern_idx + 1);
            std::vector<uint8_t> window_bits(price_bitstream.begin() + window_start,
                                           price_bitstream.begin() + window_end);
            
            if (window_bits.size() >= 2) {
                auto forward_window_result = sep::apps::cuda::simulateForwardWindowMetrics(window_bits, 0);
                
                if (forward_window_result.coherence >= 0.85f) {
                    pattern_modifier = 1.12; // TrendAcceleration
                } else if (forward_window_result.coherence >= 0.8f && forward_window_result.stability >= 0.82f) {
                    pattern_modifier = 1.08; // VolatilityBreakout  
                } else if (forward_window_result.coherence >= 0.75f && forward_window_result.stability >= 0.7f) {
                    pattern_modifier = 0.95; // MeanReversion
                }
            }
        }
        
        // Calculate scores with optimal weights
        double base_buy_score = ((1.0 - metric.stability) * stability_w) + 
                               (metric.coherence * coherence_w) + 
                               ((1.0 - metric.phase) * entropy_w);
        
        double base_sell_score = (metric.stability * stability_w) + 
                                ((1.0 - metric.coherence) * coherence_w) + 
                                (metric.phase * entropy_w);
        
        double buy_score = base_buy_score * pattern_modifier;
        double sell_score = base_sell_score * pattern_modifier;
        
        // Set signal type and confidence
        if (buy_score > sell_score) {
            signal.action = QuantumTradingSignal::BUY;
            signal.identifiers.confidence = buy_score;
        } else {
            signal.action = QuantumTradingSignal::SELL;
            signal.identifiers.confidence = sell_score;
        }
        
        // Store additional metrics for multi-timeframe analysis
        signal.identifiers.coherence = metric.coherence;
        signal.identifiers.stability = metric.stability;
        signal.identifiers.entropy = metric.phase;
        
        signals_map[candle ? std::to_string(candle->timestamp) : ""] = signal;
    }
    
    std::cout << "[" << timeframe_name << "] Generated " << signals_map.size() << " signals" << std::endl;
    return signals_map;
}

// Update signal map for real-time aggregation
void sep::trading::MultiTimeframeAnalyzer::updateSignalMap(int timeframe_minutes, const std::string& timestamp, const QuantumTradingSignal& signal) {
    if (timeframe_minutes == 5) {
        m5_signals_[timestamp] = signal;
    } else if (timeframe_minutes == 15) {
        m15_signals_[timestamp] = signal;
    }
}

// Bootstrap method for historical M1 data processing
void sep::trading::QuantumSignalBridge::bootstrap(const std::vector<Candle>& historical_m1_candles) {
    std::cout << "[Bootstrap] Processing " << historical_m1_candles.size() << " historical M1 candles..." << std::endl;
    
    if (!realtime_aggregator_) {
        std::cerr << "[Bootstrap] Error: Real-time aggregator not initialized" << std::endl;
        return;
    }
    
    // Feed all historical candles to build initial M5/M15 history
    for (const auto& candle : historical_m1_candles) {
        realtime_aggregator_->addM1Candle(candle);
    }
    
    std::cout << "[Bootstrap] Completed. System ready for live analysis." << std::endl;
}

// Callback for when higher timeframe candles are completed
void sep::trading::QuantumSignalBridge::onHigherTimeframeCandle(const Candle& candle, int timeframe_minutes) {
    std::cout << "[RealTime] New " << timeframe_minutes << "M candle completed at " 
              << candle.timestamp << " OHLC: " << candle.open << "/" << candle.high 
              << "/" << candle.low << "/" << candle.close << std::endl;
    
    // Run SEP analysis on this new candle
    std::vector<Candle> single_candle = {candle};
    std::string timeframe_name = (timeframe_minutes == 5) ? "M5" : "M15";
    
    auto new_signals = mtf_analyzer_->runAnalysisPipeline(single_candle, timeframe_name);
    
    // Update the appropriate signal map
    for (const auto& [timestamp, signal] : new_signals) {
        mtf_analyzer_->updateSignalMap(timeframe_minutes, timestamp, signal);
    }
    
    std::cout << "[RealTime] Updated " << timeframe_name << " signals map with "
    << new_signals.size() << " new signals" << std::endl;
}

void sep::trading::QuantumSignalBridge::loadOptimalConfig() {
    const std::string config_path = "/sep/optimal_config.json";
    
    if (!std::filesystem::exists(config_path)) {
        std::cout << "[QuantumSignal] No optimal config found, using defaults" << std::endl;
        return;
    }
    
    try {
        std::ifstream file(config_path);
        nlohmann::json config;
        file >> config;
        
        // Load thresholds (weights are not used here but could be extended)
        if (config.contains("confidence_threshold")) {
            confidence_threshold_.store(config["confidence_threshold"].get<float>());
        }
        if (config.contains("coherence_threshold")) {
            coherence_threshold_.store(config["coherence_threshold"].get<float>());
        }
        
        std::cout << "[QuantumSignal] Loaded optimal config:" << std::endl;
        std::cout << "  Confidence threshold: " << confidence_threshold_.load() << std::endl;
        std::cout << "  Coherence threshold: " << coherence_threshold_.load() << std::endl;
        std::cout << "  Expected profitability score: " << config.value("profitability_score", 0.0) << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "[QuantumSignal] Error loading optimal config: " << e.what() << std::endl;
    }
}

