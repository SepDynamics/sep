#pragma once

#include <memory>
#include <vector>
#include <string>
#include <chrono>
#include "connectors/oanda_connector.h"
#include "connectors/market_data_converter.h"
#include "memory/memory_tier_manager.hpp"
#include "engine/engine.h"

namespace sep::workbench {

/**
 * Clean, unified dashboard that actually works
 * Shows only the essential information in a usable format
 */
class UnifiedDashboard {
public:
    UnifiedDashboard();
    ~UnifiedDashboard();

    // Core lifecycle
    bool initialize();
    void shutdown();
    void render();

private:
    // Main sections
    void renderAccountSection();
    void renderMarketDataSection();
    void renderEngineMetricsSection();
    void renderMemorySection();
    
    // Helper methods
    void updateMarketData();
    void updateEngineMetrics();
    void updateMemoryInfo();
    void feedMarketDataToEngine(const sep::connectors::MarketData& market_data);
    std::string formatCurrency(double value);
    std::string formatBytes(size_t bytes);

private:
    // Core components
    std::unique_ptr<sep::connectors::OandaConnector> oanda_connector_;
    std::unique_ptr<sep::core::Engine> sep_engine_;
    std::shared_ptr<sep::memory::MemoryTierManager> memory_manager_;
    
    // Current data
    struct {
        std::string account_id;
        double balance = 0.0;
        double unrealized_pnl = 0.0;
        std::string currency = "USD";
        bool connected = false;
    } account_info_;
    
    struct {
        std::string instrument = "EUR_USD";
        double bid = 0.0;
        double ask = 0.0;
        double spread = 0.0;
        std::chrono::system_clock::time_point last_update;
    } market_data_;
    
    struct {
        double coherence = 0.0;
        double stability = 0.0;
        double entropy = 0.0;
        size_t pattern_count = 0;
        bool processing = false;
    } engine_metrics_;
    
    struct {
        size_t total_mb = 0;
        size_t tier1_mb = 0;
        size_t tier2_mb = 0;
        size_t tier3_mb = 0;
        float pressure_percent = 0.0f;
    } memory_info_;
    
    // Update timers
    std::chrono::steady_clock::time_point last_market_update_;
    std::chrono::steady_clock::time_point last_engine_update_;
    std::chrono::steady_clock::time_point last_memory_update_;
};

} // namespace sep::workbench
