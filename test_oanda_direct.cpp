#include <iostream>
#include <thread>
#include <chrono>

// Simple test to verify OANDA API is accessible
int main() {
    std::cout << "🧪 Testing OANDA API directly..." << std::endl;
    
    // Test that we can make a basic curl request
    int result = system("source OANDA.env && curl -s -H \"Authorization: Bearer $OANDA_API_KEY\" \"https://api-fxpractice.oanda.com/v3/instruments/EUR_USD/candles?granularity=M1&count=5\" | head -10");
    
    if (result == 0) {
        std::cout << "✅ OANDA API is accessible!" << std::endl;
    } else {
        std::cout << "❌ OANDA API test failed" << std::endl;
    }
    
    return 0;
}
