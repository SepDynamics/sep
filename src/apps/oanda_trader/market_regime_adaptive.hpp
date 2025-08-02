#pragma once

#include <vector>
#include <string>
#include <chrono>
#include <memory>
#include "enhanced_market_model_cache.hpp"

namespace sep {

enum class VolatilityLevel {
    Low,
    Medium, 
    High
};

enum class TrendStrength {
    Ranging,
    Weak,
    Strong
};

enum class LiquidityLevel {
    Low,
    Medium,
    High
};

enum class NewsImpactLevel {
    None,
    Low,
    Medium,
    High
};

enum class QuantumCoherenceLevel {
    Low,
    Medium,
    High
};

struct MarketRegime {
    VolatilityLevel volatility;
    TrendStrength trend;
    LiquidityLevel liquidity;
    NewsImpactLevel news_impact;
    QuantumCoherenceLevel q_coherence;
    double regime_confidence;  // How confident we are in this regime classification
};

struct AdaptiveThresholds {
    double confidence_threshold;     // Base: 0.65, adapted ±0.15
    double coherence_threshold;      // Base: 0.30, adapted ±0.20
    double stability_requirement;    // Additional stability requirement
    double signal_frequency_modifier; // Increase/decrease signal rate
    std::string regime_description;   // Human-readable regime description
};

class MarketRegimeAdaptiveProcessor {
private:
    std::shared_ptr<sep::cache::EnhancedMarketModelCache> market_cache_;
    
    // Base optimized thresholds from current 60.73% accuracy system
    static constexpr double BASE_CONFIDENCE_THRESHOLD = 0.65;
    static constexpr double BASE_COHERENCE_THRESHOLD = 0.30;
    static constexpr double BASE_STABILITY_REQUIREMENT = 0.5;
    static constexpr double BASE_SIGNAL_FREQUENCY = 0.191; // 19.1% signal rate
    
    // Volatility calculation parameters
    static constexpr int VOLATILITY_PERIODS = 24;  // 24 periods for volatility calculation
    static constexpr double HIGH_VOLATILITY_THRESHOLD = 0.008;  // 0.8% per period
    static constexpr double LOW_VOLATILITY_THRESHOLD = 0.003;   // 0.3% per period
    
    // Trend calculation parameters
    static constexpr int TREND_PERIODS = 48;  // 48 periods for trend analysis
    static constexpr double STRONG_TREND_THRESHOLD = 0.015;  // 1.5% total move
    static constexpr double WEAK_TREND_THRESHOLD = 0.005;   // 0.5% total move
    
    // Cache for regime calculations
    MarketRegime last_regime_;
    std::chrono::system_clock::time_point last_regime_update_;
    
public:
    MarketRegimeAdaptiveProcessor(std::shared_ptr<sep::cache::EnhancedMarketModelCache> market_cache);
    
    // Main interface
    AdaptiveThresholds calculateRegimeOptimalThresholds(const std::string& asset);
    MarketRegime detectCurrentRegime(const std::string& asset);
    
    // Regime detection components
    VolatilityLevel calculateVolatilityLevel(const std::vector<Candle>& data);
    TrendStrength calculateTrendStrength(const std::vector<Candle>& data);
    LiquidityLevel calculateLiquidityLevel(const std::string& asset);
    NewsImpactLevel calculateNewsImpact();
    QuantumCoherenceLevel calculateQuantumCoherence(const std::vector<Candle>& data);
    
    // Threshold adaptation logic
    AdaptiveThresholds adaptThresholdsForRegime(const MarketRegime& regime);
    double calculateVolatilityAdjustment(VolatilityLevel volatility);
    double calculateTrendAdjustment(TrendStrength trend);
    double calculateLiquidityAdjustment(LiquidityLevel liquidity);
    double calculateNewsAdjustment(NewsImpactLevel news);
    double calculateCoherenceAdjustment(QuantumCoherenceLevel coherence);
    
    // Utility functions
    double calculateATR(const std::vector<Candle>& data, int periods = 14);
    double calculateRSI(const std::vector<Candle>& data, int periods = 14);
    double calculateSMA(const std::vector<Candle>& data, int periods);
    bool isLondonSession();
    bool isNewYorkSession();
    bool isTokyoSession();
    
    // Debugging and monitoring
    void logRegimeDetails(const MarketRegime& regime, const AdaptiveThresholds& thresholds);
    std::string serializeRegimeData(const MarketRegime& regime, const AdaptiveThresholds& thresholds);
    
    // Performance tracking
    void updateRegimeCache(const std::string& asset);
    void invalidateRegimeCache();
};

} // namespace sep
