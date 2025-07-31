#pragma once

#include <vector>
#include <cstdint>
#include <string>
#include "connectors/oanda_connector.h"

namespace sep {
namespace connectors {

/**
 * @brief Converts market data from OANDA into byte streams for SEP pattern analysis
 * 
 * This class bridges the gap between financial market data and the SEP engine's
 * datatype-agnostic pattern analysis. It converts various market data types
 * (prices, volumes, spreads) into byte representations that can be analyzed
 * for quantum patterns.
 */
class MarketDataConverter {
public:
    /**
     * @brief Convert OANDA candle data to byte stream
     * 
     * Serializes OHLCV data into a byte stream format that preserves
     * temporal relationships and price movements for pattern analysis.
     * 
     * @param candles Vector of OANDA candles
     * @return Byte stream representation of the candle data
     */
    static std::vector<uint8_t> candlesToByteStream(const std::vector<OandaCandle>& candles);
    
    /**
     * @brief Convert price vector to bit stream
     * 
     * Converts a vector of double prices into a normalized bit stream
     * for quantum pattern analysis.
     * 
     * @param prices Vector of price values
     * @return Bit stream representation of price movements
     */
    static std::vector<uint8_t> convertToBitstream(const std::vector<double>& prices);
    

    
    /**
     * @brief Convert order book data to byte stream
     * 
     * Encodes bid/ask depth information to detect market microstructure patterns.
     * 
     * @param bid_book Vector of bid prices and volumes
     * @param ask_book Vector of ask prices and volumes
     * @return Byte stream representation
     */
    static std::vector<uint8_t> orderBookToByteStream(
        const std::vector<std::pair<double, double>>& bid_book,
        const std::vector<std::pair<double, double>>& ask_book
    );
    
    /**
     * @brief Create a composite byte stream from multiple data sources
     * 
     * Combines price, volume, and spread data into a unified byte stream
     * for comprehensive pattern analysis.
     * 
     * @param candles Historical candle data
     * @param market_data Current market snapshot
     * @param window_size Number of candles to include
     * @return Composite byte stream
     */
    static std::vector<uint8_t> createCompositeStream(
        const std::vector<OandaCandle>& candles,
        const MarketData& market_data,
        size_t window_size = 100
    );
    
private:
    // Helper methods for data encoding
    static void appendDouble(std::vector<uint8_t>& stream, double value);
    static void appendUint64(std::vector<uint8_t>& stream, uint64_t value);
    static void appendTimestamp(std::vector<uint8_t>& stream, const std::string& timestamp);
    
    // Normalization helpers
    static std::pair<double, double> calculateMeanStd(const std::vector<double>& values);
    static double normalizeValue(double value, double mean, double std);
};

} // namespace connectors
} // namespace sep