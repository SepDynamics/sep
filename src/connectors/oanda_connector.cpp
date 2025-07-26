#include "oanda_connector.h"

#include <chrono>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <thread>

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
        last_error_ = "Connection test failed: HTTP " + std::to_string(response.response_code) +
                      " - " + response.data;
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
    stopPriceStream();
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
    
    if (count > 0) {
        endpoint += "&count=" + std::to_string(count);
    }
    
    if (!from.empty()) {
        endpoint += "&from=" + from;
    }
    if (!to.empty()) {
        endpoint += "&to=" + to;
    }

    auto response = makeRequest(endpoint);

    if (response.response_code != 200) {
        last_error_ =
            "Failed to get historical data: HTTP " + std::to_string(response.response_code);
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
    if (streaming_active_)
    {
        last_error_ = "Streaming already active";
        return false;
    }

    if (instruments.empty())
    {
        last_error_ = "No instruments specified for streaming";
        return false;
    }

    std::string instruments_param;
    for (size_t i = 0; i < instruments.size(); ++i)
    {
        if (i > 0) instruments_param += ",";
        instruments_param += instruments[i];
    }

    streaming_active_ = true;
    stream_thread_ =
        std::thread([this, instruments_param]() { streamPriceData(instruments_param); });

    return true;
}

bool OandaConnector::stopPriceStream() {
    if (!streaming_active_)
    {
        return true;
    }

    streaming_active_ = false;

    if (stream_thread_.joinable())
    {
        stream_thread_.join();
    }

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
    if (!connector->streaming_active_) {
        return 0; // Abort stream
    }
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

    struct curl_slist* headers = nullptr;
    std::string auth_header = "Authorization: Bearer " + api_key_;
    headers = curl_slist_append(headers, auth_header.c_str());
    headers = curl_slist_append(headers, "Content-Type: application/json");
    curl_easy_setopt(curl_handle_, CURLOPT_HTTPHEADER, headers);

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

    if (response.response_code >= 400)
    {
        std::cerr << "OANDA API Error: " << response.response_code << " - " << response.data
                  << std::endl;
    }

    return response;
}

double OandaConnector::calculateATR(const std::string& instrument, const std::string& granularity,
                                    int periods)
{
    auto candles = getHistoricalData(instrument, granularity, "", "", periods + 1);
    
    if (candles.size() < periods) {
        last_error_ = "Insufficient candle data for ATR calculation";
        return 0.0;
    }
    
    std::vector<double> true_ranges;

    for (size_t i = 1; i < candles.size(); i++) {
        double high = candles[i].high;
        double low = candles[i].low;
        double prev_close = candles[i-1].close;

        double tr1 = high - low;
        double tr2 = std::abs(high - prev_close);
        double tr3 = std::abs(low - prev_close);
        double true_range = std::max({tr1, tr2, tr3});
        
        true_ranges.push_back(true_range);
    }

    if (true_ranges.empty()) {
        return 0.0;
    }
    
    double atr = std::accumulate(true_ranges.begin(), true_ranges.end(), 0.0) / true_ranges.size();
    return atr;
}

int OandaConnector::getVolatilityLevel(double current_atr, const std::string& instrument) {
    // Fetch last 90 days of daily candles for a stable volatility baseline
    auto candles = getHistoricalData(instrument, "D", "", "", 90);
    if (candles.size() < 20) { // Need a reasonable number of candles
        last_error_ = "Insufficient historical data for volatility calculation.";
        return 1; // Default to low volatility
    }

    auto atrs = calculateHistoricalATRs(candles);
    if (atrs.empty()) {
        last_error_ = "Could not calculate historical ATRs.";
        return 1; // Default to low volatility
    }

    std::sort(atrs.begin(), atrs.end());

    double p25 = atrs[atrs.size() / 4];
    double p75 = atrs[atrs.size() * 3 / 4];

    if (current_atr < p25) return 1;      // Low volatility
    if (current_atr < p75) return 2;      // Medium volatility
    return 3;                           // High volatility
}

