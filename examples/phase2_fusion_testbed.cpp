#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>
#include "apps/oanda_trader/multi_asset_signal_fusion.hpp"
#include "apps/oanda_trader/market_regime_adaptive.hpp"
#include "apps/oanda_trader/quantum_signal_bridge.hpp"
#include "apps/oanda_trader/enhanced_market_model_cache.hpp"
#include "connectors/oanda_connector.h"

using namespace sep;
using json = nlohmann::json;

// Test configuration
struct TestConfig {
    std::string primary_asset = "EUR_USD";
    std::vector<std::string> test_assets = {"EUR_USD", "GBP_USD", "USD_JPY", "USD_CHF"};
    bool enable_regime_adaptation = true;
    bool verbose_logging = true;
};

class Phase2TestBed {
private:
    std::shared_ptr<sep::connectors::OandaConnector> oanda_connector_;
    std::shared_ptr<sep::cache::EnhancedMarketModelCache> market_cache_;
    std::shared_ptr<sep::trading::QuantumSignalBridge> quantum_bridge_;
    std::shared_ptr<MultiAssetSignalFusion> signal_fusion_;
    std::shared_ptr<MarketRegimeAdaptiveProcessor> regime_processor_;
    
    TestConfig config_;
    json results_;

public:
    Phase2TestBed(const TestConfig& config) : config_(config) {
        // Initialize logging
        if (config_.verbose_logging) {
            spdlog::set_level(spdlog::level::debug);
        }
        
        // Initialize components
        initializeComponents();
    }
    
    void initializeComponents() {
        spdlog::info("🚀 Initializing Phase 2 Components...");
        
        // Initialize OANDA connector
        auto api_key = std::getenv("OANDA_API_KEY");
        auto account_id = std::getenv("OANDA_ACCOUNT_ID");
        auto api_url = std::getenv("OANDA_API_URL");
        
        if (!api_key || !account_id || !api_url) {
            spdlog::warn("OANDA credentials not found, using demo data");
            oanda_connector_ = std::make_shared<sep::connectors::OandaConnector>("demo", "demo", "https://api-fxpractice.oanda.com");
        } else {
            oanda_connector_ = std::make_shared<sep::connectors::OandaConnector>(api_key, account_id, api_url);
        }
        
        // Initialize enhanced market cache
        market_cache_ = std::make_shared<sep::cache::EnhancedMarketModelCache>(oanda_connector_);
        
        // Initialize quantum signal bridge
        quantum_bridge_ = std::make_shared<sep::trading::QuantumSignalBridge>();
        
        // Initialize signal fusion
        signal_fusion_ = std::make_shared<MultiAssetSignalFusion>(quantum_bridge_, market_cache_);
        
        // Initialize regime adaptive processor
        regime_processor_ = std::make_shared<MarketRegimeAdaptiveProcessor>(market_cache_);
        
        spdlog::info("✅ Phase 2 Components Initialized Successfully");
    }
    
    void runFusionTest() {
        spdlog::info("🔗 Testing Multi-Asset Signal Fusion...");
        
        try {
            // Test fusion for primary asset
            auto fused_signal = signal_fusion_->generateFusedSignal(config_.primary_asset);
            
            // Log fusion results
            std::string direction_str = 
                (fused_signal.primary_direction == Direction::BUY) ? "BUY" :
                (fused_signal.primary_direction == Direction::SELL) ? "SELL" : "HOLD";
            
            spdlog::info("🎯 FUSION RESULT: {} {} | Confidence: {:.3f} | Coherence: {:.3f} | Strength: {:.3f}",
                        config_.primary_asset, direction_str, 
                        fused_signal.fusion_confidence, fused_signal.cross_asset_coherence,
                        fused_signal.signal_strength);
            
            // Store results
            results_["fusion_test"] = {
                {"asset", config_.primary_asset},
                {"direction", direction_str},
                {"confidence", fused_signal.fusion_confidence},
                {"coherence", fused_signal.cross_asset_coherence},
                {"strength", fused_signal.signal_strength},
                {"contributing_assets", fused_signal.contributing_signals.size()}
            };
            
            // Test individual asset correlations
            for (const auto& asset : config_.test_assets) {
                if (asset != config_.primary_asset) {
                    auto correlation = signal_fusion_->calculateDynamicCorrelation(config_.primary_asset, asset);
                    spdlog::debug("📊 Correlation {}-{}: {:.3f} (stability: {:.3f})", 
                                config_.primary_asset, asset, correlation.strength, correlation.stability);
                }
            }
            
        } catch (const std::exception& e) {
            spdlog::error("❌ Fusion test failed: {}", e.what());
            results_["fusion_test"] = {{"error", e.what()}};
        }
    }
    
