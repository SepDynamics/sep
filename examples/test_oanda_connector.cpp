#include "../src/connectors/oanda_connector.h"
#include <iostream>
#include <cstdlib>

int main() {
    std::cout << "Testing OANDA Connector..." << std::endl;
    
    // Get environment variables
    const char* api_key = std::getenv("OANDA_API_KEY");
    const char* account_id = std::getenv("OANDA_ACCOUNT_ID");
    
    if (!api_key || !account_id) {
        std::cerr << "Error: OANDA_API_KEY and OANDA_ACCOUNT_ID environment variables must be set" << std::endl;
        return 1;
    }
    
    std::cout << "API Key: " << std::string(api_key).substr(0, 10) << "..." << std::endl;
    std::cout << "Account ID: " << account_id << std::endl;
    
    // Create connector (sandbox mode)
    sep::connectors::OandaConnector connector(api_key, account_id, true);
    
    // Initialize
    std::cout << "\nInitializing connector..." << std::endl;
    if (!connector.initialize()) {
        std::cerr << "Failed to initialize: " << connector.getLastError() << std::endl;
        return 1;
    }
    std::cout << "✓ Connector initialized successfully" << std::endl;
    
    // Test connection
    std::cout << "\nTesting connection..." << std::endl;
    std::cout << "Using sandbox URL: https://api-fxpractice.oanda.com" << std::endl;
    std::cout << "Testing endpoint: /v3/accounts/" << account_id << std::endl;
    
    if (!connector.testConnection()) {
        std::cerr << "Connection test failed: " << connector.getLastError() << std::endl;
        
        // Try a simple curl command to debug
        std::string curl_cmd = "curl -H 'Authorization: Bearer " + std::string(api_key) + 
                              "' 'https://api-fxpractice.oanda.com/v3/accounts/" + std::string(account_id) + "' -v";
        std::cout << "\nDebug: Running manual curl test..." << std::endl;
        std::cout << "Command: " << curl_cmd << std::endl;
        system(curl_cmd.c_str());
        
        return 1;
    }
    std::cout << "✓ Connection test passed" << std::endl;
    
    // Get account info
    std::cout << "\nFetching account info..." << std::endl;
    auto account_info = connector.getAccountInfo();
    if (connector.hasError()) {
        std::cerr << "Failed to get account info: " << connector.getLastError() << std::endl;
        return 1;
    }
    
    if (account_info.contains("account")) {
        auto& account = account_info["account"];
        std::cout << "✓ Account ID: " << account.value("id", "N/A") << std::endl;
        std::cout << "✓ Currency: " << account.value("currency", "N/A") << std::endl;
        std::cout << "✓ Balance: " << account.value("balance", "N/A") << std::endl;
    }
    
    // Get instruments
    std::cout << "\nFetching available instruments..." << std::endl;
    auto instruments = connector.getInstruments();
    if (connector.hasError()) {
        std::cerr << "Failed to get instruments: " << connector.getLastError() << std::endl;
        return 1;
    }
    
    if (instruments.contains("instruments")) {
        auto& instr_list = instruments["instruments"];
        std::cout << "✓ Found " << instr_list.size() << " instruments" << std::endl;
        
        // Show first 5 instruments
        int count = 0;
        for (const auto& instr : instr_list) {
            if (count++ >= 5) break;
            std::cout << "  - " << instr.value("name", "N/A") << std::endl;
        }
    }
    
    // Test historical data for EUR_USD
    std::cout << "\nFetching historical data for EUR_USD..." << std::endl;
    auto candles = connector.getHistoricalData("EUR_USD", "M1", "", "", 10);
    
    if (connector.hasError()) {
        std::cerr << "Failed to get historical data: " << connector.getLastError() << std::endl;
        return 1;
    }
    
    std::cout << "✓ Retrieved " << candles.size() << " candles" << std::endl;
    for (size_t i = 0; i < std::min(size_t(3), candles.size()); i++) {
        const auto& candle = candles[i];
        std::cout << "  " << candle.time << " O:" << candle.open 
                  << " H:" << candle.high << " L:" << candle.low 
                  << " C:" << candle.close << " V:" << candle.volume << std::endl;
    }
    
    std::cout << "\n✓ All tests passed! OANDA connector is working." << std::endl;
    
    return 0;
}
