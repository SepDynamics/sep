#pragma once

#include <string>
#include <vector>
#include <atomic>
#include <chrono>
#include <functional>
#include <memory>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <thread>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

namespace sep {
namespace connectors {

struct MarketData {
    std::string instrument;
    double bid;
    double ask;
    double mid;
    uint64_t timestamp;
    double volume;
    std::vector<double> bid_book;
    std::vector<double> ask_book;

    // Technical indicators
    double atr;              // Average True Range
    int volatility_level;    // 1-4 volatility classification
    double spread;           // Bid-ask spread
    double daily_change;     // Daily price change %
};

struct OandaCandle {
    std::string time;
    double open;
    double high;
    double low;
    double close;
    long volume;
};

struct DataValidationResult
{
    bool valid;
    std::vector<std::string> errors;
    std::vector<std::string> warnings;
};

enum class OrderStatus { PENDING, FILLED, CANCELED };

struct OrderInfo {
    std::string id;
    std::string instrument;
    double units{0};
    double price{0};
    OrderStatus status{OrderStatus::PENDING};
};

class OandaConnector {
public:
    OandaConnector(const std::string& api_key, const std::string& account_id, bool sandbox = true);
    ~OandaConnector();

    // Connection management
    bool initialize();
    bool testConnection();
    void shutdown();

    // Historical data
    std::vector<OandaCandle> getHistoricalData(
        const std::string& instrument,
        const std::string& granularity,
        const std::string& from,
        const std::string& to,
        int count = 500
    );

    // Real-time streaming
    bool startPriceStream(const std::vector<std::string>& instruments);
    bool stopPriceStream();
    void setPriceCallback(std::function<void(const MarketData&)> callback);

    // Account information
    nlohmann::json getAccountInfo();
    nlohmann::json getInstruments();
    nlohmann::json placeOrder(const nlohmann::json& order_details);
    nlohmann::json getOpenPositions();
    nlohmann::json getOrders();

    // Technical analysis
    double calculateATR(const std::string& instrument, const std::string& granularity = "H1",
                        int periods = 14);
    MarketData getMarketData(const std::string& instrument);
    int getVolatilityLevel(double current_atr, const std::string& instrument);

    // Sample Data
    void setupSampleData(const std::string& instrument, const std::string& granularity, const std::string& output_file);

    // Error handling
    std::string getLastError() const { return last_error_; }
    bool hasError() const { return !last_error_.empty(); }

    // Order tracking
    void refreshOrders();
    const std::vector<OrderInfo>& pendingOrders() const { return pending_orders_; }
    const std::vector<OrderInfo>& filledOrders() const { return filled_orders_; }
    const std::vector<OrderInfo>& canceledOrders() const { return canceled_orders_; }

private:
    std::string api_key_;
    std::string account_id_;
    std::string base_url_;
    std::string stream_url_;
    bool sandbox_;
    
    CURL* curl_handle_;
    std::string last_error_;
    std::function<void(const MarketData&)> price_callback_;
    
    // HTTP helpers
    struct CurlResponse {
        std::string data;
        long response_code;
    };
    
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, CurlResponse* response);
    static size_t StreamCallback(void* contents, size_t size, size_t nmemb, OandaConnector* connector);

    CurlResponse makeRequest(const std::string& endpoint, const std::string& method = "GET", const std::string& data = "");
    void processStreamData(const std::string& data);

    // Data conversion and validation
    MarketData parseMarketData(const nlohmann::json& price_data);
    OandaCandle parseCandle(const nlohmann::json& candle_data);
    int64_t parseTimestamp(const std::string& time_str);
    DataValidationResult validateCandle(const OandaCandle& candle);
    DataValidationResult validateCandleSequence(const std::vector<OandaCandle>& candles,
                                                const std::string& granularity);
    std::vector<double> calculateHistoricalATRs(const std::vector<OandaCandle>& candles);

    // Rate limiting
    void enforceRateLimit();
    std::chrono::steady_clock::time_point last_request_time_;
    
    // Streaming support
    std::atomic<bool> streaming_active_{false};
    std::thread stream_thread_;
    std::string stream_buffer_;
    void streamPriceData(const std::string& instruments);

    // Order caches
    std::vector<OrderInfo> pending_orders_;
    std::vector<OrderInfo> filled_orders_;
    std::vector<OrderInfo> canceled_orders_;
}; 

} // namespace connectors
}  // namespace sep