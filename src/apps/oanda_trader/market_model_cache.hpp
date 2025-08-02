#pragma once
#include <vector>
#include <string>
#include <memory>
#include <map>
#include "connectors/oanda_connector.h"
#include "candle_types.h"
#include "quantum_signal_bridge.hpp"

namespace sep::apps {

class MarketModelCache {
public:
    explicit MarketModelCache(std::shared_ptr<sep::connectors::OandaConnector> connector);

    // Main function: Ensures cache for the last week is ready
    bool ensureCacheForLastWeek(const std::string& instrument = "EUR_USD");
    
    // Accessor for the processed signals
    const std::map<std::string, sep::trading::QuantumTradingSignal>& getSignalMap() const;

private:
    bool loadCache(const std::string& filepath);
    bool saveCache(const std::string& filepath) const;
    void processAndCacheData(const std::vector<Candle>& raw_candles, const std::string& filepath);
    std::string getCacheFilepathForLastWeek(const std::string& instrument) const;

    std::shared_ptr<sep::connectors::OandaConnector> oanda_connector_;
    std::map<std::string, sep::trading::QuantumTradingSignal> processed_signals_;
    std::string cache_directory_ = "/sep/cache/market_model/";
};

} // namespace sep::apps
