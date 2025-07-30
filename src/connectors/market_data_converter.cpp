#include "connectors/market_data_converter.h"
#include <cstring>
#include <cmath>
#include <numeric>
#include <algorithm>

namespace sep {
namespace connectors {

std::vector<uint8_t> MarketDataConverter::candlesToByteStream(const std::vector<OandaCandle>& candles) {
    std::vector<uint8_t> stream;
    
    // Reserve space for efficiency
    stream.reserve(candles.size() * (sizeof(double) * 5 + 32)); // OHLCV + timestamp estimate
    
    for (const auto& candle : candles) {
        // Encode timestamp
        appendTimestamp(stream, candle.time);
        
        // Encode OHLCV data
        appendDouble(stream, candle.open);
        appendDouble(stream, candle.high);
        appendDouble(stream, candle.low);
        appendDouble(stream, candle.close);
        appendDouble(stream, static_cast<double>(candle.volume));
        
        // Add spread information if we had bid/ask data
        double spread = (candle.high - candle.low) / candle.close;
        appendDouble(stream, spread);
    }
    
    return stream;
}



std::vector<uint8_t> MarketDataConverter::orderBookToByteStream(
    const std::vector<std::pair<double, double>>& bid_book,
    const std::vector<std::pair<double, double>>& ask_book) {
    
    std::vector<uint8_t> stream;
    
    // Encode book imbalance for pattern detection
    double total_bid_volume = 0;
    double total_ask_volume = 0;
    
    for (const auto& [price, volume] : bid_book) {
        total_bid_volume += volume;
        appendDouble(stream, price);
        appendDouble(stream, volume);
    }
    
    for (const auto& [price, volume] : ask_book) {
        total_ask_volume += volume;
        appendDouble(stream, price);
        appendDouble(stream, volume);
    }
    
    // Encode order book imbalance
    double imbalance = (total_bid_volume - total_ask_volume) / (total_bid_volume + total_ask_volume);
    appendDouble(stream, imbalance);
    
    return stream;
}

std::vector<uint8_t> MarketDataConverter::createCompositeStream(
    const std::vector<OandaCandle>& candles,
    const MarketData& market_data,
    size_t window_size) {
    
    std::vector<uint8_t> stream;
    
    // Use recent candles for context
    size_t start_idx = candles.size() > window_size ? candles.size() - window_size : 0;
    std::vector<OandaCandle> recent_candles(candles.begin() + start_idx, candles.end());
    
    // Combine historical and real-time data
    auto candle_stream = candlesToByteStream(recent_candles);
    auto market_stream = std::vector<uint8_t>();
    
    // Merge streams
    stream.insert(stream.end(), candle_stream.begin(), candle_stream.end());
    stream.insert(stream.end(), market_stream.begin(), market_stream.end());
    
    // Add market regime indicators
    if (!recent_candles.empty()) {
        // Calculate simple volatility measure
        std::vector<double> returns;
        for (size_t i = 1; i < recent_candles.size(); ++i) {
            double ret = (recent_candles[i].close - recent_candles[i-1].close) / recent_candles[i-1].close;
            returns.push_back(ret);
        }
        
        auto [mean, std] = calculateMeanStd(returns);
        appendDouble(stream, std); // Volatility indicator
        
        // Trend indicator
        double trend = (recent_candles.back().close - recent_candles.front().close) / recent_candles.front().close;
        appendDouble(stream, trend);
    }
    
    return stream;
}

void MarketDataConverter::appendDouble(std::vector<uint8_t>& stream, double value) {
    const uint8_t* bytes = reinterpret_cast<const uint8_t*>(&value);
    stream.insert(stream.end(), bytes, bytes + sizeof(double));
}

void MarketDataConverter::appendUint64(std::vector<uint8_t>& stream, uint64_t value) {
    const uint8_t* bytes = reinterpret_cast<const uint8_t*>(&value);
    stream.insert(stream.end(), bytes, bytes + sizeof(uint64_t));
}

void MarketDataConverter::appendTimestamp(std::vector<uint8_t>& stream, const std::string& timestamp) {
    // Convert ISO timestamp to epoch for consistent encoding
    // For now, use a hash of the timestamp
    std::hash<std::string> hasher;
    uint64_t time_hash = hasher(timestamp);
    appendUint64(stream, time_hash);
}

std::pair<double, double> MarketDataConverter::calculateMeanStd(const std::vector<double>& values) {
    if (values.empty()) return {0.0, 0.0};
    
    double mean = std::accumulate(values.begin(), values.end(), 0.0) / values.size();
    
    double sq_sum = 0.0;
    for (double val : values) {
        sq_sum += (val - mean) * (val - mean);
    }
    
    double std = std::sqrt(sq_sum / values.size());
    return {mean, std};
}

double MarketDataConverter::normalizeValue(double value, double mean, double std) {
    if (std == 0.0) return 0.0;
    return (value - mean) / std;
}

} // namespace connectors
} // namespace sep