#include "market_regime_adaptive.hpp"
#include <algorithm>
#include <numeric>
#include <cmath>
#include <spdlog/spdlog.h>
#include <fmt/format.h>

namespace sep {

MarketRegimeAdaptiveProcessor::MarketRegimeAdaptiveProcessor(
    std::shared_ptr<sep::cache::EnhancedMarketModelCache> market_cache)
    : market_cache_(market_cache) {
    
    last_regime_update_ = std::chrono::system_clock::now() - std::chrono::hours(24);
    spdlog::info("MarketRegimeAdaptiveProcessor initialized");
}

AdaptiveThresholds MarketRegimeAdaptiveProcessor::calculateRegimeOptimalThresholds(const std::string& asset) {
    // Update regime detection if needed (every 15 minutes)
    auto now = std::chrono::system_clock::now();
    if (now - last_regime_update_ > std::chrono::minutes(15)) {
        updateRegimeCache(asset);
        last_regime_update_ = now;
    }
    
    // Detect current market regime
    auto regime = detectCurrentRegime(asset);
    
    // Adapt thresholds based on regime
    auto thresholds = adaptThresholdsForRegime(regime);
    
    logRegimeDetails(regime, thresholds);
    return thresholds;
}

MarketRegime MarketRegimeAdaptiveProcessor::detectCurrentRegime(const std::string& asset) {
    try {
        // For Phase 2 implementation, use simplified regime detection
        // TODO: Implement proper historical data fetching
        std::vector<Candle> m15_data; // Empty for now - will trigger default regime
        
        if (m15_data.size() < 50) {
            spdlog::warn("Insufficient data for regime detection for {}", asset);
            // Return conservative default regime
            return {
                VolatilityLevel::Medium,
                TrendStrength::Ranging,
                LiquidityLevel::Medium,
                NewsImpactLevel::Low,
                QuantumCoherenceLevel::Medium,
                0.5
            };
        }
        
        // Calculate regime components
        auto volatility = calculateVolatilityLevel(m15_data);
        auto trend = calculateTrendStrength(m15_data);
        auto liquidity = calculateLiquidityLevel(asset);
        auto news = calculateNewsImpact();
        auto coherence = calculateQuantumCoherence(m15_data);
        
        // Calculate overall regime confidence based on data quality
        double regime_confidence = std::min(1.0, static_cast<double>(m15_data.size()) / 100.0);
        
        MarketRegime regime{
            .volatility = volatility,
            .trend = trend,
            .liquidity = liquidity,
            .news_impact = news,
            .q_coherence = coherence,
            .regime_confidence = regime_confidence
        };
        
        last_regime_ = regime;
        return regime;
        
    } catch (const std::exception& e) {
        spdlog::error("Error detecting market regime for {}: {}", asset, e.what());
        return last_regime_;  // Return cached regime if available
    }
}

VolatilityLevel MarketRegimeAdaptiveProcessor::calculateVolatilityLevel(const std::vector<Candle>& data) {
    if (data.size() < VOLATILITY_PERIODS) {
        return VolatilityLevel::Medium;
    }
    
    // Calculate ATR-based volatility
    double atr = calculateATR(data, VOLATILITY_PERIODS);
    
    // Normalize ATR by current price
    double current_price = data.back().close;
    double normalized_volatility = atr / current_price;
    
    if (normalized_volatility > HIGH_VOLATILITY_THRESHOLD) {
        return VolatilityLevel::High;
    } else if (normalized_volatility < LOW_VOLATILITY_THRESHOLD) {
        return VolatilityLevel::Low;
    } else {
        return VolatilityLevel::Medium;
    }
}

TrendStrength MarketRegimeAdaptiveProcessor::calculateTrendStrength(const std::vector<Candle>& data) {
    if (data.size() < TREND_PERIODS) {
        return TrendStrength::Ranging;
    }
    
    // Calculate price change over trend periods
    double start_price = data[data.size() - TREND_PERIODS].close;
    double end_price = data.back().close;
    double total_change = std::abs(end_price - start_price) / start_price;
    
    // Calculate trend consistency using SMA slope
    double sma_start = calculateSMA(std::vector<Candle>(
        data.end() - TREND_PERIODS, data.end() - TREND_PERIODS + 10), 10);
    double sma_end = calculateSMA(std::vector<Candle>(
        data.end() - 10, data.end()), 10);
    double sma_change = std::abs(sma_end - sma_start) / sma_start;
    
    // Combine total change and trend consistency
    double trend_strength = (total_change + sma_change) / 2.0;
    
    if (trend_strength > STRONG_TREND_THRESHOLD) {
        return TrendStrength::Strong;
    } else if (trend_strength > WEAK_TREND_THRESHOLD) {
        return TrendStrength::Weak;
    } else {
        return TrendStrength::Ranging;
    }
}

LiquidityLevel MarketRegimeAdaptiveProcessor::calculateLiquidityLevel(const std::string& asset) {
    // Simple session-based liquidity estimation
    bool london = isLondonSession();
    bool new_york = isNewYorkSession();
    bool tokyo = isTokyoSession();
    
    // High liquidity during major session overlaps
    if ((london && new_york) || (london && tokyo && asset.find("EUR") != std::string::npos)) {
        return LiquidityLevel::High;
    }
    // Medium liquidity during single major sessions
    else if (london || new_york || (tokyo && asset.find("JPY") != std::string::npos)) {
        return LiquidityLevel::Medium;
    }
    // Low liquidity otherwise
    else {
        return LiquidityLevel::Low;
    }
}

NewsImpactLevel MarketRegimeAdaptiveProcessor::calculateNewsImpact() {
    // TODO: Integrate with economic calendar API
    // For now, return Low as baseline
    return NewsImpactLevel::Low;
}

QuantumCoherenceLevel MarketRegimeAdaptiveProcessor::calculateQuantumCoherence(const std::vector<Candle>& data) {
    if (data.size() < 20) {
        return QuantumCoherenceLevel::Medium;
    }
    
    // Calculate price coherence using correlation with moving averages
    std::vector<double> prices;
    std::vector<double> sma_20;
    
    for (size_t i = 19; i < data.size(); ++i) {
        prices.push_back(data[i].close);
        
        double sma = 0.0;
        for (size_t j = i - 19; j <= i; ++j) {
            sma += data[j].close;
        }
        sma /= 20.0;
        sma_20.push_back(sma);
    }
    
    // Calculate correlation coefficient
    if (prices.size() != sma_20.size() || prices.size() < 10) {
        return QuantumCoherenceLevel::Medium;
    }
    
    double mean_price = std::accumulate(prices.begin(), prices.end(), 0.0) / prices.size();
    double mean_sma = std::accumulate(sma_20.begin(), sma_20.end(), 0.0) / sma_20.size();
    
    double numerator = 0.0, denom1 = 0.0, denom2 = 0.0;
    for (size_t i = 0; i < prices.size(); ++i) {
        double diff_price = prices[i] - mean_price;
        double diff_sma = sma_20[i] - mean_sma;
        numerator += diff_price * diff_sma;
        denom1 += diff_price * diff_price;
        denom2 += diff_sma * diff_sma;
    }
    
    double correlation = 0.0;
    if (denom1 > 0 && denom2 > 0) {
        correlation = std::abs(numerator / std::sqrt(denom1 * denom2));
    }
    
    if (correlation > 0.8) {
        return QuantumCoherenceLevel::High;
    } else if (correlation > 0.5) {
        return QuantumCoherenceLevel::Medium;
    } else {
        return QuantumCoherenceLevel::Low;
    }
}

AdaptiveThresholds MarketRegimeAdaptiveProcessor::adaptThresholdsForRegime(const MarketRegime& regime) {
    AdaptiveThresholds thresholds{
        .confidence_threshold = BASE_CONFIDENCE_THRESHOLD,
        .coherence_threshold = BASE_COHERENCE_THRESHOLD,
        .stability_requirement = BASE_STABILITY_REQUIREMENT,
        .signal_frequency_modifier = 1.0,
        .regime_description = ""
    };
    
    // Apply volatility adjustments
    double vol_adj = calculateVolatilityAdjustment(regime.volatility);
    thresholds.confidence_threshold += vol_adj;
    
    // Apply trend adjustments  
    double trend_adj = calculateTrendAdjustment(regime.trend);
    thresholds.coherence_threshold += trend_adj;
    
    // Apply liquidity adjustments
    double liq_adj = calculateLiquidityAdjustment(regime.liquidity);
    thresholds.confidence_threshold += liq_adj * 0.5;
    thresholds.signal_frequency_modifier += liq_adj;
    
    // Apply news adjustments
    double news_adj = calculateNewsAdjustment(regime.news_impact);
    thresholds.confidence_threshold += news_adj;
    
    // Apply quantum coherence adjustments
    double coh_adj = calculateCoherenceAdjustment(regime.q_coherence);
    thresholds.coherence_threshold += coh_adj;
    
    // Ensure thresholds stay within reasonable bounds
    thresholds.confidence_threshold = std::max(0.5, std::min(0.8, thresholds.confidence_threshold));
    thresholds.coherence_threshold = std::max(0.1, std::min(0.5, thresholds.coherence_threshold));
    thresholds.stability_requirement = std::max(0.3, std::min(0.8, thresholds.stability_requirement));
    thresholds.signal_frequency_modifier = std::max(0.5, std::min(2.0, thresholds.signal_frequency_modifier));
    
    // Create human-readable description
    std::string vol_str = (regime.volatility == VolatilityLevel::High) ? "High-Vol" :
                         (regime.volatility == VolatilityLevel::Low) ? "Low-Vol" : "Med-Vol";
    std::string trend_str = (regime.trend == TrendStrength::Strong) ? "Strong-Trend" :
                           (regime.trend == TrendStrength::Weak) ? "Weak-Trend" : "Ranging";
    std::string liq_str = (regime.liquidity == LiquidityLevel::High) ? "High-Liq" :
                         (regime.liquidity == LiquidityLevel::Low) ? "Low-Liq" : "Med-Liq";
    
    thresholds.regime_description = vol_str + "_" + trend_str + "_" + liq_str;
    
    return thresholds;
}

double MarketRegimeAdaptiveProcessor::calculateVolatilityAdjustment(VolatilityLevel volatility) {
    switch (volatility) {
        case VolatilityLevel::High:
            return 0.10;  // Increase confidence threshold in high volatility
        case VolatilityLevel::Low:
            return -0.05; // Decrease confidence threshold in low volatility
        case VolatilityLevel::Medium:
        default:
            return 0.0;
    }
}

double MarketRegimeAdaptiveProcessor::calculateTrendAdjustment(TrendStrength trend) {
    switch (trend) {
        case TrendStrength::Strong:
            return -0.10; // Lower coherence thresholds for trend-following
        case TrendStrength::Weak:
            return 0.05;  // Slightly higher coherence requirements
        case TrendStrength::Ranging:
        default:
            return 0.0;
    }
}

double MarketRegimeAdaptiveProcessor::calculateLiquidityAdjustment(LiquidityLevel liquidity) {
    switch (liquidity) {
        case LiquidityLevel::Low:
            return 0.1;   // Increase thresholds significantly in low liquidity
        case LiquidityLevel::High:
            return -0.05; // Decrease thresholds slightly in high liquidity
        case LiquidityLevel::Medium:
        default:
            return 0.0;
    }
}

double MarketRegimeAdaptiveProcessor::calculateNewsAdjustment(NewsImpactLevel news) {
    switch (news) {
        case NewsImpactLevel::High:
            return 0.15;  // Much higher confidence required during news
        case NewsImpactLevel::Medium:
            return 0.08;
        case NewsImpactLevel::Low:
            return 0.02;
        case NewsImpactLevel::None:
        default:
            return 0.0;
    }
}

double MarketRegimeAdaptiveProcessor::calculateCoherenceAdjustment(QuantumCoherenceLevel coherence) {
    switch (coherence) {
        case QuantumCoherenceLevel::High:
            return -0.05; // Lower coherence thresholds when market is coherent
        case QuantumCoherenceLevel::Low:
            return 0.10;  // Higher coherence thresholds when market is chaotic
        case QuantumCoherenceLevel::Medium:
        default:
            return 0.0;
    }
}

double MarketRegimeAdaptiveProcessor::calculateATR(const std::vector<Candle>& data, int periods) {
    if (data.size() < static_cast<size_t>(periods + 1)) {
        return 0.0;
    }
    
    std::vector<double> true_ranges;
    for (size_t i = 1; i < data.size() && true_ranges.size() < static_cast<size_t>(periods); ++i) {
        double high_low = data[i].high - data[i].low;
        double high_close = std::abs(data[i].high - data[i-1].close);
        double low_close = std::abs(data[i].low - data[i-1].close);
        
        double true_range = std::max({high_low, high_close, low_close});
        true_ranges.push_back(true_range);
    }
    
    return std::accumulate(true_ranges.begin(), true_ranges.end(), 0.0) / true_ranges.size();
}

double MarketRegimeAdaptiveProcessor::calculateSMA(const std::vector<Candle>& data, int periods) {
    if (data.size() < static_cast<size_t>(periods)) {
        return 0.0;
    }
    
    double sum = 0.0;
    for (size_t i = data.size() - periods; i < data.size(); ++i) {
        sum += data[i].close;
    }
    
    return sum / periods;
}

bool MarketRegimeAdaptiveProcessor::isLondonSession() {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto utc_tm = *std::gmtime(&time_t);
    
    int hour = utc_tm.tm_hour;
    // London session: 8:00 - 17:00 UTC (approximately)
    return hour >= 8 && hour < 17;
}

bool MarketRegimeAdaptiveProcessor::isNewYorkSession() {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto utc_tm = *std::gmtime(&time_t);
    
    int hour = utc_tm.tm_hour;
    // New York session: 13:00 - 22:00 UTC (approximately)
    return hour >= 13 && hour < 22;
}

bool MarketRegimeAdaptiveProcessor::isTokyoSession() {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto utc_tm = *std::gmtime(&time_t);
    
    int hour = utc_tm.tm_hour;
    // Tokyo session: 00:00 - 09:00 UTC (approximately)
    return hour >= 0 && hour < 9;
}

void MarketRegimeAdaptiveProcessor::logRegimeDetails(const MarketRegime& regime, const AdaptiveThresholds& thresholds) {
    spdlog::info("📊 MARKET REGIME: {} | Conf:{:.3f} Coh:{:.3f} Freq:{:.3f}", 
                thresholds.regime_description,
                thresholds.confidence_threshold,
                thresholds.coherence_threshold,
                thresholds.signal_frequency_modifier);
    
    spdlog::debug("  Volatility: {} | Trend: {} | Liquidity: {} | Confidence: {:.2f}",
                 static_cast<int>(regime.volatility),
                 static_cast<int>(regime.trend),
                 static_cast<int>(regime.liquidity),
                 regime.regime_confidence);
}

std::string MarketRegimeAdaptiveProcessor::serializeRegimeData(const MarketRegime& regime, const AdaptiveThresholds& thresholds) {
    return fmt::format("{{ \"regime\": \"{}\", \"confidence_threshold\": {:.3f}, \"coherence_threshold\": {:.3f}, \"frequency_modifier\": {:.3f} }}",
                      thresholds.regime_description,
                      thresholds.confidence_threshold,
                      thresholds.coherence_threshold,
                      thresholds.signal_frequency_modifier);
}

void MarketRegimeAdaptiveProcessor::updateRegimeCache(const std::string& asset) {
    try {
        last_regime_ = detectCurrentRegime(asset);
        spdlog::debug("Updated regime cache for {}", asset);
    } catch (const std::exception& e) {
        spdlog::error("Failed to update regime cache for {}: {}", asset, e.what());
    }
}

void MarketRegimeAdaptiveProcessor::invalidateRegimeCache() {
    last_regime_update_ = std::chrono::system_clock::now() - std::chrono::hours(24);
    spdlog::info("Market regime cache invalidated");
}

} // namespace sep
