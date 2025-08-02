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
#include <unordered_map>
#include <mutex>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include "common/financial_data_types.h"

namespace sep {
namespace connectors {

struct MarketData {
    std::string instrument = "";
    double bid = 0.0;
    double ask = 0.0;
    double mid = 0.0;
    uint64_t timestamp = 0;
    double volume = 0.0;
    std::vector<double> bid_book;
    std::vector<double> ask_book;

    // Technical indicators
    double atr = 0.0;              // Average True Range
    int volatility_level = 1;      // 1-4 volatility classification
    double spread = 0.0;           // Bid-ask spread
    double daily_change = 0.0;     // Daily price change %
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
        const std::string& to
    );

    // Real-time streaming
    bool startPriceStream(const std::vector<std::string>& instruments);
    bool stopPriceStream();
    void setPriceCallback(std::function<void(const MarketData&)> callback);
    void setCandleCallback(std::function<void(const common::CandleData&)> cb);

    // Account information
    nlohmann::json getAccountInfo();
    nlohmann::json getInstruments();
    nlohmann::json placeOrder(const nlohmann::json& order_details);
    nlohmann::json getOpenPositions();
    nlohmann::json getOrders();

    // Technical analysis
    double calculateATR(const std::string& instrument, const std::string& granularity = "H1",
                        size_t periods = 14);
    MarketData getMarketData(const std::string& instrument);
    int getVolatilityLevel(double current_atr, const std::string& instrument);

    // Sample Data
    void setupSampleData(const std::string& instrument, const std::string& granularity, const std::string& output_file);
    bool fetchHistoricalData(const std::string& instrument, const std::string& output_file);
    bool saveEURUSDM1_48h(const std::string& output_file = "eur_usd_m1_48h.json");

    // Error handling
    std::string getLastError() const { return last_error_; }
    bool hasError() const { return !last_error_.empty(); }

    // Order tracking
    void refreshOrders();
    const std::vector<common::OrderInfo>& pendingOrders() const { return pending_orders_; }
    const std::vector<common::OrderInfo>& filledOrders() const { return filled_orders_; }
    const std::vector<common::OrderInfo>& canceledOrders() const { return canceled_orders_; }
    void setOrderCallback(std::function<void(const common::OrderInfo&)> cb) { order_callback_ = std::move(cb); }

private:
    std::string api_key_;
    std::string account_id_;
    std::string base_url_;
    std::string stream_url_;
    bool sandbox_;
    std::string cache_path_ = "./cache/oanda";
    
    CURL* curl_handle_;
    std::string last_error_;
    std::function<void(const MarketData&)> price_callback_;
    std::function<void(const common::CandleData&)> candle_callback_;

    struct CandleBuilder {
        common::CandleData candle;
        std::chrono::system_clock::time_point start;
        bool active{false};
    };
    std::unordered_map<std::string, CandleBuilder> candle_builders_;
    
    // HTTP helpers
    struct CurlResponse {
        std::string data;
        long response_code;
    };
    
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, CurlResponse* response);
    static size_t StreamCallback(void* contents, size_t size, size_t nmemb, OandaConnector* connector);

    CurlResponse makeRequest(const std::string& endpoint, const std::string& method = "GET", const std::string& data = "");
    void processStreamData(const std::string& data);
    void updateCandleBuilder(const MarketData& md);

    // Cache helpers
    std::string getCacheFilename(const std::string& instrument, const std::string& granularity, const std::string& from, const std::string& to);
    std::vector<OandaCandle> loadFromCache(const std::string& filename);
    void saveToCache(const std::string& filename, const std::vector<OandaCandle>& candles);

    // Data conversion and validation
    MarketData parseMarketData(const nlohmann::json& price_data);
    OandaCandle parseCandle(const nlohmann::json& candle_data);
    int64_t parseTimestamp(const std::string& time_str);
    DataValidationResult validateCandle(const OandaCandle& candle);
    DataValidationResult validateCandleSequence(const std::vector<OandaCandle>& candles,
                                                const std::string& granularity);
    std::vector<double> calculateHistoricalATRs(const std::vector<OandaCandle>& candles);

    // Rate limiting
    void enforceRateLimit(const std::string& endpoint);
    std::chrono::steady_clock::time_point last_request_time_;
    std::unordered_map<std::string, std::chrono::steady_clock::time_point> endpoint_last_request_;
    struct CachedResponse {
        CurlResponse response;
        std::chrono::steady_clock::time_point timestamp;
    };
    std::unordered_map<std::string, CachedResponse> response_cache_;
    std::mutex request_mutex_;
    
    // Streaming support
    std::atomic<bool> streaming_active_{false};
    std::thread stream_thread_;
    std::string stream_buffer_;
    void streamPriceData(const std::string& instruments);

    // Order caches
    std::vector<common::OrderInfo> pending_orders_;
    std::vector<common::OrderInfo> filled_orders_;
    std::vector<common::OrderInfo> canceled_orders_;
    std::function<void(const common::OrderInfo&)> order_callback_;
};

} // namespace connectors
}  // namespace sep