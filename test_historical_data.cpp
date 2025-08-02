#include "connectors/oanda_connector.h"
#include <iostream>
#include <chrono>
#include <thread>

int main() {
    const char* api_key = std::getenv("OANDA_API_KEY");
    const char* account_id = std::getenv("OANDA_ACCOUNT_ID");
    
    if (!api_key || !account_id) {
        std::cerr << "Missing OANDA environment variables" << std::endl;
        return 1;
    }
    
    sep::connectors::OandaConnector connector(api_key, account_id);
    
    std::cout << "Testing OANDA historical data fetch..." << std::endl;
    
    // Test 1: Use the working fetchHistoricalData method
    std::cout << "Test 1: Using fetchHistoricalData method..." << std::endl;
    bool success = connector.fetchHistoricalData("EUR_USD", "test_output.json");
    std::cout << "fetchHistoricalData result: " << (success ? "SUCCESS" : "FAILED") << std::endl;
    if (!success) {
        std::cout << "Error: " << connector.getLastError() << std::endl;
    }
    
    // Test 2: Try getHistoricalData with timestamp format
    std::cout << "\nTest 2: Using getHistoricalData with UNIX timestamps..." << std::endl;
    auto now = std::chrono::system_clock::now();
    auto start = now - std::chrono::hours(24);
    auto now_t = std::chrono::system_clock::to_time_t(now);
    auto start_t = std::chrono::system_clock::to_time_t(start);
    
    std::atomic<bool> callback_done{false};
    std::vector<sep::connectors::OandaCandle> received_candles;
    
    connector.getHistoricalData("EUR_USD", "M1", std::to_string(start_t), std::to_string(now_t),
        [&](const std::vector<sep::connectors::OandaCandle>& candles) {
            received_candles = candles;
            callback_done = true;
            std::cout << "Callback received " << candles.size() << " candles" << std::endl;
        });
    
    // Wait for callback
    int timeout = 30;
    while (!callback_done && timeout-- > 0) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    std::cout << "getHistoricalData result: " << received_candles.size() << " candles" << std::endl;
    
    // Test 3: Try with no time range (should get last 2880 candles)
    std::cout << "\nTest 3: Using getHistoricalData with no time range..." << std::endl;
    callback_done = false;
    received_candles.clear();
    
    connector.getHistoricalData("EUR_USD", "M1", "", "",
        [&](const std::vector<sep::connectors::OandaCandle>& candles) {
            received_candles = candles;
            callback_done = true;
            std::cout << "Callback received " << candles.size() << " candles" << std::endl;
        });
    
    timeout = 30;
    while (!callback_done && timeout-- > 0) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    std::cout << "getHistoricalData (no range) result: " << received_candles.size() << " candles" << std::endl;
    
    return 0;
}
