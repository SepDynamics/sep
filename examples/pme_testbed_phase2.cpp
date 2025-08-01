#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <nlohmann/json.hpp>
#include <cmath>
#include <algorithm>
#include <iomanip>

#include "quantum/quantum_manifold_optimizer.h"
#include "quantum/signal.h"
#include "quantum/bitspace/qfh.h"
#include "apps/oanda_trader/forward_window_kernels.hpp"

using json = nlohmann::json;

struct Candle {
    std::string time;
    double open, high, low, close, volume;
};

void from_json(const json& j, Candle& c) {
    j.at("time").get_to(c.time);
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

// Phase 2: Advanced Market Analysis
class AdvancedMarketAnalyzer {
public:
    enum MarketRegime {
        TRENDING_UP,
        TRENDING_DOWN,
        RANGING,
        HIGH_VOLATILITY,
        LOW_VOLATILITY
    };
    
    struct MarketState {
        MarketRegime regime;
        double confidence;
        double volatility_level;
        double trend_strength;
        bool is_liquid_session;
    };
    
    static MarketState analyzeMarketRegime(const std::vector<Candle>& candles, size_t index) {
        MarketState state;
        
        if (index < 20) {
            state.regime = RANGING;
            state.confidence = 0.5;
            state.volatility_level = 0.5;
            state.trend_strength = 0.0;
            state.is_liquid_session = true;
            return state;
        }
        
        // Calculate trend strength over 20 periods
        double trend_sum = 0.0;
        double volatility_sum = 0.0;
        
        for (int i = 1; i <= 20; ++i) {
            double price_change = candles[index - i + 1].close - candles[index - i].close;
            double range = candles[index - i].high - candles[index - i].low;
            
            trend_sum += price_change;
            volatility_sum += range;
        }
        
        double avg_change = trend_sum / 20.0;
        double avg_volatility = volatility_sum / 20.0;
        
        // Normalize for forex (EUR/USD typical values)
        state.trend_strength = std::abs(avg_change) * 10000; // Convert to pips
        state.volatility_level = avg_volatility * 10000;
        
        // Market regime classification
        if (state.trend_strength > 15.0) { // Strong trend > 15 pips
            state.regime = avg_change > 0 ? TRENDING_UP : TRENDING_DOWN;
            state.confidence = std::min(1.0, state.trend_strength / 30.0);
        } else if (state.volatility_level > 20.0) { // High volatility > 20 pips
            state.regime = HIGH_VOLATILITY;
            state.confidence = std::min(1.0, state.volatility_level / 40.0);
        } else if (state.volatility_level < 8.0) { // Low volatility < 8 pips
            state.regime = LOW_VOLATILITY;
            state.confidence = std::min(1.0, (8.0 - state.volatility_level) / 8.0);
        } else {
            state.regime = RANGING;
            state.confidence = 1.0 - (state.trend_strength / 15.0);
        }
        
        // Check for liquid trading session (simplified)
        state.is_liquid_session = true; // Assume all data is from liquid sessions
        
        return state;
    }
    
    static double calculateSignalQuality(const sep::quantum::manifold::QuantumPattern& pattern, 
                                       const MarketState& market_state) {
        double quality = 0.5; // Base quality
        
        // Pattern coherence quality
        if (pattern.coherence > 0.7) quality += 0.2;
        else if (pattern.coherence < 0.3) quality -= 0.2;
        
        // Stability quality based on market regime
        switch (market_state.regime) {
            case TRENDING_UP:
            case TRENDING_DOWN:
                if (pattern.stability > 0.6) quality += 0.15;
                break;
            case RANGING:
                if (pattern.stability > 0.4 && pattern.stability < 0.6) quality += 0.1;
                break;
            case HIGH_VOLATILITY:
                quality -= 0.1; // Reduce quality in high volatility
                break;
            case LOW_VOLATILITY:
                if (pattern.coherence > 0.6) quality += 0.1;
                break;
        }
        
        // Entropy quality (complexity measure)
        double entropy_factor = 1.0 - std::abs(pattern.phase - 0.5) * 2.0; // Prefer moderate entropy
        quality += entropy_factor * 0.1;
        
        return std::min(1.0, std::max(0.0, quality));
    }
};

int main(int argc, char** argv) {
    if (argc != 2 && argc != 7) {
        std::cerr << "Usage: " << argv[0] << " <path_to_data_file> [stability_w] [coherence_w] [entropy_w] [buy_threshold] [sell_threshold]" << std::endl;
        return 1;
    }

    std::string data_file_path = argv[1];

    std::ifstream data_stream(data_file_path);
    if (!data_stream) {
        std::cerr << "Failed to open data file: " << data_file_path << std::endl;
        return 1;
    }

    json j;
    data_stream >> j;

    std::vector<Candle> candles;
    if (j.contains("candles")) {
        candles = j["candles"].get<std::vector<Candle>>();
    } else if (j.is_array()) {
        candles = j.get<std::vector<Candle>>();
    }

    if (candles.empty()) {
        std::cerr << "Error: No candle data found" << std::endl;
        return 1;
    }

    std::cout << "Loaded " << candles.size() << " candles for Phase 2 Enhanced Analysis" << std::endl;

    // Initialize quantum pattern engine
    sep::quantum::manifold::QuantumManifoldOptimizationEngine engine;
    engine.initialize();

    // =================================================================
    // EXPERIMENT 024: THE GREAT UNIFICATION 
    // Switch from legacy QuantumManifoldOptimizationEngine to enhanced QFHBasedProcessor
    // This connects Phase 2 trajectory damping and pattern vocabulary to main testbed
    // Goal: Use trajectory-based damping and enhanced patterns for real accuracy improvement
    // =================================================================
    
    // Initialize enhanced QFH processor with trajectory damping
    sep::quantum::QFHOptions qfh_options;
    qfh_options.collapse_threshold = 0.3f;  // Rupture ratio threshold
    qfh_options.flip_threshold = 0.7f;      // Flip ratio threshold
    sep::quantum::QFHBasedProcessor qfh_processor(qfh_options);
    
    // Convert candle data to bitstreams for QFH analysis
    std::vector<double> close_prices;
    for (const auto& candle : candles) {
        close_prices.push_back(candle.close);
    }
    
    // Generate bitstream from price movements
    std::vector<uint8_t> price_bitstream;
    for (size_t i = 1; i < close_prices.size(); ++i) {
        // Convert price movement to bit: 1 = up, 0 = down
        price_bitstream.push_back(close_prices[i] > close_prices[i-1] ? 1 : 0);
    }
    
    std::cout << "Generated bitstream of " << price_bitstream.size() << " bits from " 
              << close_prices.size() << " price points" << std::endl;
    
    std::vector<sep::quantum::manifold::QuantumPattern> quantum_patterns;
    
    // Process patterns using enhanced QFH with trajectory damping
    for (size_t i = 0; i < candles.size(); ++i) {
        const auto& candle = candles[i];
        
        sep::quantum::manifold::QuantumPattern q_p;
        q_p.id = "pattern_" + candle.time;
        
        // Extract bitstream window for this candle
        if (i < price_bitstream.size() && i >= 10) {
            size_t window_start = std::max(0, (int)i - 10);
            size_t window_end = std::min(price_bitstream.size(), i + 1);
            std::vector<uint8_t> window_bits(price_bitstream.begin() + window_start, 
            price_bitstream.begin() + window_end);
            
            if (window_bits.size() < 2) {
                // Too small for transitions - use fallback
                continue;  // Skip to next iteration
            }
            
            // Use ENHANCED QFH processor with trajectory damping
            sep::quantum::QFHResult qfh_result = qfh_processor.analyze(window_bits);
            
            // Apply trajectory-based damping using Phase 2 enhancements
            auto damped_trajectory = qfh_processor.integrateFutureTrajectories(window_bits, window_bits.size()/2);
            
            // Extract enhanced metrics from QFH analysis
            q_p.coherence = qfh_result.coherence;
            q_p.stability = 1.0f - qfh_result.rupture_ratio; // Stability inversely related to ruptures
            q_p.phase = qfh_result.entropy / 2.0f; // Normalize entropy to [0,1]
            
            // Apply trajectory damping to coherence (Phase 2 enhancement)
            double trajectory_confidence = qfh_processor.matchKnownPaths({damped_trajectory.final_value});
            q_p.coherence = 0.7 * q_p.coherence + 0.3 * trajectory_confidence;
            
        } else {
            // Fallback to legacy forward window metrics for edge cases
            if (i > 0 && i <= price_bitstream.size()) {
                size_t window_start = std::max(0, (int)i - 5);
                size_t window_end = std::min(price_bitstream.size(), i);
                std::vector<uint8_t> window_bits(price_bitstream.begin() + window_start,
                                               price_bitstream.begin() + window_end);
                
                auto fw_result = sep::apps::cuda::simulateForwardWindowMetrics(window_bits, 0);
                q_p.coherence = fw_result.coherence;
                q_p.stability = fw_result.stability;
                q_p.phase = fw_result.entropy / 2.0f;
            } else {
                q_p.coherence = 0.5;
                q_p.stability = 0.5;
                q_p.phase = 0.5;
            }
        }
        
        quantum_patterns.push_back(q_p);
    }

    // DEBUG: Pattern count analysis
    std::cout << "DEBUG: Created " << quantum_patterns.size() << " patterns from " 
              << candles.size() << " candles" << std::endl;
    
    // Process patterns through engine
    engine.processPatterns(quantum_patterns);
    auto metrics = engine.getMetrics();
    
    std::vector<sep::quantum::Signal> signals;

    // EXPERIMENT 001: Phase 1 parameters in Phase 2 framework
    double stability_w = 0.4;     // Phase 1 proven weight
    double coherence_w = 0.4;     // Phase 1 proven weight
    double entropy_w = 0.2;       // Phase 1 proven weight
    double base_buy_threshold = 0.50;   // Phase 1 proven threshold
    double base_sell_threshold = 0.52;  // Phase 1 asymmetric threshold
    
    // Quality filtering parameters
    double min_signal_quality = 0.45; // Balanced threshold for quality vs quantity
    
    // Volatility-adaptive thresholds (from Phase 1)
    double avg_volatility = 0.0;
    int vol_window = std::min(100, (int)candles.size());
    for (int i = 0; i < vol_window; ++i) {
        int idx = candles.size() - 1 - i;
        avg_volatility += candles[idx].high - candles[idx].low;
    }
    avg_volatility /= vol_window;
    double volatility_multiplier = 1.0 + (avg_volatility * 10000 - 10) * 0.02;
    volatility_multiplier = std::max(0.8, std::min(1.5, volatility_multiplier));
    
    if (argc == 7) {
        stability_w = std::stod(argv[2]);
        coherence_w = std::stod(argv[3]);
        entropy_w = std::stod(argv[4]);
        base_buy_threshold = std::stod(argv[5]);
        base_sell_threshold = std::stod(argv[6]);
    }

    // EXPERIMENT 011: Multi-timeframe enhanced signal generation
    for (size_t pattern_idx = 0; pattern_idx < metrics.size(); ++pattern_idx) {
        const auto& metric = metrics[pattern_idx];
        sep::quantum::Signal signal;
        signal.pattern_id = metric.id;
        
        const Candle* candle = nullptr;
        for (const auto& c : candles) {
            if ("pattern_" + c.time == signal.pattern_id) {
                candle = &c;
            }
        }

        // Phase 1's volume confirmation factor
        double volume_factor = 1.0;
        if (candle && candle->volume > 0) {
            double avg_volume = 150.0; // Approximate average from data
            volume_factor = 0.85 + 0.3 * (candle->volume / avg_volume);
            volume_factor = std::max(0.7, std::min(1.4, volume_factor));
        }
        
        // EXPERIMENT 022: Proven Experiment 011 with dynamic adaptive thresholds
        double base_buy_score = (metric.stability * stability_w) + 
                               (metric.coherence * coherence_w) + 
                               ((1.0 - metric.phase) * entropy_w);
        
        double base_sell_score = ((1.0 - metric.stability) * stability_w) + 
                                ((1.0 - metric.coherence) * coherence_w) + 
                                (metric.phase * entropy_w);
        
        // Exact multi-timeframe coherence boost from successful Experiment 011
        double temporal_coherence = 1.0;
        if (pattern_idx >= 15) { // Need enough history for timeframe analysis
            // Calculate 5-minute and 15-minute pattern coherence
            double tf5_coherence = 0.0, tf15_coherence = 0.0;
            int tf5_window = 5, tf15_window = 15;
            
            // 5-minute timeframe coherence
            for (int j = 1; j <= tf5_window && pattern_idx >= (size_t)j; ++j) {
                tf5_coherence += metrics[pattern_idx-j].coherence;
            }
            tf5_coherence /= tf5_window;
            
            // 15-minute timeframe coherence  
            for (int j = 1; j <= tf15_window && pattern_idx >= (size_t)j; ++j) {
                tf15_coherence += metrics[pattern_idx-j].coherence;
            }
            tf15_coherence /= tf15_window;
            
            // Temporal alignment bonus (exact from Experiment 011)
            if (std::abs(metric.coherence - tf5_coherence) < 0.1 && 
                std::abs(tf5_coherence - tf15_coherence) < 0.1) {
                temporal_coherence = 1.15; // 15% boost for temporal alignment
            } else if (std::abs(metric.coherence - tf5_coherence) < 0.2) {
                temporal_coherence = 1.08; // 8% boost for partial alignment
            }
        }
        
        double buy_score = base_buy_score * volume_factor * temporal_coherence;
        double sell_score = base_sell_score * volume_factor * temporal_coherence;
        
        // EXPERIMENT 023: Phase 1 Simple Volatility Adaptation
        // Use Phase 1's proven simple volatility multiplier without complex regime analysis
        // Based on phase_comparison.md: "Simple volatility adaptation outperformed complex regime logic"
        
        // Phase 1's direct volatility adaptation - simple and effective
        double buy_threshold = base_buy_threshold * volatility_multiplier;
        double sell_threshold = base_sell_threshold * volatility_multiplier;

        // DEBUG: Phase 1 volatility adaptation analysis (first 5 patterns)
        static int debug_count = 0;
        if (debug_count < 5) {
            std::cout << "PHASE2 EXPERIMENT_023[" << debug_count << "]: buy_score=" << buy_score 
                      << " sell_score=" << sell_score << " vol_mult=" << volatility_multiplier
                      << " buy_thresh=" << buy_threshold << " sell_thresh=" << sell_threshold << std::endl;
            debug_count++;
        }

        if (buy_score > buy_threshold) {
            signal.type = sep::quantum::SignalType::BUY;
            signal.confidence = buy_score;
        } else if (sell_score > sell_threshold) {
            signal.type = sep::quantum::SignalType::SELL;
            signal.confidence = sell_score;
        } else {
            signal.type = sep::quantum::SignalType::HOLD;
            signal.confidence = 0.0;
        }
        
        signals.push_back(signal);
    }

    // EXPERIMENT 005: Simplified output without market regime
    std::cout << "timestamp,open,high,low,close,volume,pattern_id,coherence,stability,entropy,signal,signal_confidence" << std::endl;

    for (size_t i = 0; i < metrics.size() && i < candles.size(); ++i) {
        const auto& metric = metrics[i];
        const auto& candle = candles[i];
        
        std::cout << candle.time << "," << std::fixed << std::setprecision(5)
                  << candle.open << "," << candle.high << "," << candle.low << "," << candle.close 
                  << "," << candle.volume << "," << metric.id << ","
                  << metric.coherence << "," << metric.stability << "," << metric.phase;
                  
        if (i < signals.size()) {
            const auto& signal = signals[i];
            switch (signal.type) {
                case sep::quantum::SignalType::BUY:
                    std::cout << ",BUY";
                    break;
                case sep::quantum::SignalType::SELL:
                    std::cout << ",SELL";
                    break;
                default:
                    std::cout << ",HOLD";
                    break;
            }
            std::cout << "," << signal.confidence;
        } else {
            std::cout << ",HOLD,0.0";
        }
        std::cout << std::endl;
    }

    // EXPERIMENT 007: Threshold calibration with signal distribution analysis
    int correct_predictions = 0;
    int total_predictions = 0;
    int high_confidence_correct = 0;
    int high_confidence_total = 0;

    // Signal distribution tracking
    double min_confidence = 1.0, max_confidence = 0.0, sum_confidence = 0.0;
    double min_coherence = 1.0, max_coherence = 0.0, sum_coherence = 0.0;
    double min_stability = 1.0, max_stability = 0.0, sum_stability = 0.0;
    int signal_count = 0;

    // EXPERIMENT 011: Multi-timeframe with optimized thresholds
    double confidence_threshold = 0.65; // Proven optimal from exp 009
    double coherence_threshold = 0.55;  // Proven optimal from exp 009  
    double stability_threshold = 0.0;

    for (size_t i = 0; i < candles.size() - 1 && i < signals.size() && i < metrics.size(); ++i) {
        const auto& current_candle = candles[i];
        const auto& next_candle = candles[i + 1];
        const auto& metric = metrics[i];

        if (signals[i].type != sep::quantum::SignalType::HOLD) {
            total_predictions++;
            signal_count++;
            
            // Track signal distribution
            min_confidence = std::min(min_confidence, signals[i].confidence);
            max_confidence = std::max(max_confidence, signals[i].confidence);
            sum_confidence += signals[i].confidence;
            
            min_coherence = std::min(min_coherence, metric.coherence);
            max_coherence = std::max(max_coherence, metric.coherence);
            sum_coherence += metric.coherence;
            
            min_stability = std::min(min_stability, metric.stability);
            max_stability = std::max(max_stability, metric.stability);
            sum_stability += metric.stability;
            
            // Check if signal meets calibrated criteria
            bool high_confidence = (signals[i].confidence >= confidence_threshold &&
                                   metric.coherence >= coherence_threshold &&
                                   metric.stability >= stability_threshold);
            
            if (high_confidence) {
                high_confidence_total++;
            }
            
            bool correct = false;
            double pip_change = (next_candle.close - current_candle.close) * 10000;
            
            if (signals[i].type == sep::quantum::SignalType::BUY && pip_change > 0.5) {
                correct = true;
            } else if (signals[i].type == sep::quantum::SignalType::SELL && pip_change < -0.5) {
                correct = true;
            }
            
            if (correct) {
                correct_predictions++;
                if (high_confidence) {
                    high_confidence_correct++;
                }
            }
        }
    }

    // Display signal distribution analysis
    std::cerr << "\n--- Signal Distribution Analysis ---" << std::endl;
    if (signal_count > 0) {
        std::cerr << "Confidence: min=" << std::fixed << std::setprecision(3) << min_confidence 
                  << " max=" << max_confidence << " avg=" << (sum_confidence/signal_count) << std::endl;
        std::cerr << "Coherence:  min=" << min_coherence 
                  << " max=" << max_coherence << " avg=" << (sum_coherence/signal_count) << std::endl;
        std::cerr << "Stability:  min=" << min_stability 
                  << " max=" << max_stability << " avg=" << (sum_stability/signal_count) << std::endl;
    }

    std::cerr << "\n--- Phase 2 Multi-Timeframe Enhanced Results ---" << std::endl;
    if (total_predictions > 0) {
        double accuracy = static_cast<double>(correct_predictions) / total_predictions * 100.0;
        std::cerr << "Overall Accuracy: " << std::fixed << std::setprecision(2) << accuracy << "%" << std::endl;
        std::cerr << "Correct Predictions: " << correct_predictions << std::endl;
        std::cerr << "Total Predictions: " << total_predictions << std::endl;
        
        std::cerr << "Thresholds: confidence≥" << confidence_threshold 
                  << " coherence≥" << coherence_threshold 
                  << " stability≥" << stability_threshold << std::endl;
        
        if (high_confidence_total > 0) {
            double hc_accuracy = static_cast<double>(high_confidence_correct) / high_confidence_total * 100.0;
            std::cerr << "High Confidence Accuracy: " << std::fixed << std::setprecision(2) << hc_accuracy << "%" << std::endl;
            std::cerr << "High Confidence Signals: " << high_confidence_total << " (" 
                      << std::fixed << std::setprecision(1) << (100.0 * high_confidence_total / total_predictions) << "%)" << std::endl;
        } else {
            std::cerr << "No high confidence signals found with current thresholds" << std::endl;
        }
    } else {
        std::cerr << "No predictions made" << std::endl;
    }

    return 0;
}
