#include "src/connectors/oanda_connector.h"
#include <iostream>
#include <chrono>
#include <thread>

int main() {
    std::cout << "🧪 Testing OANDA Connector..." << std::endl;
    
    const char* api_key = std::getenv("OANDA_API_KEY");
    const char* account_id = std::getenv("OANDA_ACCOUNT_ID");
    
    if (!api_key || !account_id) {
        std::cerr << "❌ Environment variables not set" << std::endl;
        return 1;
    }
    
    std::cout << "📡 Creating OANDA connector..." << std::endl;
    sep::connectors::OandaConnector connector(api_key, account_id);
    
    std::cout << "📥 Requesting historical data..." << std::endl;
    
    bool received_data = false;
    size_t candle_count = 0;
    
    connector.getHistoricalData(
        "EUR_USD", "M1", "", "",
        [&](const std::vector<sep::connectors::OandaCandle>& candles) {
            candle_count = candles.size();
            received_data = true;
            std::cout << "📊 Callback received " << candle_count << " candles" << std::endl;
            if (candle_count > 0) {
                std::cout << "First candle time: " << candles[0].time << std::endl;
                std::cout << "First candle close: " << candles[0].close << std::endl;
            }
        });
    
    // Wait for response
    for (int i = 0; i < 10 && !received_data; ++i) {
        std::cout << "⏳ Waiting... " << i << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    if (received_data) {
        std::cout << "✅ Success! Received " << candle_count << " candles" << std::endl;
    } else {
        std::cout << "❌ Failed to receive data" << std::endl;
    }
    
    return 0;
}
