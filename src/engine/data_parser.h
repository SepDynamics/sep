
#pragma once

#include "engine/shim.h"
#include "quantum/types.h"

namespace sep {

    // Supported data formats
    enum class DataFormat
    {
        AUTO,    // Auto-detect format
        JSON,    // JSON format
        CSV,     // CSV format
        BINARY,  // Raw binary data
        CANDLE   // Market candle data
    };

    // Raw candle data structure matching the JSON format
    struct CandleData
    {
        shim::string time;
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
    shim::vector<sep::quantum::Pattern> parseFile(const shim::string& path,
                                                  DataFormat format = DataFormat::AUTO);

    // Parse from memory buffer (binary/non-UTF8 safe)
    shim::vector<sep::quantum::Pattern> parseBuffer(const uint8_t* data, size_t size,
                                                    DataFormat format = DataFormat::AUTO);

    // Parse from stream (maintains state for continuous data)
    shim::vector<sep::quantum::Pattern> parseStream(std::istream& stream,
                                                    DataFormat format = DataFormat::AUTO);

    // Specific format parsers
    shim::vector<CandleData> parseQuantJSON(const shim::string& path);
    shim::vector<sep::quantum::Pattern> parseCSV(const shim::string& path);
    shim::vector<sep::quantum::Pattern> parseBinary(const uint8_t* data, size_t size);

    // Convert raw candle data to SEP patterns
    shim::vector<sep::quantum::Pattern> candlesToPatterns(const shim::vector<CandleData>& candles);

    // Convert patterns to PinStates for engine compatibility
    shim::vector<PinState> toPinStates(const shim::vector<sep::quantum::Pattern>& patterns);

private:
    // Format detection
    DataFormat detectFormat(const uint8_t* data, size_t size) const;
    DataFormat detectFileFormat(const shim::string& path) const;

    // Parse timestamp string to unix timestamp
    uint64_t parseTimestamp(const shim::string& timestamp) const;

    // Stream state for continuous parsing
    struct StreamState {
        shim::vector<uint8_t> buffer;
        size_t processed = 0;
    };
    shim::unique_ptr<StreamState> stream_state_;
};

} // namespace sep