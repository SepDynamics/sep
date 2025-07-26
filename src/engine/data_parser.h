#pragma once

#include "engine/standard_includes.h"
#include <map>
#include "apps/workbench/core/multi_timeframe_analyzer.h"

namespace sep {
    namespace quantum
    {
        struct Pattern;
    }

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
    std::vector<sep::quantum::Pattern> parseFile(const std::string& path,
                                                 DataFormat format = DataFormat::AUTO);

    // Parse from memory buffer (binary/non-UTF8 safe)
    std::vector<sep::quantum::Pattern> parseBuffer(const uint8_t* data, size_t size,
                                                   DataFormat format = DataFormat::AUTO);

    // Parse from stream (maintains state for continuous data)
    std::vector<sep::quantum::Pattern> parseStream(std::istream& stream,
                                                   DataFormat format = DataFormat::AUTO);

    // Specific format parsers
    std::vector<CandleData> parseQuantJSON(const std::string& path);
    std::vector<sep::quantum::Pattern> parseCSV(const std::string& path);
    std::vector<sep::quantum::Pattern> parseBinary(const uint8_t* data, size_t size);

    // Convert raw candle data to SEP patterns
    std::vector<sep::quantum::Pattern> candlesToPatterns(const std::vector<CandleData>& candles);

    // Convert patterns to PinStates for engine compatibility
    std::vector<sep::PinState> toPinStates(const std::vector<sep::quantum::Pattern>& patterns);

    // Export correlation metrics to CSV
    bool exportCorrelationCSV(const std::string& path,
                              const std::map<std::string, workbench::CorrelationMetrics>& data) const;

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