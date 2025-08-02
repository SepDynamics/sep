#include "weekend_optimizer.hpp"
#include "market_utils.hpp"
#include <fstream>
#include <sstream>
#include <filesystem>
#include <regex>
#include <iostream>
#include <iomanip>
#include <nlohmann/json.hpp>

namespace SEP {

bool WeekendOptimizer::runWeekendOptimization() {
    std::cout << "[WEEKEND_OPTIMIZER] Starting weekend optimization cycle..." << std::endl;
    
    // Step 1: Parse all available log files
    if (!parseLogFiles()) {
        std::cerr << "[WEEKEND_OPTIMIZER] Failed to parse log files" << std::endl;
        return false;
    }
    
    std::cout << "[WEEKEND_OPTIMIZER] Parsed " << trade_history_.size() 
              << " trade records from live logs" << std::endl;
    
    // Step 2: Load current configuration
    current_config_ = loadOptimalConfig();
    
    // Step 3: Run optimization
    OptimalConfig new_config = optimizeParameters();
    
    // Step 4: Save improved configuration
    if (new_config.profitability_score > current_config_.profitability_score) {
        std::cout << "[WEEKEND_OPTIMIZER] Found improved configuration!" << std::endl;
        std::cout << "[WEEKEND_OPTIMIZER] Old score: " << current_config_.profitability_score << std::endl;
        std::cout << "[WEEKEND_OPTIMIZER] New score: " << new_config.profitability_score << std::endl;
        
        if (saveOptimalConfig(new_config)) {
            current_config_ = new_config;
            std::cout << "[WEEKEND_OPTIMIZER] Configuration saved successfully" << std::endl;
            return true;
        }
    } else {
        std::cout << "[WEEKEND_OPTIMIZER] Current configuration remains optimal" << std::endl;
        std::cout << "[WEEKEND_OPTIMIZER] Score: " << current_config_.profitability_score << std::endl;
    }
    
    return true;
}

bool WeekendOptimizer::parseLogFiles() {
    trade_history_.clear();
    
    if (!std::filesystem::exists(live_results_dir_)) {
        std::cout << "[WEEKEND_OPTIMIZER] Live results directory not found: " 
                  << live_results_dir_ << std::endl;
        return false;
    }
    
    int files_parsed = 0;
    for (const auto& entry : std::filesystem::directory_iterator(live_results_dir_)) {
        if (entry.path().extension() == ".log") {
            auto trades = parseLogFile(entry.path().string());
            trade_history_.insert(trade_history_.end(), trades.begin(), trades.end());
            files_parsed++;
        }
    }
    
    std::cout << "[WEEKEND_OPTIMIZER] Parsed " << files_parsed << " log files" << std::endl;
    return files_parsed > 0;
}

std::vector<WeekendOptimizer::TradeResult> WeekendOptimizer::parseLogFile(const std::string& filepath) {
    std::vector<TradeResult> trades;
    std::ifstream file(filepath);
    std::string line;
    
    // Regex patterns for log parsing
    std::regex signal_pattern(R"(\[SIGNAL_GENERATED\] (.+?) (BUY|SELL) \| Conf:([\d.]+) Coh:([\d.]+) Stab:([\d.]+))");
    std::regex triple_pattern(R"(\[TRIPLE_CONFIRMED\] (.+?) (BUY|SELL) signal confirmed)");
    std::regex threshold_pattern(R"(\[THRESHOLD_FAIL\] (.+?) (BUY|SELL) \| Conf:([\d.]+) < ([\d.]+) or Coh:([\d.]+) < ([\d.]+))");
    std::regex trade_success_pattern(R"(\[TRADE_SUCCESS\] (.+?) (BUY|SELL) .* P/L: ([+-]?[\d.]+))");
    
    while (std::getline(file, line)) {
        std::smatch match;
        
        // Parse signal generation
        if (std::regex_search(line, match, signal_pattern)) {
            TradeResult trade;
            trade.pair = match[1].str();
            trade.direction = match[2].str();
            trade.confidence = std::stod(match[3].str());
            trade.coherence = std::stod(match[4].str());
            trade.stability = std::stod(match[5].str());
            trade.executed = false;
            trade.profitable = false;
            trade.pnl = 0.0;
            trade.timestamp = filepath; // Simplified - could extract from line
            
            trades.push_back(trade);
        }
        
        // Mark trades as executed if they appear in triple confirmation
        else if (std::regex_search(line, match, triple_pattern)) {
            for (auto& trade : trades) {
                if (trade.pair == match[1].str() && trade.direction == match[2].str() && !trade.executed) {
                    trade.executed = true;
                    trade.reason = "TRIPLE_CONFIRMED";
                    break;
                }
            }
        }
        
        // Parse trade outcomes
        else if (std::regex_search(line, match, trade_success_pattern)) {
            for (auto& trade : trades) {
                if (trade.pair == match[1].str() && trade.direction == match[2].str() && trade.executed) {
                    trade.pnl = std::stod(match[3].str());
                    trade.profitable = trade.pnl > 0;
                    break;
                }
            }
        }
    }
    
    return trades;
}

WeekendOptimizer::OptimalConfig WeekendOptimizer::loadOptimalConfig() {
    OptimalConfig config; // Default values
    
    if (!std::filesystem::exists(config_file_path_)) {
        std::cout << "[WEEKEND_OPTIMIZER] No existing config found, using defaults" << std::endl;
        return config;
    }
    
    try {
        std::ifstream file(config_file_path_);
        nlohmann::json j;
        file >> j;
        
        config.stability_weight = j.value("stability_weight", 0.4);
        config.coherence_weight = j.value("coherence_weight", 0.1);
        config.entropy_weight = j.value("entropy_weight", 0.5);
        config.confidence_threshold = j.value("confidence_threshold", 0.65);
        config.coherence_threshold = j.value("coherence_threshold", 0.30);
        config.accuracy = j.value("accuracy", 0.0);
        config.signal_rate = j.value("signal_rate", 0.0);
        config.profitability_score = j.value("profitability_score", 0.0);
        
        std::cout << "[WEEKEND_OPTIMIZER] Loaded existing config with score: " 
                  << config.profitability_score << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "[WEEKEND_OPTIMIZER] Error loading config: " << e.what() << std::endl;
    }
    
    return config;
}

bool WeekendOptimizer::saveOptimalConfig(const OptimalConfig& config) {
    try {
        nlohmann::json j;
        j["stability_weight"] = config.stability_weight;
        j["coherence_weight"] = config.coherence_weight;
        j["entropy_weight"] = config.entropy_weight;
        j["confidence_threshold"] = config.confidence_threshold;
        j["coherence_threshold"] = config.coherence_threshold;
        j["accuracy"] = config.accuracy;
        j["signal_rate"] = config.signal_rate;
        j["profitability_score"] = config.profitability_score;
        j["optimization_date"] = std::time(nullptr);
        
        std::ofstream file(config_file_path_);
        file << j.dump(4);
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "[WEEKEND_OPTIMIZER] Error saving config: " << e.what() << std::endl;
        return false;
    }
}

WeekendOptimizer::OptimalConfig WeekendOptimizer::optimizeParameters() {
    OptimalConfig best_config = current_config_;
    double best_score = calculateProfitabilityScore(current_config_);
    
    std::cout << "[WEEKEND_OPTIMIZER] Testing parameter combinations..." << std::endl;
    
    // Test weight combinations (simplified grid search)
    std::vector<std::array<double, 3>> weight_combinations = {
        {0.4, 0.1, 0.5}, // Current best
        {0.3, 0.2, 0.5}, // More coherence
        {0.5, 0.1, 0.4}, // More stability
        {0.2, 0.1, 0.7}, // More entropy
        {0.1, 0.3, 0.6}, // Coherence focus
        {0.6, 0.1, 0.3}  // Stability focus
    };
    
    // Test threshold combinations
    std::vector<std::array<double, 2>> threshold_combinations = {
        {0.65, 0.30}, // Current best
        {0.70, 0.25}, // Higher confidence
        {0.60, 0.35}, // Lower confidence, higher coherence
        {0.75, 0.20}, // Very high confidence
        {0.55, 0.40}  // Lower thresholds
    };
    
    for (const auto& weights : weight_combinations) {
        for (const auto& thresholds : threshold_combinations) {
            OptimalConfig test_config;
            test_config.stability_weight = weights[0];
            test_config.coherence_weight = weights[1];
            test_config.entropy_weight = weights[2];
            test_config.confidence_threshold = thresholds[0];
            test_config.coherence_threshold = thresholds[1];
            
            double score = calculateProfitabilityScore(test_config);
            
            if (score > best_score) {
                best_score = score;
                best_config = test_config;
                best_config.profitability_score = score;
                
                std::cout << "[WEEKEND_OPTIMIZER] New best: S=" << weights[0] 
                          << " C=" << weights[1] << " E=" << weights[2]
                          << " | CT=" << thresholds[0] << " CoT=" << thresholds[1]
                          << " | Score=" << score << std::endl;
            }
        }
    }
    
    return best_config;
}

double WeekendOptimizer::calculateProfitabilityScore(const OptimalConfig& config) {
    if (trade_history_.empty()) {
        return 0.0;
    }
    
    int high_confidence_trades = 0;
    int profitable_trades = 0;
    int total_executed = 0;
    
    for (const auto& trade : trade_history_) {
        // Simulate if this trade would meet the new thresholds
        if (trade.confidence >= config.confidence_threshold && 
            trade.coherence >= config.coherence_threshold) {
            high_confidence_trades++;
            
            if (trade.executed) {
                total_executed++;
                if (trade.profitable) {
                    profitable_trades++;
                }
            }
        }
    }
    
    if (high_confidence_trades == 0) {
        return 0.0;
    }
    
    double signal_rate = static_cast<double>(high_confidence_trades) / trade_history_.size();
    double accuracy = total_executed > 0 ? 
        static_cast<double>(profitable_trades) / total_executed : 0.0;
    
    // Profitability score: (accuracy - 50%) * signal_rate * 1000
    double score = (accuracy - 0.5) * signal_rate * 1000;
    
    return std::max(0.0, score);
}

} // namespace SEP