MarketData OandaConnector::getMarketData(const std::string& instrument) {
    MarketData market_data;
    market_data.instrument = instrument;

    std::string endpoint = "/v3/accounts/" + account_id_ + "/pricing?instruments=" + instrument;
    auto response = makeRequest(endpoint);

    if (response.response_code != 200)
    {
        last_error_ = "Failed to get market data: " + response.data;
        return market_data;
    }

    try {
        auto json_response = nlohmann::json::parse(response.data);
        if (json_response.contains("prices") && !json_response["prices"].empty())
        {
            market_data = parseMarketData(json_response["prices"][0]);
        }
    }
    catch (const std::exception& e)
    {
        last_error_ = "Error parsing market data: " + std::string(e.what());
        return market_data;
    }

    market_data.atr = calculateATR(instrument);
    market_data.volatility_level = getVolatilityLevel(market_data.atr, instrument);
    market_data.spread = market_data.ask - market_data.bid;

    return market_data;
}

void OandaConnector::processStreamData(const std::string& data) {
    stream_buffer_ += data;
    size_t newline_pos;
    while ((newline_pos = stream_buffer_.find('\n')) != std::string::npos)
    {
        std::string line = stream_buffer_.substr(0, newline_pos);
        stream_buffer_.erase(0, newline_pos + 1);

        if (line.empty() || line.length() <= 1) {
            continue;
        }

        try {
            auto json_data = nlohmann::json::parse(line);
            if (json_data.contains("type")) {
                std::string type = json_data["type"];
                if (type == "PRICE") {
                    if (price_callback_) {
                        price_callback_(parseMarketData(json_data));
                    }
                } else if (type == "HEARTBEAT") {
                    // Connection is alive
                }
            }
        } catch (const nlohmann::json::parse_error& e) {
            // Incomplete JSON object, wait for more data
        }
    }
}

MarketData OandaConnector::parseMarketData(const nlohmann::json& price_data) {
    MarketData market_data;
    
    if (price_data.contains("instrument")) {
        market_data.instrument = price_data["instrument"];
    }

    if (price_data.contains("time")) {
        market_data.timestamp = parseTimestamp(price_data["time"]);
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

    if (candle_data.contains("mid")) {
        const auto& mid = candle_data["mid"];
        if (mid.contains("o")) candle.open = std::stod(mid["o"].get<std::string>());
        if (mid.contains("h")) candle.high = std::stod(mid["h"].get<std::string>());
        if (mid.contains("l")) candle.low = std::stod(mid["l"].get<std::string>());
        if (mid.contains("c")) candle.close = std::stod(mid["c"].get<std::string>());
    }
    
    return candle;
}

void OandaConnector::enforceRateLimit()
{
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_request_time_);

    if (elapsed.count() < 50)
    {  // OANDA allows 100 req/sec, 10ms, but being conservative
        std::this_thread::sleep_for(std::chrono::milliseconds(50 - elapsed.count()));
    }

    last_request_time_ = std::chrono::steady_clock::now();
}

void OandaConnector::streamPriceData(const std::string& instruments)
{
    std::string url = stream_url_ + "/v3/accounts/" + account_id_ + "/pricing/stream";
    url += "?instruments=" + instruments;

    CURL* curl = curl_easy_init();
    if (!curl)
    {
        std::cerr << "Failed to initialize CURL for streaming" << std::endl;
        return;
    }

    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, ("Authorization: Bearer " + api_key_).c_str());
    headers = curl_slist_append(headers, "Accept-Datetime-Format: UNIX");

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OandaConnector::StreamCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);

    curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);
    curl_easy_setopt(curl, CURLOPT_TCP_KEEPIDLE, 120L);
    curl_easy_setopt(curl, CURLOPT_TCP_KEEPINTVL, 60L);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK && streaming_active_)
    {
        std::cerr << "Streaming error: " << curl_easy_strerror(res) << std::endl;
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
}

