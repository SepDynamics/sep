#pragma once

#include "engine/internal/standard_includes.h"
#include <map>
#include <deque>
#include "common/financial_data_types.h"
#include "quantum/types.h"


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

// Universal data parser for all input sources
class DataParser {
public:
    DataParser() = default;
    ~DataParser() = default;

    // Parse from file (auto-detects format)
        std::vector<quantum::Pattern> parseFile(const std::string& path,
                                                 DataFormat format = DataFormat::AUTO);

    // Parse from memory buffer (binary/non-UTF8 safe)
        std::vector<quantum::Pattern> parseBuffer(const uint8_t* data, size_t size,
                                                   DataFormat format = DataFormat::AUTO);

    // Parse from stream (maintains state for continuous data)
        std::vector<quantum::Pattern> parseStream(std::istream& stream,
                                                   DataFormat format = DataFormat::AUTO);

    // Specific format parsers
    std::vector<sep::common::CandleData> parseQuantJSON(const std::string& path);
        std::vector<quantum::Pattern> parseCSV(const std::string& path);
        std::vector<quantum::Pattern> parseBinary(const uint8_t* data, size_t size);

    // Convert raw candle data to SEP patterns
        std::vector<quantum::Pattern> candlesToPatterns(const std::vector<sep::common::CandleData>& candles);

    // Utility: write candle data to OANDA-style JSON
    void writeQuantJSON(const std::vector<sep::common::CandleData>& candles, const std::string& path) const;

    // Save candle data with validation checks (time ordering, field ranges)
    bool saveValidatedCandlesJSON(const std::vector<sep::common::CandleData>& candles,
                                  const std::string& path) const;

    // Convert patterns to PinStates for engine compatibility
        std::vector<PinState> toPinStates(const std::vector<quantum::Pattern>& patterns);

    // Export correlation metrics to CSV
        bool exportCorrelationCSV(
            const std::string& path,
            const std::map<std::string, sep::common::CorrelationMetrics>& data) const;
        bool exportCorrelationJSON(
            const std::string& path,
            const std::map<std::string, sep::common::CorrelationMetrics>& data) const;
        bool exportCorrelationHistoryCSV(
            const std::string& path,
            const std::map<std::string, std::deque<sep::common::CorrelationMetrics>>& history)
            const;
        bool exportCorrelationForBacktester(
            const std::string& path,
            const std::deque<sep::common::CorrelationMetrics>& metrics) const;

    private:
        // Format detection
        DataFormat detectFormat(const uint8_t* data, size_t size) const;
        DataFormat detectFileFormat(const std::string& path) const;

        // Parse timestamp string to unix timestamp
        uint64_t parseTimestamp(const std::string& timestamp) const;

        // Stream state for continuous parsing
        struct StreamState
        {
            std::vector<uint8_t> buffer;
            size_t processed = 0;
        };
    std::unique_ptr<StreamState> stream_state_;
};

} // namespace sep