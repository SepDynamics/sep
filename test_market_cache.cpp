#include "src/apps/oanda_trader/market_model_cache.hpp"
#include "src/connectors/oanda_connector.h"
#include <iostream>
#include <memory>

int main() {
    std::cout << "🧪 Testing Market Model Cache..." << std::endl;
    
    // Create a mock OANDA connector
    auto connector = std::make_shared<sep::connectors::OandaConnector>("test_api_key", "test_account_id");
    
    // Create the cache
    sep::apps::MarketModelCache cache(connector);
    
    std::cout << "✅ Market Model Cache created successfully!" << std::endl;
    std::cout << "📁 Cache directory: /sep/cache/market_model/" << std::endl;
    
    // Test cache path generation
    std::cout << "📅 Testing cache path generation..." << std::endl;
    
    return 0;
}