nlohmann::json OandaConnector::placeOrder(const nlohmann::json& order_details) {
    std::string endpoint = "/v3/accounts/" + account_id_ + "/orders";

    CurlResponse response = makeRequest(endpoint, "POST", order_details.dump());

    if (response.response_code == 201) {
        try {
            auto json_resp = nlohmann::json::parse(response.data);
            OrderInfo info;
            if (json_resp.contains("orderCreateTransaction")) {
                const auto& tx = json_resp["orderCreateTransaction"];
                info.id = tx.value("id", "");
                info.instrument = tx.value("instrument", "");
                info.units = std::stod(tx.value("units", "0"));
                info.price = tx.contains("price") ? std::stod(tx["price"].get<std::string>()) : 0.0;
                info.status = OrderStatus::PENDING;
                pending_orders_.push_back(info);
            }
            if (json_resp.contains("orderFillTransaction")) {
                const auto& tx = json_resp["orderFillTransaction"];
                info.id = tx.value("id", "");
                info.instrument = tx.value("instrument", "");
                info.units = std::stod(tx.value("units", "0"));
                info.price = std::stod(tx.value("price", "0"));
                info.status = OrderStatus::FILLED;
                filled_orders_.push_back(info);
            }
            return json_resp;
        } catch (const std::exception& e) {
            last_error_ = "Failed to parse placeOrder response: " + std::string(e.what());
            return nlohmann::json{{"error", last_error_}};
        }
    }

    last_error_ = "Failed to place order: " + response.data;
    return nlohmann::json{{"error", last_error_}};
}

nlohmann::json OandaConnector::getOpenPositions() {
    std::string endpoint = "/v3/accounts/" + account_id_ + "/openPositions";
    
    CurlResponse response = makeRequest(endpoint, "GET");
    
    if (response.response_code == 200) {
        return nlohmann::json::parse(response.data);
    }
    
    last_error_ = "Failed to get open positions: " + response.data;
    return nlohmann::json{ {"error", last_error_} };
}

nlohmann::json OandaConnector::getOrders() {
    std::string endpoint = "/v3/accounts/" + account_id_ + "/orders?state=FILLED";
    
    CurlResponse response = makeRequest(endpoint, "GET");
    
    if (response.response_code == 200) {
        return nlohmann::json::parse(response.data);
    }
    
    last_error_ = "Failed to get orders: " + response.data;
    return nlohmann::json{ {"error", last_error_} };
}

void OandaConnector::refreshOrders() {
    auto fetch_state = [this](const std::string& state) -> nlohmann::json {
        std::string endpoint = "/v3/accounts/" + account_id_ + "/orders?state=" + state;
        CurlResponse res = makeRequest(endpoint, "GET");
        if (res.response_code == 200) {
            return nlohmann::json::parse(res.data);
        }
        return {};
    };

    pending_orders_.clear();
    filled_orders_.clear();
    canceled_orders_.clear();

    auto pending_json = fetch_state("PENDING");
    if (pending_json.contains("orders")) {
        for (const auto& o : pending_json["orders"]) {
            OrderInfo info;
            info.id = o.value("id", "");
            info.instrument = o.value("instrument", "");
            info.units = std::stod(o.value("units", "0"));
            info.price = o.contains("price") ? std::stod(o["price"].get<std::string>()) : 0.0;
            info.status = OrderStatus::PENDING;
            pending_orders_.push_back(info);
        }
    }

    auto filled_json = fetch_state("FILLED");
    if (filled_json.contains("orders")) {
        for (const auto& o : filled_json["orders"]) {
            OrderInfo info;
            info.id = o.value("id", "");
            info.instrument = o.value("instrument", "");
            info.units = std::stod(o.value("units", "0"));
            info.price = o.contains("price") ? std::stod(o["price"].get<std::string>()) : 0.0;
            info.status = OrderStatus::FILLED;
            filled_orders_.push_back(info);
        }
    }

    auto canceled_json = fetch_state("CANCELLED");
    if (canceled_json.contains("orders")) {
        for (const auto& o : canceled_json["orders"]) {
            OrderInfo info;
            info.id = o.value("id", "");
            info.instrument = o.value("instrument", "");
            info.units = std::stod(o.value("units", "0"));
            info.price = o.contains("price") ? std::stod(o["price"].get<std::string>()) : 0.0;
            info.status = OrderStatus::CANCELED;
            canceled_orders_.push_back(info);
        }
    }
}

