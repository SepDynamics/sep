#pragma once

#include <vector>
#include <string>
#include <chrono>
#include <memory>
#include <unordered_map>
#include "quantum_signal_bridge.hpp"
#include "enhanced_market_model_cache.hpp"

namespace sep {

enum class Direction {
    BUY,
    SELL,
    HOLD
};

struct AssetSignal {
    std::string instrument;                      // EUR_USD, GBP_USD, etc.
    sep::trading::QuantumIdentifiers quantum_identifiers;  // Current 60.73% accuracy system
    double correlation_weight;                   // Dynamic correlation to primary asset
    std::chrono::milliseconds lag;               // Optimal time lag for correlation
    double confidence_modifier;                  // Boost/reduce based on cross-asset agreement
};

struct FusedSignal {
    Direction primary_direction;     // BUY/SELL/HOLD for target asset
    double fusion_confidence;       // Weighted confidence across all assets
    std::vector<AssetSignal> contributing_signals;
    double cross_asset_coherence;   // Agreement level across correlated assets
    double signal_strength;         // Overall signal strength (0.0 to 1.0)
};

struct CrossAssetCorrelation {
    double strength;                 // Correlation coefficient (-1.0 to 1.0)
    std::chrono::milliseconds optimal_lag;  // Optimal time lag for maximum correlation
    double stability;               // How stable this correlation has been over time
};

class MultiAssetSignalFusion {
private:
    std::shared_ptr<sep::trading::QuantumSignalBridge> quantum_processor_;
    std::shared_ptr<sep::cache::EnhancedMarketModelCache> market_cache_;
    
    // Major forex pairs for cross-asset analysis
    const std::vector<std::string> MAJOR_PAIRS = {
        "EUR_USD", "GBP_USD", "USD_JPY", "USD_CHF", 
        "AUD_USD", "USD_CAD", "NZD_USD"
    };
    
    // Correlation cache for performance
    std::unordered_map<std::string, CrossAssetCorrelation> correlation_cache_;
    std::chrono::system_clock::time_point last_correlation_update_;
    
public:
    MultiAssetSignalFusion(
        std::shared_ptr<sep::trading::QuantumSignalBridge> quantum_processor,
        std::shared_ptr<sep::cache::EnhancedMarketModelCache> market_cache
    );
    
    // Main fusion interface
    FusedSignal generateFusedSignal(const std::string& target_asset);
    
    // Core fusion logic
    std::vector<std::string> getCorrelatedAssets(const std::string& target_asset);
    CrossAssetCorrelation calculateDynamicCorrelation(
        const std::string& asset1, 
        const std::string& asset2
    );
    double calculateCrossAssetBoost(
        const sep::trading::QuantumIdentifiers& signal, 
        const CrossAssetCorrelation& correlation
    );
    FusedSignal fuseSignals(const std::vector<AssetSignal>& asset_signals);
    
    // Advanced correlation analysis
    std::vector<double> calculateCorrelationMatrix(const std::vector<std::string>& assets);
    double calculateCrossAssetCoherence(const std::vector<AssetSignal>& signals);
    
    // Performance optimization
    void updateCorrelationCache();
    void invalidateCorrelationCache();
    
    // Debugging and analysis
    void logFusionDetails(const FusedSignal& signal);
    std::string serializeFusionResult(const FusedSignal& signal);
};

} // namespace sep
