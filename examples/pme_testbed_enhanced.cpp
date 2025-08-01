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

// Enhanced pattern metrics calculations
class EnhancedPatternMetrics {
public:
    // Calculate true market coherence using autocorrelation
    static double calculateCoherence(const std::vector<double>& prices, int lag = 5) {
        if (prices.size() <= lag) return 0.5;
        
        double mean = 0.0;
        for (double price : prices) mean += price;
        mean /= prices.size();
        
        double autocorr = 0.0;
        double variance = 0.0;
        
        for (size_t i = lag; i < prices.size(); ++i) {
            double x = prices[i] - mean;
            double y = prices[i - lag] - mean;
            autocorr += x * y;
            variance += x * x;
        }
        
        if (variance == 0.0) return 0.5;
        autocorr /= variance;
        
        // Map to [0,1] range where 1 = perfect coherence
        return std::min(1.0, std::max(0.0, 0.5 + 0.5 * autocorr));
    }
    
    // Calculate multi-timeframe stability 
    static double calculateStability(const std::vector<Candle>& candles, size_t index, int window = 10) {
        if (index < window || index >= candles.size()) return 0.5;
        
        // Calculate short-term and medium-term trends
        double short_trend = 0.0;
        double medium_trend = 0.0;
        
        // Short-term (3 candles)
        for (int i = 1; i <= 3 && index >= i; ++i) {
            short_trend += candles[index].close - candles[index - i].close;
        }
        
        // Medium-term (10 candles)  
        for (int i = 1; i <= window && index >= i; ++i) {
            medium_trend += candles[index].close - candles[index - i].close;
        }
        
        // Trend consistency score
        bool trends_align = (short_trend * medium_trend) > 0;
        double trend_strength = std::abs(short_trend) / std::max(0.0001, std::abs(medium_trend));
        
        // Volatility adjustment
        double range_sum = 0.0;
        for (int i = 0; i < window && index >= i; ++i) {
            range_sum += candles[index - i].high - candles[index - i].low;
        }
        double avg_range = range_sum / window;
        double volatility_factor = 1.0 / (1.0 + avg_range * 10000); // Scale for forex
        
        double stability = trends_align ? 
            0.5 + 0.3 * std::min(1.0, trend_strength) + 0.2 * volatility_factor :
            0.5 - 0.2 * std::min(1.0, trend_strength);
            
        return std::min(1.0, std::max(0.0, stability));
    }
    