void OandaConnector::setupSampleData(const std::string& instrument, const std::string& granularity, const std::string& output_file) {
    auto candles =
        getHistoricalData(instrument, granularity, "", "", 48 * 60);  // 48 hours of M1 data

    if (candles.empty()) {
        last_error_ = "Failed to fetch any data for sample setup.";
        std::cerr << "[OandaConnector] Error: " << last_error_ << std::endl;
        return;
    }

    auto validation = validateCandleSequence(candles, granularity);
    if (!validation.valid)
    {
        last_error_ = "Fetched data failed validation.";
        for (const auto& err : validation.errors)
            last_error_ +=
                "\
 - " + err;
        std::cerr << "[OandaConnector] Error: " << last_error_ << std::endl;
        return;
    }

    nlohmann::json json_output;
    for (const auto& c : candles) {
        nlohmann::json candle_json;
        candle_json["time"] = c.time;
        candle_json["open"] = c.open;
        candle_json["high"] = c.high;
        candle_json["low"] = c.low;
        candle_json["close"] = c.close;
        candle_json["volume"] = c.volume;
        json_output.push_back(candle_json);
    }

    std::ofstream out_stream(output_file);
    if (!out_stream.is_open()) {
        last_error_ = "Failed to open output file: " + output_file;
        std::cerr << "[OandaConnector] Error: " << last_error_ << std::endl;
        return;
    }

    out_stream << json_output.dump(4);
    out_stream.close();

    std::cout << "[OandaConnector] Successfully wrote " << candles.size() << " candles to " << output_file << std::endl;
}

// --- Data Validation Implementations ---

int64_t OandaConnector::parseTimestamp(const std::string& time_str)
{
    std::tm tm = {};
    std::stringstream ss(time_str);
    ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S");
    auto time_point = std::chrono::system_clock::from_time_t(std::mktime(&tm));
    return std::chrono::duration_cast<std::chrono::seconds>(time_point.time_since_epoch()).count();
}

DataValidationResult OandaConnector::validateCandle(const OandaCandle& candle)
{
    DataValidationResult result{true};
    if (candle.high < candle.low || candle.high < candle.open || candle.high < candle.close)
    {
        result.valid = false;
        result.errors.push_back("High price is not the highest price.");
    }
    if (candle.low > candle.high || candle.low > candle.open || candle.low > candle.close)
    {
        result.valid = false;
        result.errors.push_back("Low price is not the lowest price.");
    }
    if (candle.volume < 0)
    {
        result.valid = false;
        result.errors.push_back("Volume cannot be negative.");
    }
    return result;
}

DataValidationResult OandaConnector::validateCandleSequence(const std::vector<OandaCandle>& candles,
                                                            const std::string& granularity)
{
    DataValidationResult result{true};
    if (candles.size() < 2) return result;  // Not enough data to check sequence

    for (size_t i = 0; i < candles.size(); ++i)
    {
        auto single_validation = validateCandle(candles[i]);
        if (!single_validation.valid)
        {
            result.valid = false;
            for (const auto& err : single_validation.errors)
            {
                result.errors.push_back("Candle at " + candles[i].time + ": " + err);
            }
        }
    }

    // Check for timestamp continuity
    // This is a simplified check. A robust implementation would parse the granularity string.
    int64_t expected_diff = 60;  // Default to M1
    if (granularity == "H1")
        expected_diff = 3600;
    else if (granularity == "D")
        expected_diff = 86400;

    for (size_t i = 1; i < candles.size(); ++i)
    {
        int64_t t1 = parseTimestamp(candles[i - 1].time);
        int64_t t2 = parseTimestamp(candles[i].time);
        if ((t2 - t1) != expected_diff)
        {
            result.warnings.push_back("Timestamp gap detected between " + candles[i - 1].time +
                                      " and " + candles[i].time);
        }
    }

    return result;
}

std::vector<double> OandaConnector::calculateHistoricalATRs(const std::vector<OandaCandle>& candles)
{
    std::vector<double> atrs;
    if (candles.size() < 15) return atrs;

    std::vector<double> true_ranges;
    for (size_t i = 1; i < candles.size(); ++i)
    {
        double tr = std::max({candles[i].high - candles[i].low,
                              std::abs(candles[i].high - candles[i - 1].close),
                              std::abs(candles[i].low - candles[i - 1].close)});
        true_ranges.push_back(tr);
    }

    if (true_ranges.size() < 14) return atrs;

    double first_atr = std::accumulate(true_ranges.begin(), true_ranges.begin() + 14, 0.0) / 14.0;
    atrs.push_back(first_atr);

    for (size_t i = 14; i < true_ranges.size(); ++i)
    {
        double next_atr = (atrs.back() * 13 + true_ranges[i]) / 14.0;
        atrs.push_back(next_atr);
    }

    return atrs;
}

} // namespace connectors
}  // namespace sep