#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>

#include "quantum/quantum_manifold_optimizer.h"
#include "quantum/signal.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct Candle {
    std::string time;
    double open;
    double high;
    double low;
    double close;
    int volume;
};

void from_json(const json& j, Candle& c) {
    j.at("time").get_to(c.time);
    j.at("volume").get_to(c.volume);
    
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
    
    // Handle both array format and OANDA object format
    if (j.is_array()) {
        candles = j.get<std::vector<Candle>>();
    } else if (j.contains("candles") && j["candles"].is_array()) {
        candles = j["candles"].get<std::vector<Candle>>();
    } else {
        std::cerr << "Invalid JSON format. Expected array of candles or object with 'candles' array." << std::endl;
        return 1;
    }

    // Instantiate the full SEP Engine
    sep::quantum::manifold::QuantumManifoldOptimizationEngine engine;
    engine.initialize();



    // Convert candles to QuantumPatterns with ENHANCED CALCULATIONS
    std::vector<sep::quantum::manifold::QuantumPattern> quantum_patterns;
    std::vector<double> close_prices;
    
    // Build price array for autocorrelation
    for (const auto& candle : candles) {
        close_prices.push_back(candle.close);
    }
    
    for (size_t i = 0; i < candles.size(); ++i) {
        const auto& candle = candles[i];
        sep::quantum::manifold::QuantumPattern q_p;
        q_p.id = "pattern_" + candle.time;
        
        // ENHANCED COHERENCE: Autocorrelation-based
        if (i >= 5 && close_prices.size() > 5) {
            double autocorr = 0.0, variance = 0.0;
            int lag = 3; // 3-candle lag optimal for forex
            double mean = 0.0;
            int window = std::min(10, (int)i);
            
            // Calculate mean for window
            for (int j = 0; j < window; ++j) {
                mean += close_prices[i - j];
            }
            mean /= window;
            
            // Calculate autocorrelation
            for (int j = lag; j < window; ++j) {
                double x = close_prices[i - j] - mean;
                double y = close_prices[i - j + lag] - mean;
                autocorr += x * y;
                variance += x * x;
            }
            
            q_p.coherence = variance > 0 ? 
                std::min(1.0, std::max(0.0, 0.5 + 0.5 * (autocorr / variance))) : 0.5;
        } else {
            q_p.coherence = 0.5; // Default for insufficient data
        }
        
        // ENHANCED STABILITY: Multi-timeframe trend consistency
        if (i >= 10) {
            double short_trend = 0.0, medium_trend = 0.0;
            
            // 3-candle short trend
            for (int j = 1; j <= 3; ++j) {
                short_trend += candles[i].close - candles[i - j].close;
            }
            
            // 10-candle medium trend
            for (int j = 1; j <= 10; ++j) {
                medium_trend += candles[i].close - candles[i - j].close;
            }
            
            bool trends_align = (short_trend * medium_trend) > 0;
            double trend_ratio = std::abs(short_trend) / std::max(0.0001, std::abs(medium_trend));
            
            // Volatility adjustment
            double avg_range = 0.0;
            for (int j = 0; j < 10; ++j) {
                avg_range += candles[i - j].high - candles[i - j].low;
            }
            avg_range /= 10.0;
            double volatility_factor = 1.0 / (1.0 + avg_range * 10000); // Scale for forex
            
            q_p.stability = trends_align ? 
                std::min(1.0, 0.5 + 0.3 * trend_ratio + 0.2 * volatility_factor) :
                std::max(0.0, 0.5 - 0.2 * trend_ratio);
        } else {
            q_p.stability = 0.5; // Default
        }
        
        // ENHANCED ENTROPY: Shannon entropy of price movements
        if (i >= 10) {
            std::vector<int> bins(5, 0); // 5 movement categories
            
            for (int j = 1; j <= 10; ++j) {
                double change = candles[i - j + 1].close - candles[i - j].close;
                double range = candles[i - j].high - candles[i - j].low;
                
                if (range > 0) {
                    double norm_change = change / range;
                    if (norm_change < -0.5) bins[0]++;      // Strong down
                    else if (norm_change < -0.1) bins[1]++; // Down
                    else if (norm_change < 0.1) bins[2]++;  // Flat
                    else if (norm_change < 0.5) bins[3]++;  // Up
                    else bins[4]++;                         // Strong up
                } else {
                    bins[2]++; // Flat for zero range
                }
            }
            
            double entropy = 0.0;
            for (int count : bins) {
                if (count > 0) {
                    double p = count / 10.0;
                    entropy -= p * std::log2(p);
                }
            }
            
            q_p.phase = entropy / std::log2(5.0); // Normalize to [0,1]
        } else {
            q_p.phase = 0.5; // Default
        }
        
        quantum_patterns.push_back(q_p);
    }

    // DEBUG: Pattern count analysis
    std::cout << "DEBUG: Created " << quantum_patterns.size() << " patterns from " 
              << candles.size() << " candles" << std::endl;
    
    // Process the patterns through the engine
    engine.processPatterns(quantum_patterns);
    auto metrics = engine.getMetrics();
    
    std::vector<sep::quantum::Signal> signals;

    // --- ENHANCED SIGNAL GENERATION ---
    // Optimized weights based on market analysis
    double stability_w = 0.4;    // Reduced from 0.5 - trend less reliable in forex
    double coherence_w = 0.4;    // Increased from 0.3 - pattern coherence crucial
    double entropy_w = 0.2;      // Keep same - entropy as complexity measure
    
    // Dynamic thresholds based on market conditions
    double base_buy_threshold = 0.50;   // Lowered from 0.55 for more signals
    double base_sell_threshold = 0.52;  // Slightly higher - asymmetric for market bias
    
    // Volatility-adaptive thresholds
    double avg_volatility = 0.0;
    int vol_window = std::min(100, (int)candles.size());
    for (int i = 0; i < vol_window; ++i) {
        int idx = candles.size() - 1 - i;
        avg_volatility += candles[idx].high - candles[idx].low;
    }
    avg_volatility /= vol_window;
    double volatility_multiplier = 1.0 + (avg_volatility * 10000 - 10) * 0.02; // Adjust based on pip volatility
    volatility_multiplier = std::max(0.8, std::min(1.5, volatility_multiplier));

    if (argc == 7) {
        stability_w = std::stod(argv[2]);
        coherence_w = std::stod(argv[3]);
        entropy_w = std::stod(argv[4]);
        base_buy_threshold = std::stod(argv[5]);
        base_sell_threshold = std::stod(argv[6]);
    }


    for (const auto& metric : metrics) {
        sep::quantum::Signal signal;
        signal.pattern_id = metric.id;
        
        const Candle* candle = nullptr;
        for (const auto& c : candles) {
            if ("pattern_" + c.time == signal.pattern_id) {
                candle = &c;
            }
        }

        // --- ENHANCED SIGNAL SCORING ---
        // Volume confirmation factor
        double volume_factor = 1.0;
        if (candle && candle->volume > 0) {
            double avg_volume = 150.0; // Approximate average from data
            volume_factor = 0.85 + 0.3 * (candle->volume / avg_volume);
            volume_factor = std::max(0.7, std::min(1.4, volume_factor));
        }
        
        // Enhanced scoring with weighted patterns
        double buy_score = (metric.stability * stability_w) + 
                          (metric.coherence * coherence_w) + 
                          ((1.0 - metric.phase) * entropy_w);
        buy_score *= volume_factor;
        
        double sell_score = ((1.0 - metric.stability) * stability_w) + 
                           ((1.0 - metric.coherence) * coherence_w) + 
                           (metric.phase * entropy_w);
        sell_score *= volume_factor;
        
        // Apply dynamic thresholds with volatility adjustment
        double buy_threshold = base_buy_threshold * volatility_multiplier;
        double sell_threshold = base_sell_threshold * volatility_multiplier;

        // DEBUG: Score analysis (first 5 patterns)
        static int debug_count = 0;
        if (debug_count < 5) {
            std::cout << "PHASE1 DEBUG[" << debug_count << "]: buy_score=" << buy_score 
                      << " sell_score=" << sell_score << " buy_thresh=" << buy_threshold 
                      << " sell_thresh=" << sell_threshold << " vol_mult=" << volatility_multiplier << std::endl;
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
        // --- END ENHANCED SIGNAL SCORING ---
        
        signals.push_back(signal);
    }

    std::cout << "timestamp,open,high,low,close,volume,pattern_id,coherence,stability,entropy,signal,signal_confidence" << std::endl;

    for (const auto& metric : metrics) {
        std::string pattern_id_str = metric.id;
        std::string timestamp = pattern_id_str.substr(8);

        const Candle* candle = nullptr;
        for (const auto& c : candles) {
            if (c.time == timestamp) {
                candle = &c;
                break;
            }
        }

        if (candle) {
            const sep::quantum::Signal* signal = nullptr;
            for (const auto& s : signals) {
                if (s.pattern_id == metric.id) {
                    signal = &s;
                    break;
                }
            }

            std::cout << std::fixed << std::setprecision(5)
                      << candle->time << ","
                      << candle->open << ","
                      << candle->high << ","
                      << candle->low << ","
                      << candle->close << ","
                      << candle->volume << ","
                      << pattern_id_str << ","
                      << metric.coherence << ","
                      << metric.stability << ","
                      << metric.phase << ",";

            if (signal) {
                switch (signal->type) {
                    case sep::quantum::SignalType::BUY:
                        std::cout << "BUY";
                        break;
                    case sep::quantum::SignalType::SELL:
                        std::cout << "SELL";
                        break;
                    case sep::quantum::SignalType::HOLD:
                        std::cout << "HOLD";
                        break;
                }
                std::cout << "," << signal->confidence;
            } else {
                std::cout << "HOLD,0.0";
            }
            std::cout << std::endl;
        }
    }

    // --- Backtesting Logic ---
    int correct_predictions = 0;
    int total_predictions = 0;

    for (size_t i = 0; i < candles.size() - 1; ++i) {
        const auto& current_candle = candles[i];
        const auto& next_candle = candles[i + 1];

        const sep::quantum::Signal* signal = nullptr;
        std::string pattern_id = "pattern_" + current_candle.time;

        for (const auto& s : signals) {
            if (s.pattern_id == pattern_id) {
                signal = &s;
                break;
            }
        }

        if (signal && signal->type != sep::quantum::SignalType::HOLD) {
            total_predictions++;
            bool correct = false;
            if (signal->type == sep::quantum::SignalType::BUY) {
                if (next_candle.close > current_candle.close) {
                    correct = true;
                }
            } else if (signal->type == sep::quantum::SignalType::SELL) {
                if (next_candle.close < current_candle.close) {
                    correct = true;
                }
            }
            if (correct) {
                correct_predictions++;
            }
        }
    }

    std::cout << "\n--- Backtesting Results ---" << std::endl;
    if (total_predictions > 0) {
        double accuracy = static_cast<double>(correct_predictions) / total_predictions * 100.0;
        std::cout << "Accuracy: " << std::fixed << std::setprecision(2) << accuracy << "%" << std::endl;
        std::cout << "Correct Predictions: " << correct_predictions << std::endl;
        std::cout << "Total Predictions: " << total_predictions << std::endl;
    } else {
        std::cout << "No BUY or SELL signals were generated for backtesting." << std::endl;
    }
    std::cout << "-------------------------" << std::endl;

    return 0;
}