    // Calculate Shannon entropy of price movements
    static double calculateEntropy(const std::vector<Candle>& candles, size_t index, int window = 10) {
        if (index < window) return 0.5;
        
        // Discretize price movements into bins
        std::vector<int> bins(5, 0); // 5 bins: strong down, down, flat, up, strong up
        
        for (int i = 1; i <= window && index >= i; ++i) {
            double change = candles[index - i + 1].close - candles[index - i].close;
            double range = candles[index - i].high - candles[index - i].low;
            
            if (range > 0) {
                double normalized_change = change / range;
                
                if (normalized_change < -0.5) bins[0]++;
                else if (normalized_change < -0.1) bins[1]++;
                else if (normalized_change < 0.1) bins[2]++;
                else if (normalized_change < 0.5) bins[3]++;
                else bins[4]++;
            } else {
                bins[2]++; // No movement
            }
        }
        
        // Calculate Shannon entropy
        double entropy = 0.0;
        for (int count : bins) {
            if (count > 0) {
                double p = static_cast<double>(count) / window;
                entropy -= p * std::log2(p);
            }
        }
        
        // Normalize to [0,1] range
        return entropy / std::log2(5.0);
    }
};

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <path_to_data_file> [stability_w] [coherence_w] [entropy_w] [buy_threshold] [sell_threshold]" << std::endl;
        return 1;
    }

    std::string data_file_path = argv[1];
    
    // Parse optional parameters
    double stability_w = 0.5;
    double coherence_w = 0.3;
    double entropy_w = 0.2;
    double buy_score_threshold = 0.55;
    double sell_score_threshold = 0.55;
    
    if (argc >= 7) {
        stability_w = std::stod(argv[2]);
        coherence_w = std::stod(argv[3]);
        entropy_w = std::stod(argv[4]);
        buy_score_threshold = std::stod(argv[5]);
        sell_score_threshold = std::stod(argv[6]);
    }

    // Load and parse candle data
    std::ifstream file(data_file_path);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << data_file_path << std::endl;
        return 1;
    }

    json data;
    file >> data;
    
    std::vector<Candle> candles;
    if (data.contains("candles")) {
        candles = data["candles"].get<std::vector<Candle>>();
    } else if (data.is_array()) {
        candles = data.get<std::vector<Candle>>();
    }

    if (candles.empty()) {
        std::cerr << "Error: No candle data found" << std::endl;
        return 1;
    }

    std::cout << "Loaded " << candles.size() << " candles" << std::endl;
    std::cout << "Enhanced Pattern Analysis with weights: stability=" << stability_w 
              << ", coherence=" << coherence_w << ", entropy=" << entropy_w << std::endl;

    // Initialize quantum pattern engine
    sep::quantum::manifold::QuantumManifoldOptimizationEngine engine;

    // Convert candles to enhanced QuantumPatterns
    std::vector<sep::quantum::manifold::QuantumPattern> quantum_patterns;
    std::vector<double> close_prices;
    
    for (const auto& candle : candles) {
        close_prices.push_back(candle.close);
    }
    
    for (size_t i = 0; i < candles.size(); ++i) {
        const auto& candle = candles[i];
        sep::quantum::manifold::QuantumPattern q_p;
        q_p.id = "pattern_" + candle.time;
        
        // Enhanced pattern calculations
        q_p.coherence = EnhancedPatternMetrics::calculateCoherence(close_prices, 5);
        q_p.stability = EnhancedPatternMetrics::calculateStability(candles, i, 10);
        q_p.phase = EnhancedPatternMetrics::calculateEntropy(candles, i, 10);
        
        quantum_patterns.push_back(q_p);
    }

    // Process the patterns through the engine
    engine.processPatterns(quantum_patterns);
    auto metrics = engine.getMetrics();
    
    std::vector<sep::quantum::Signal> signals;

    // Enhanced signal generation with dynamic thresholds
    for (size_t i = 0; i < metrics.size(); ++i) {
        const auto& metric = metrics[i];
        sep::quantum::Signal signal;
        signal.pattern_id = metric.id;
        
        // Volatility-adjusted thresholds
        double volatility_factor = 1.0;
        if (i < candles.size()) {
            double range = candles[i].high - candles[i].low;
            volatility_factor = 1.0 + (range * 10000 - 10) * 0.01; // Adjust for forex volatility
            volatility_factor = std::max(0.5, std::min(2.0, volatility_factor));
        }
        
        double adj_buy_threshold = buy_score_threshold * volatility_factor;
        double adj_sell_threshold = sell_score_threshold * volatility_factor;
        
        // Enhanced scoring with volume confirmation if available
        double volume_factor = 1.0;
        if (i < candles.size() && candles[i].volume > 0) {
            double avg_volume = 150.0; // Rough average from data
            volume_factor = 0.8 + 0.4 * (candles[i].volume / avg_volume);
            volume_factor = std::max(0.5, std::min(1.5, volume_factor));
        }
        
        double buy_score = (metric.stability * stability_w) + 
                          (metric.coherence * coherence_w) + 
                          ((1.0 - metric.phase) * entropy_w);
        buy_score *= volume_factor;
        
        double sell_score = ((1.0 - metric.stability) * stability_w) + 
                           ((1.0 - metric.coherence) * coherence_w) + 
                           (metric.phase * entropy_w);
        sell_score *= volume_factor;
        
        if (buy_score > adj_buy_threshold) {
            signal.type = sep::quantum::SignalType::BUY;
            signal.confidence = buy_score;
        } else if (sell_score > adj_sell_threshold) {
            signal.type = sep::quantum::SignalType::SELL;
            signal.confidence = sell_score;
        } else {
            signal.type = sep::quantum::SignalType::HOLD;
            signal.confidence = 0.0;
        }
        
        signals.push_back(signal);
    }

    // Output enhanced CSV with additional metrics
    std::cout << "timestamp,open,high,low,close,volume,pattern_id,coherence,stability,entropy,volatility_adj,signal,signal_confidence" << std::endl;

    for (size_t i = 0; i < metrics.size() && i < candles.size(); ++i) {
        const auto& metric = metrics[i];
        const auto& candle = candles[i];
        
        std::cout << candle.time << "," << std::fixed << std::setprecision(5)
                  << candle.open << "," << candle.high << "," << candle.low << "," << candle.close 
                  << "," << candle.volume << "," << metric.id << ","
                  << metric.coherence << "," << metric.stability << "," << metric.phase;
                  
        // Add volatility adjustment factor
        double range = candle.high - candle.low;
        double volatility_adj = 1.0 + (range * 10000 - 10) * 0.01;
        volatility_adj = std::max(0.5, std::min(2.0, volatility_adj));
        std::cout << "," << volatility_adj;
        
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

    // Enhanced backtesting with pip-based validation
    int correct_predictions = 0;
    int total_predictions = 0;
    double total_pips = 0.0;

    for (size_t i = 0; i < candles.size() - 1; ++i) {
        const auto& current_candle = candles[i];
        const auto& next_candle = candles[i + 1];

        if (i < signals.size() && signals[i].type != sep::quantum::SignalType::HOLD) {
            total_predictions++;
            
            double pip_change = (next_candle.close - current_candle.close) * 10000;
            bool correct = false;
            
            if (signals[i].type == sep::quantum::SignalType::BUY && pip_change > 0.5) {
                correct = true;
                total_pips += pip_change;
            } else if (signals[i].type == sep::quantum::SignalType::SELL && pip_change < -0.5) {
                correct = true;
                total_pips += std::abs(pip_change);
            } else {
                total_pips += pip_change; // Track losses too
            }
            
            if (correct) {
                correct_predictions++;
            }
        }
    }

    std::cerr << "\n--- Enhanced Backtesting Results ---" << std::endl;
    if (total_predictions > 0) {
        double accuracy = static_cast<double>(correct_predictions) / total_predictions * 100.0;
        std::cerr << "Accuracy: " << std::fixed << std::setprecision(2) << accuracy << "%" << std::endl;
        std::cerr << "Correct Predictions: " << correct_predictions << std::endl;
        std::cerr << "Total Predictions: " << total_predictions << std::endl;
        std::cerr << "Total Pips: " << std::fixed << std::setprecision(1) << total_pips << std::endl;
        std::cerr << "Average Pips per Trade: " << std::fixed << std::setprecision(2) 
                  << (total_pips / total_predictions) << std::endl;
    } else {
        std::cerr << "No predictions made" << std::endl;
    }

    return 0;
}
