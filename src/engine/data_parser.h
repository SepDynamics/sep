#pragma once

#include <string>
#include <vector>
#include <memory>
#include <array>
#include <cstdint>
#include <istream>

namespace sep {

// Forward declarations
struct Pattern;
struct PinState;

// Supported data formats
enum class DataFormat {
    AUTO,      // Auto-detect format
    JSON,      // JSON format
    CSV,       // CSV format
    BINARY,    // Raw binary data
    CANDLE     // Market candle data
};

// Raw candle data structure matching the JSON format
struct CandleData {
    std::string time;
    uint64_t volume;
    float open;
    float high;
    float low;
    float close;
};

// Universal data parser for all input sources
class DataParser {
public:
    DataParser() = default;
    ~DataParser() = default;

    // Parse from file (auto-detects format)
    std::vector<Pattern> parseFile(const std::string& path, DataFormat format = DataFormat::AUTO);
    
    // Parse from memory buffer (binary/non-UTF8 safe)
    std::vector<Pattern> parseBuffer(const uint8_t* data, size_t size, DataFormat format = DataFormat::AUTO);
    
    // Parse from stream (maintains state for continuous data)
    std::vector<Pattern> parseStream(std::istream& stream, DataFormat format = DataFormat::AUTO);
    
    // Specific format parsers
    std::vector<CandleData> parseQuantJSON(const std::string& path);
    std::vector<Pattern> parseCSV(const std::string& path);
    std::vector<Pattern> parseBinary(const uint8_t* data, size_t size);
    
    // Convert raw candle data to SEP patterns
    std::vector<Pattern> candlesToPatterns(const std::vector<CandleData>& candles);
    
    // Convert patterns to PinStates for engine compatibility
    std::vector<PinState> toPinStates(const std::vector<Pattern>& patterns);
    
private:
    // Format detection
    DataFormat detectFormat(const uint8_t* data, size_t size) const;
    DataFormat detectFileFormat(const std::string& path) const;
    
    // Parse timestamp string to unix timestamp
    uint64_t parseTimestamp(const std::string& timestamp) const;
    
    // Stream state for continuous parsing
    struct StreamState {
        std::vector<uint8_t> buffer;
        size_t processed = 0;
    };
    std::unique_ptr<StreamState> stream_state_;
};

} // namespace sep