#include "oanda_connector.h"
#include <iostream>
#include <sstream>
#include <chrono>
#include <thread>
#include <cstring>

namespace sep {
namespace connectors {

OandaConnector::OandaConnector(const std::string& api_key, const std::string& account_id, bool sandbox)
    : api_key_(api_key)
    , account_id_(account_id)
    , sandbox_(sandbox)
    , curl_handle_(nullptr)
    , last_request_time_(std::chrono::steady_clock::now()) {
    
    if (sandbox) {
        base_url_ = "https://api-fxpractice.oanda.com";
        stream_url_ = "https://stream-fxpractice.oanda.com";
    } else {
        base_url_ = "https://api-fxtrade.oanda.com";
        stream_url_ = "https://stream-fxtrade.oanda.com";
    }
}

OandaConnector::~OandaConnector() {
    shutdown();
}

bool OandaConnector::initialize() {
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl_handle_ = curl_easy_init();
    
    if (!curl_handle_) {
        last_error_ = "Failed to initialize CURL";
        return false;
    }
    
    return testConnection();
}

bool OandaConnector::testConnection() {
    auto response = makeRequest("/v3/accounts/" + account_id_);
    
    if (response.response_code != 200) {
        last_error_ = "Connection test failed: HTTP " + std::to_string(response.response_code);
        return false;
    }
    
    try {
        auto json_response = nlohmann::json::parse(response.data);
        if (!json_response.contains("account")) {
            last_error_ = "Invalid account response";
            return false;
        }
    } catch (const std::exception& e) {
        last_error_ = "JSON parse error: " + std::string(e.what());
        return false;
    }
    
    return true;
}

void OandaConnector::shutdown() {
    if (curl_handle_) {
        curl_easy_cleanup(curl_handle_);
        curl_handle_ = nullptr;
    }
    curl_global_cleanup();
}

std::vector<OandaCandle> OandaConnector::getHistoricalData(
    const std::string& instrument,
    const std::string& granularity,
    const std::string& from,
    const std::string& to,
    int count) {
    
    std::vector<OandaCandle> candles;
    
    std::string endpoint = "/v3/instruments/" + instrument + "/candles";
    endpoint += "?granularity=" + granularity;
    endpoint += "&count=" + std::to_string(count);
    
    if (!from.empty()) {
        endpoint += "&from=" + from;
    }
    if (!to.empty()) {
        endpoint += "&to=" + to;
    }
    
    auto response = makeRequest(endpoint);
    
    if (response.response_code != 200) {
        last_error_ = "Failed to get historical data: HTTP " + std::to_string(response.response_code);
        return candles;
    }
    
    try {
        auto json_response = nlohmann::json::parse(response.data);
        
        if (!json_response.contains("candles")) {
            last_error_ = "No candles in response";
            return candles;
        }
        
        for (const auto& candle_json : json_response["candles"]) {
            candles.push_back(parseCandle(candle_json));
        }
        
    } catch (const std::exception& e) {
        last_error_ = "Error parsing historical data: " + std::string(e.what());
    }
    
    return candles;
}

bool OandaConnector::startPriceStream(const std::vector<std::string>& instruments) {
    // TODO: Implement streaming via separate thread
    last_error_ = "Streaming not yet implemented";
    return false;
}

bool OandaConnector::stopPriceStream() {
    // TODO: Implement
    return true;
}

void OandaConnector::setPriceCallback(std::function<void(const MarketData&)> callback) {
    price_callback_ = callback;
}

nlohmann::json OandaConnector::getAccountInfo() {
    auto response = makeRequest("/v3/accounts/" + account_id_);
    
    if (response.response_code != 200) {
        last_error_ = "Failed to get account info: HTTP " + std::to_string(response.response_code);
        return nlohmann::json{};
    }
    
    try {
        return nlohmann::json::parse(response.data);
    } catch (const std::exception& e) {
        last_error_ = "Error parsing account info: " + std::string(e.what());
        return nlohmann::json{};
    }
}

nlohmann::json OandaConnector::getInstruments() {
    auto response = makeRequest("/v3/accounts/" + account_id_ + "/instruments");
    
    if (response.response_code != 200) {
        last_error_ = "Failed to get instruments: HTTP " + std::to_string(response.response_code);
        return nlohmann::json{};
    }
    
    try {
        return nlohmann::json::parse(response.data);
    } catch (const std::exception& e) {
        last_error_ = "Error parsing instruments: " + std::string(e.what());
        return nlohmann::json{};
    }
}

size_t OandaConnector::WriteCallback(void* contents, size_t size, size_t nmemb, CurlResponse* response) {
    size_t total_size = size * nmemb;
    response->data.append(static_cast<char*>(contents), total_size);
    return total_size;
}

size_t OandaConnector::StreamCallback(void* contents, size_t size, size_t nmemb, OandaConnector* connector) {
    size_t total_size = size * nmemb;
    std::string data(static_cast<char*>(contents), total_size);
    connector->processStreamData(data);
    return total_size;
}

OandaConnector::CurlResponse OandaConnector::makeRequest(const std::string& endpoint, const std::string& method, const std::string& data) {
    enforceRateLimit();
    
    CurlResponse response;
    
    if (!curl_handle_) {
        response.response_code = 0;
        last_error_ = "CURL not initialized";
        return response;
    }
    
    std::string url = base_url_ + endpoint;
    
    curl_easy_setopt(curl_handle_, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl_handle_, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl_handle_, CURLOPT_WRITEDATA, &response);
    
    // Set headers
    struct curl_slist* headers = nullptr;
    std::string auth_header = "Authorization: Bearer " + api_key_;
    headers = curl_slist_append(headers, auth_header.c_str());
    headers = curl_slist_append(headers, "Content-Type: application/json");
    curl_easy_setopt(curl_handle_, CURLOPT_HTTPHEADER, headers);
    
    // Set method
    if (method == "POST") {
        curl_easy_setopt(curl_handle_, CURLOPT_POST, 1L);
        if (!data.empty()) {
            curl_easy_setopt(curl_handle_, CURLOPT_POSTFIELDS, data.c_str());
        }
    }
    
    CURLcode res = curl_easy_perform(curl_handle_);
    curl_slist_free_all(headers);
    
    if (res != CURLE_OK) {
        last_error_ = "CURL error: " + std::string(curl_easy_strerror(res));
        response.response_code = 0;
        return response;
    }
    
    curl_easy_getinfo(curl_handle_, CURLINFO_RESPONSE_CODE, &response.response_code);
    return response;
}

void OandaConnector::processStreamData(const std::string& data) {
    // TODO: Parse streaming price data and call price_callback_
}

MarketData OandaConnector::parseMarketData(const nlohmann::json& price_data) {
    MarketData market_data;
    
    // TODO: Parse JSON price data into MarketData struct
    if (price_data.contains("instrument")) {
        market_data.instrument = price_data["instrument"];
    }
    
    if (price_data.contains("bids") && !price_data["bids"].empty()) {
        market_data.bid = std::stod(price_data["bids"][0]["price"].get<std::string>());
    }
    
    if (price_data.contains("asks") && !price_data["asks"].empty()) {
        market_data.ask = std::stod(price_data["asks"][0]["price"].get<std::string>());
    }
    
    market_data.mid = (market_data.bid + market_data.ask) / 2.0;
    
    return market_data;
}

OandaCandle OandaConnector::parseCandle(const nlohmann::json& candle_data) {
    OandaCandle candle;
    
    if (candle_data.contains("time")) {
        candle.time = candle_data["time"];
    }
    
    if (candle_data.contains("volume")) {
        candle.volume = candle_data["volume"];
    }
    
    // Parse mid prices
    if (candle_data.contains("mid")) {
        const auto& mid = candle_data["mid"];
        if (mid.contains("o")) candle.open = std::stod(mid["o"].get<std::string>());
        if (mid.contains("h")) candle.high = std::stod(mid["h"].get<std::string>());
        if (mid.contains("l")) candle.low = std::stod(mid["l"].get<std::string>());
        if (mid.contains("c")) candle.close = std::stod(mid["c"].get<std::string>());
    }
    
    return candle;
}

void OandaConnector::enforceRateLimit() {
    // OANDA allows 100 requests per second - enforce 50ms minimum between requests
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_request_time_);
    
    if (elapsed.count() < 50) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50 - elapsed.count()));
    }
    
    last_request_time_ = std::chrono::steady_clock::now();
}

} // namespace connectors
} // namespace sep
