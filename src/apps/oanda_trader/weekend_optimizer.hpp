#pragma once

#include <string>
#include <vector>
#include <map>

namespace SEP {

/**
 * Weekend Optimizer - Self-Improving Trading System
 * Analyzes live trading logs during market closure to optimize parameters
 * for the upcoming week. Creates a continuous learning feedback loop.
 */
class WeekendOptimizer {
public:
    struct TradeResult {
        std::string timestamp;
        std::string pair;
        std::string direction;
        double confidence;
        double coherence; 
        double stability;
        bool executed;
        bool profitable;  // Will be calculated from price data
        double pnl;       // Profit/Loss if trade was executed
        std::string reason; // Why trade was/wasn't executed
    };
    
    struct OptimalConfig {
        double stability_weight = 0.4;
        double coherence_weight = 0.1;
        double entropy_weight = 0.5;
        double confidence_threshold = 0.65;
        double coherence_threshold = 0.30;
        double accuracy = 0.0;
        double signal_rate = 0.0;
        double profitability_score = 0.0;
    };

private:
    std::vector<TradeResult> trade_history_;
    OptimalConfig current_config_;
    std::string config_file_path_;
    std::string live_results_dir_;

public:
    explicit WeekendOptimizer(const std::string& config_path = "/sep/optimal_config.json",
                             const std::string& results_dir = "/sep/live_results/")
        : config_file_path_(config_path), live_results_dir_(results_dir) {}
    
    /**
     * Main entry point for weekend optimization
     * Called when markets are closed to analyze and optimize
     */
    bool runWeekendOptimization();
    
    /**
     * Parse all log files from the live_results directory
     * Extract trade attempts, successes, and outcomes
     */
    bool parseLogFiles();
    
    /**
     * Load the current optimal configuration from disk
     * Returns default config if file doesn't exist
     */
    OptimalConfig loadOptimalConfig();
    
    /**
     * Save the optimized configuration to disk
     */
    bool saveOptimalConfig(const OptimalConfig& config);
    
    /**
     * Run systematic optimization on the parsed trade data
     * Tests different weight and threshold combinations
     */
    OptimalConfig optimizeParameters();
    
    /**
     * Calculate profitability score for a given configuration
     * Uses the parsed trade results to simulate performance
     */
    double calculateProfitabilityScore(const OptimalConfig& config);
    
    /**
     * Get the current optimal configuration
     */
    const OptimalConfig& getCurrentConfig() const { return current_config_; }

private:
    /**
     * Parse a single log file and extract trade information
     */
    std::vector<TradeResult> parseLogFile(const std::string& filepath);
    
    /**
     * Determine if a hypothetical trade would have been profitable
     * Requires fetching historical price data for the trade period
     */
    bool calculateTradeOutcome(TradeResult& trade);
    
    /**
     * Extract metric values from a log line
     */
    bool extractMetrics(const std::string& line, double& confidence, 
                       double& coherence, double& stability);
};

} // namespace SEP