    void runRegimeTest() {
        spdlog::info("📊 Testing Market Regime Adaptation...");
        
        try {
            // Get adaptive thresholds for primary asset
            auto thresholds = regime_processor_->calculateRegimeOptimalThresholds(config_.primary_asset);
            
            spdlog::info("🎛️ ADAPTIVE THRESHOLDS: {} | Conf:{:.3f} Coh:{:.3f} Freq:{:.3f}", 
                        thresholds.regime_description,
                        thresholds.confidence_threshold,
                        thresholds.coherence_threshold,
                        thresholds.signal_frequency_modifier);
            
            // Store results
            results_["regime_test"] = {
                {"asset", config_.primary_asset},
                {"regime", thresholds.regime_description},
                {"confidence_threshold", thresholds.confidence_threshold},
                {"coherence_threshold", thresholds.coherence_threshold},
                {"frequency_modifier", thresholds.signal_frequency_modifier}
            };
            
            // Test regime detection for multiple assets
            for (const auto& asset : config_.test_assets) {
                auto regime = regime_processor_->detectCurrentRegime(asset);
                spdlog::debug("🔍 {} Regime: Vol={} Trend={} Liq={} Conf={:.2f}", 
                            asset, static_cast<int>(regime.volatility), 
                            static_cast<int>(regime.trend), static_cast<int>(regime.liquidity),
                            regime.regime_confidence);
            }
            
        } catch (const std::exception& e) {
            spdlog::error("❌ Regime test failed: {}", e.what());
            results_["regime_test"] = {{"error", e.what()}};
        }
    }
    
    void runIntegratedTest() {
        spdlog::info("🔄 Testing Integrated Phase 2 System...");
        
        try {
            // Get adaptive thresholds
            auto thresholds = regime_processor_->calculateRegimeOptimalThresholds(config_.primary_asset);
            
            // Generate fused signal
            auto fused_signal = signal_fusion_->generateFusedSignal(config_.primary_asset);
            
            // Apply regime-adaptive filtering
            bool signal_passes = (fused_signal.fusion_confidence >= thresholds.confidence_threshold) &&
                                (fused_signal.cross_asset_coherence >= thresholds.coherence_threshold);
            
            std::string direction_str = 
                (fused_signal.primary_direction == Direction::BUY) ? "BUY" :
                (fused_signal.primary_direction == Direction::SELL) ? "SELL" : "HOLD";
            
            if (signal_passes && fused_signal.primary_direction != Direction::HOLD) {
                spdlog::info("✅ PHASE 2 SIGNAL CONFIRMED: {} {} | Fusion:{:.3f} Regime:{:.3f}", 
                            config_.primary_asset, direction_str,
                            fused_signal.fusion_confidence, thresholds.confidence_threshold);
            } else {
                spdlog::info("⏸️ PHASE 2 SIGNAL FILTERED: {} {} | Fusion:{:.3f} < Regime:{:.3f}", 
                            config_.primary_asset, direction_str,
                            fused_signal.fusion_confidence, thresholds.confidence_threshold);
            }
            
            // Store integrated results
            results_["integrated_test"] = {
                {"asset", config_.primary_asset},
                {"direction", direction_str},
                {"signal_passes", signal_passes},
                {"fusion_confidence", fused_signal.fusion_confidence},
                {"required_confidence", thresholds.confidence_threshold},
                {"cross_asset_coherence", fused_signal.cross_asset_coherence},
                {"required_coherence", thresholds.coherence_threshold},
                {"regime", thresholds.regime_description}
            };
            
        } catch (const std::exception& e) {
            spdlog::error("❌ Integrated test failed: {}", e.what());
            results_["integrated_test"] = {{"error", e.what()}};
        }
    }
    
    void runAllTests() {
        spdlog::info("🧪 Starting Phase 2 Complete Test Suite...");
        
        runFusionTest();
        runRegimeTest();
        runIntegratedTest();
        
        // Print final results
        spdlog::info("📋 PHASE 2 TEST RESULTS:");
        std::cout << results_.dump(2) << std::endl;
        
        spdlog::info("🏁 Phase 2 Testing Complete");
    }
};

int main(int argc, char* argv[]) {
    spdlog::info("🚀 Phase 2 Fusion & Regime Adaptive Testbed");
    
    TestConfig config;
    
    // Parse command line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--asset" && i + 1 < argc) {
            config.primary_asset = argv[++i];
        } else if (arg == "--quiet") {
            config.verbose_logging = false;
        } else if (arg == "--no-regime") {
            config.enable_regime_adaptation = false;
        }
    }
    
    try {
        Phase2TestBed testbed(config);
        testbed.runAllTests();
        
        spdlog::info("✅ Phase 2 testbed completed successfully");
        return 0;
        
    } catch (const std::exception& e) {
        spdlog::error("❌ Phase 2 testbed failed: {}", e.what());
        return 1;
    }
}
