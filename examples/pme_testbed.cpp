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
    j.at("open").get_to(c.open);
    j.at("high").get_to(c.high);
    j.at("low").get_to(c.low);
    j.at("close").get_to(c.close);
    j.at("volume").get_to(c.volume);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <path_to_data_file>" << std::endl;
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

    auto candles = j.get<std::vector<Candle>>();

    // Instantiate the full SEP Engine
    sep::quantum::manifold::QuantumManifoldOptimizationEngine engine;
    engine.initialize();



    // Convert candles to QuantumPatterns
    std::vector<sep::quantum::manifold::QuantumPattern> quantum_patterns;
    for (const auto& candle : candles) {
        sep::quantum::manifold::QuantumPattern q_p;
        q_p.id = "pattern_" + candle.time;
        q_p.coherence = 1.0 / (1.0 + candle.high - candle.low); // Simplified volatility
        q_p.stability = (candle.close - candle.open) * 10000; // Simplified stability
        q_p.phase = std::abs(candle.close - candle.open) / (candle.high - candle.low + 1e-6); // Simplified complexity, using phase as entropy
        quantum_patterns.push_back(q_p);
    }

    // Process the patterns through the engine
    engine.processPatterns(quantum_patterns);
    auto metrics = engine.getMetrics();
    
    std::vector<sep::quantum::Signal> signals;

    // --- START NEW LOGIC ---
    // Define your experimental thresholds here
    double sell_entropy_threshold = 0.7;
    double buy_stability_threshold = 0.6;
    double buy_coherence_threshold = 0.7;
    double buy_entropy_threshold = 0.3;

    double stability_w = 0.5;
    double coherence_w = 0.3;
    double entropy_w = 0.2;
    double buy_score_threshold = 0.55;
    double sell_score_threshold = 0.55;

    if (argc == 7) {
        stability_w = std::stod(argv[2]);
        coherence_w = std::stod(argv[3]);
        entropy_w = std::stod(argv[4]);
        buy_score_threshold = std::stod(argv[5]);
        sell_score_threshold = std::stod(argv[6]);
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

        // --- THIS IS YOUR OPTIMIZATION TARGET ---
        double buy_score = (metric.stability * stability_w) + (metric.coherence * coherence_w) + ((1.0 - metric.phase) * entropy_w);
        buy_score /= (stability_w + coherence_w + entropy_w);

        double sell_score = ((1.0 - metric.stability) * stability_w) + ((1.0 - metric.coherence) * coherence_w) + (metric.phase * entropy_w);
        sell_score /= (stability_w + coherence_w + entropy_w);

        if (buy_score > buy_score_threshold) {
            signal.type = sep::quantum::SignalType::BUY;
            signal.confidence = buy_score;
        } else if (sell_score > sell_score_threshold) {
            signal.type = sep::quantum::SignalType::SELL;
            signal.confidence = sell_score;
        } else {
            signal.type = sep::quantum::SignalType::HOLD;
            signal.confidence = 0.0;
        }
        // --- END OPTIMIZATION TARGET ---
        
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