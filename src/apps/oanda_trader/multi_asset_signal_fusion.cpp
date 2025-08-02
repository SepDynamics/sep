#include "multi_asset_signal_fusion.hpp"
#include <algorithm>
#include <numeric>
#include <cmath>
#include <spdlog/spdlog.h>
#include <fmt/format.h>

namespace sep {

MultiAssetSignalFusion::MultiAssetSignalFusion(
    std::shared_ptr<sep::trading::QuantumSignalBridge> quantum_processor,
    std::shared_ptr<sep::cache::EnhancedMarketModelCache> market_cache)
    : quantum_processor_(quantum_processor), market_cache_(market_cache) {
    
    last_correlation_update_ = std::chrono::system_clock::now() - std::chrono::hours(24);
    spdlog::info("MultiAssetSignalFusion initialized with {} major pairs", MAJOR_PAIRS.size());
}

FusedSignal MultiAssetSignalFusion::generateFusedSignal(const std::string& target_asset) {
    spdlog::debug("Generating fused signal for {}", target_asset);
    
    // Update correlation cache if needed (every hour)
    auto now = std::chrono::system_clock::now();
    if (now - last_correlation_update_ > std::chrono::hours(1)) {
        updateCorrelationCache();
        last_correlation_update_ = now;
    }
    
    // Get correlated assets for the target
    auto correlated_assets = getCorrelatedAssets(target_asset);
    std::vector<AssetSignal> asset_signals;
    
    // Generate quantum signals for each correlated asset
    for (const auto& asset : correlated_assets) {
        try {
            // Skip different asset pairs for now - focus on same asset analysis
            if (asset != target_asset) {
                continue;
            }
            
            auto correlation = calculateDynamicCorrelation(target_asset, asset);
            
            // Create placeholder quantum identifiers (will be replaced with real data)
            sep::trading::QuantumIdentifiers quantum_identifiers{
                .confidence = 0.7f,
                .coherence = 0.4f,
                .stability = 0.5f,
                .converged = true
            };
            
            AssetSignal signal{
                .instrument = asset,
                .quantum_identifiers = quantum_identifiers,
                .correlation_weight = 1.0,  // Perfect correlation with self
                .lag = correlation.optimal_lag,
                .confidence_modifier = calculateCrossAssetBoost(quantum_identifiers, correlation)
            };
            
            asset_signals.push_back(signal);
            spdlog::debug("Added signal for {} with correlation weight {:.3f}", 
                         asset, signal.correlation_weight);
            
        } catch (const std::exception& e) {
            spdlog::warn("Failed to process asset {}: {}", asset, e.what());
        }
    }
    
    // Fuse all the signals
    return fuseSignals(asset_signals);
}

std::vector<std::string> MultiAssetSignalFusion::getCorrelatedAssets(const std::string& target_asset) {
    std::vector<std::string> correlated;
    
    // Always include the target asset itself
    correlated.push_back(target_asset);
    
    // Add other major pairs
    for (const auto& pair : MAJOR_PAIRS) {
        if (pair != target_asset) {
            correlated.push_back(pair);
        }
    }
    
    return correlated;
}

CrossAssetCorrelation MultiAssetSignalFusion::calculateDynamicCorrelation(
    const std::string& asset1, 
    const std::string& asset2) {
    
    // Check cache first
    std::string cache_key = asset1 + "_" + asset2;
    auto it = correlation_cache_.find(cache_key);
    if (it != correlation_cache_.end()) {
        return it->second;
    }
    
    // Try reverse order
    std::string reverse_key = asset2 + "_" + asset1;
    it = correlation_cache_.find(reverse_key);
    if (it != correlation_cache_.end()) {
        auto corr = it->second;
        // Reverse correlation sign if needed
        if (asset1 != asset2) {
            corr.strength = -corr.strength;
        }
        return corr;
    }
    
    // Calculate new correlation if not in cache
    try {
        // For now, return a default correlation since we're simplifying for Phase 2
        // TODO: Implement proper historical data fetching through connector
        std::vector<Candle> data1, data2; // Empty for now
        
        if (data1.empty() || data2.empty()) {
            spdlog::warn("Insufficient data for correlation calculation: {} or {}", asset1, asset2);
            return {0.0, std::chrono::milliseconds(0), 0.0};
        }
        
        // Calculate price returns for correlation
        std::vector<double> returns1, returns2;
        for (size_t i = 1; i < data1.size() && i < data2.size(); ++i) {
            double return1 = (data1[i].close - data1[i-1].close) / data1[i-1].close;
            double return2 = (data2[i].close - data2[i-1].close) / data2[i-1].close;
            returns1.push_back(return1);
            returns2.push_back(return2);
        }
        
        // Calculate Pearson correlation coefficient
        double correlation = 0.0;
        if (returns1.size() > 10) {  // Need minimum data points
            double mean1 = std::accumulate(returns1.begin(), returns1.end(), 0.0) / returns1.size();
            double mean2 = std::accumulate(returns2.begin(), returns2.end(), 0.0) / returns2.size();
            
            double numerator = 0.0, denom1 = 0.0, denom2 = 0.0;
            for (size_t i = 0; i < returns1.size(); ++i) {
                double diff1 = returns1[i] - mean1;
                double diff2 = returns2[i] - mean2;
                numerator += diff1 * diff2;
                denom1 += diff1 * diff1;
                denom2 += diff2 * diff2;
            }
            
            if (denom1 > 0 && denom2 > 0) {
                correlation = numerator / std::sqrt(denom1 * denom2);
            }
        }
        
        CrossAssetCorrelation result{
            .strength = correlation,
            .optimal_lag = std::chrono::milliseconds(0),  // TODO: Implement lag optimization
            .stability = std::abs(correlation)  // Simple stability measure for now
        };
        
        // Cache the result
        correlation_cache_[cache_key] = result;
        spdlog::debug("Calculated correlation between {} and {}: {:.3f}", 
                     asset1, asset2, correlation);
        
        return result;
        
    } catch (const std::exception& e) {
        spdlog::error("Error calculating correlation between {} and {}: {}", 
                     asset1, asset2, e.what());
        return {0.0, std::chrono::milliseconds(0), 0.0};
    }
}

double MultiAssetSignalFusion::calculateCrossAssetBoost(
    const sep::trading::QuantumIdentifiers& signal, 
    const CrossAssetCorrelation& correlation) {
    
    // Base boost is proportional to correlation strength
    double base_boost = std::abs(correlation.strength) * 0.5;
    
    // Additional boost for high-confidence signals
    if (signal.confidence > 0.7) {
        base_boost *= 1.2;
    }
    
    // Additional boost for stable correlations
    if (correlation.stability > 0.6) {
        base_boost *= 1.1;
    }
    
    // Cap the boost to prevent over-amplification
    return std::min(base_boost, 0.3);
}

FusedSignal MultiAssetSignalFusion::fuseSignals(const std::vector<AssetSignal>& asset_signals) {
    if (asset_signals.empty()) {
        spdlog::warn("No asset signals to fuse");
        return {Direction::HOLD, 0.0, {}, 0.0, 0.0};
    }
    
    // Calculate weighted votes for BUY/SELL/HOLD
    double buy_weight = 0.0, sell_weight = 0.0, hold_weight = 0.0;
    double total_weight = 0.0;
    double total_confidence = 0.0;
    
    for (const auto& signal : asset_signals) {
        double weight = signal.correlation_weight * (1.0 + signal.confidence_modifier);
        total_weight += weight;
        total_confidence += signal.quantum_identifiers.confidence * weight;
        
        // Determine signal direction based on quantum identifiers
        if (signal.quantum_identifiers.confidence > 0.65) {
            if (signal.quantum_identifiers.stability < 0.5) {  // Low stability = BUY signal
                buy_weight += weight;
            } else {  // High stability = SELL signal
                sell_weight += weight;
            }
        } else {
            hold_weight += weight;
        }
    }
    
    // Normalize weights
    if (total_weight > 0) {
        buy_weight /= total_weight;
        sell_weight /= total_weight;
        hold_weight /= total_weight;
        total_confidence /= total_weight;
    }
    
    // Determine primary direction
    Direction primary_direction = Direction::HOLD;
    double signal_strength = 0.0;
    
    if (buy_weight > sell_weight && buy_weight > hold_weight) {
        primary_direction = Direction::BUY;
        signal_strength = buy_weight;
    } else if (sell_weight > buy_weight && sell_weight > hold_weight) {
        primary_direction = Direction::SELL;
        signal_strength = sell_weight;
    } else {
        primary_direction = Direction::HOLD;
        signal_strength = hold_weight;
    }
    
    // Calculate cross-asset coherence
    double coherence = calculateCrossAssetCoherence(asset_signals);
    
    // Fusion confidence combines individual confidence with coherence
    double fusion_confidence = total_confidence * (0.7 + 0.3 * coherence);
    
    FusedSignal result{
        .primary_direction = primary_direction,
        .fusion_confidence = fusion_confidence,
        .contributing_signals = asset_signals,
        .cross_asset_coherence = coherence,
        .signal_strength = signal_strength
    };
    
    logFusionDetails(result);
    return result;
}

double MultiAssetSignalFusion::calculateCrossAssetCoherence(const std::vector<AssetSignal>& signals) {
    if (signals.size() < 2) {
        return 1.0;  // Perfect coherence with only one signal
    }
    
    // Calculate agreement between signals
    int agreement_count = 0;
    int total_pairs = 0;
    
    for (size_t i = 0; i < signals.size(); ++i) {
        for (size_t j = i + 1; j < signals.size(); ++j) {
            total_pairs++;
            
            // Check if signals agree on direction
            bool signal_i_buy = signals[i].quantum_identifiers.confidence > 0.65 && 
                               signals[i].quantum_identifiers.stability < 0.5;
            bool signal_j_buy = signals[j].quantum_identifiers.confidence > 0.65 && 
                               signals[j].quantum_identifiers.stability < 0.5;
            
            bool signal_i_sell = signals[i].quantum_identifiers.confidence > 0.65 && 
                                signals[i].quantum_identifiers.stability >= 0.5;
            bool signal_j_sell = signals[j].quantum_identifiers.confidence > 0.65 && 
                                signals[j].quantum_identifiers.stability >= 0.5;
            
            // Agreement if both signals point in same direction or both are neutral
            if ((signal_i_buy && signal_j_buy) || 
                (signal_i_sell && signal_j_sell) ||
                (!signal_i_buy && !signal_i_sell && !signal_j_buy && !signal_j_sell)) {
                agreement_count++;
            }
        }
    }
    
    return total_pairs > 0 ? static_cast<double>(agreement_count) / total_pairs : 1.0;
}

void MultiAssetSignalFusion::updateCorrelationCache() {
    spdlog::info("Updating correlation cache");
    correlation_cache_.clear();
    
    // Pre-calculate correlations for major pairs
    for (size_t i = 0; i < MAJOR_PAIRS.size(); ++i) {
        for (size_t j = i + 1; j < MAJOR_PAIRS.size(); ++j) {
            calculateDynamicCorrelation(MAJOR_PAIRS[i], MAJOR_PAIRS[j]);
        }
    }
    
    spdlog::info("Correlation cache updated with {} entries", correlation_cache_.size());
}

void MultiAssetSignalFusion::invalidateCorrelationCache() {
    correlation_cache_.clear();
    last_correlation_update_ = std::chrono::system_clock::now() - std::chrono::hours(24);
    spdlog::info("Correlation cache invalidated");
}

void MultiAssetSignalFusion::logFusionDetails(const FusedSignal& signal) {
    std::string direction_str = 
        (signal.primary_direction == Direction::BUY) ? "BUY" :
        (signal.primary_direction == Direction::SELL) ? "SELL" : "HOLD";
    
    spdlog::info("🔗 FUSED SIGNAL: {} | Confidence: {:.3f} | Coherence: {:.3f} | Strength: {:.3f} | Assets: {}", 
                direction_str, signal.fusion_confidence, signal.cross_asset_coherence, 
                signal.signal_strength, signal.contributing_signals.size());
    
    for (const auto& contrib : signal.contributing_signals) {
        spdlog::debug("  └─ {}: conf={:.3f}, weight={:.3f}, boost={:.3f}", 
                     contrib.instrument, contrib.quantum_identifiers.confidence,
                     contrib.correlation_weight, contrib.confidence_modifier);
    }
}

std::string MultiAssetSignalFusion::serializeFusionResult(const FusedSignal& signal) {
    std::string direction_str = 
        (signal.primary_direction == Direction::BUY) ? "BUY" :
        (signal.primary_direction == Direction::SELL) ? "SELL" : "HOLD";
    
    return fmt::format("{{ \"direction\": \"{}\", \"confidence\": {:.3f}, \"coherence\": {:.3f}, \"strength\": {:.3f}, \"assets\": {} }}",
                      direction_str, signal.fusion_confidence, signal.cross_asset_coherence,
                      signal.signal_strength, signal.contributing_signals.size());
}

} // namespace sep
