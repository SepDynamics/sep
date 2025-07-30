#include "data_parser.h"

#include <cmath>
#include <ctime>
#include <nlohmann/json.hpp>

#include "common/financial_data_types.h"
#include "engine/internal/standard_includes.h"
#include "quantum/types.h"
#include "types.h"

namespace sep {

// Parse from file (auto-detects format)
std::vector<quantum::Pattern> DataParser::parseFile(const std::string& path, DataFormat format)
{
    if (format == DataFormat::AUTO) {
        format = detectFileFormat(path);
    }
    
    switch (format) {
        case DataFormat::JSON:
        case DataFormat::CANDLE: {
            auto candles = parseQuantJSON(path);
            return candlesToPatterns(candles);
        }
        case DataFormat::CSV:
            return parseCSV(path);
        case DataFormat::BINARY: {
            std::ifstream file(path.c_str(), std::ios::binary);
            if (!file.is_open()) {
                std::cerr << "Error: Could not open file: " << path << std::endl;
                return {};
            }
            
            // Read entire file into buffer
            file.seekg(0, std::ios::end);
            size_t size = file.tellg();
            file.seekg(0, std::ios::beg);

            std::vector<uint8_t> buffer(size);
            file.read(reinterpret_cast<char*>(buffer.data()), size);
            file.close();
            
            return parseBinary(buffer.data(), size);
        }
        default:
            std::cerr << "Error: Unsupported format" << std::endl;
            return {};
    }
}

// Parse from memory buffer (binary/non-UTF8 safe)
std::vector<quantum::Pattern> DataParser::parseBuffer(const uint8_t* data, size_t size,
                                                           DataFormat format)
{
    if (format == DataFormat::AUTO) {
        format = detectFormat(data, size);
    }
    
    switch (format) {
        case DataFormat::JSON:
        case DataFormat::CANDLE: {
            // Convert buffer to string for JSON parsing
            std::string json_str(reinterpret_cast<const char*>(data), size);
            try {
                nlohmann::json j = nlohmann::json::parse(json_str.c_str());
                std::vector<sep::common::CandleData> candles;

                if (j.contains("candles") && j["candles"].is_array()) {
                    for (const auto& candle_json : j["candles"]) {
                        sep::common::CandleData candle;
                        
                        if (candle_json.contains("time") && candle_json["time"].is_string()) {
                            auto tp = sep::common::parseTimestamp(candle_json["time"].get<std::string>());
                            candle.timestamp = std::chrono::duration_cast<std::chrono::seconds>(tp.time_since_epoch()).count();
                        }
                        
                        if (candle_json.contains("volume") && candle_json["volume"].is_number()) {
                            candle.volume = candle_json["volume"].get<uint64_t>();
                        }
                        
                        if (candle_json.contains("mid") && candle_json["mid"].is_object()) {
                            const auto& mid = candle_json["mid"];
                            
                            if (mid.contains("o") && mid["o"].is_string()) {
                                candle.open = std::stof(mid["o"].get<std::string>().c_str());
                            }
                            if (mid.contains("h") && mid["h"].is_string()) {
                                candle.high = std::stof(mid["h"].get<std::string>().c_str());
                            }
                            if (mid.contains("l") && mid["l"].is_string()) {
                                candle.low = std::stof(mid["l"].get<std::string>().c_str());
                            }
                            if (mid.contains("c") && mid["c"].is_string()) {
                                candle.close = std::stof(mid["c"].get<std::string>().c_str());
                            }
                        }
                        
                        candles.push_back(candle);
                    }
                }
                
                return candlesToPatterns(candles);
            } catch (const std::exception& e) {
                std::cerr << "Error parsing JSON from buffer: " << e.what() << std::string("\n");
                return {};
            }
        }
        case DataFormat::BINARY:
            return parseBinary(data, size);
        default:
            std::cerr << "Error: Unsupported format for buffer parsing" << std::string("\n");
            return {};
    }
}

// Parse from stream (maintains state for continuous data)
std::vector<quantum::Pattern> DataParser::parseStream(std::istream& stream, DataFormat format)
{
    if (!stream_state_) {
        stream_state_ = std::make_unique<StreamState>();
    }

    // Read available data from stream
    char buffer[4096];
    while (stream.read(buffer, sizeof(buffer))) {
        stream_state_->buffer.insert(stream_state_->buffer.end(), buffer, buffer + stream.gcount());
    }
    stream_state_->buffer.insert(stream_state_->buffer.end(), buffer, buffer + stream.gcount());

    if (format == DataFormat::AUTO && !stream_state_->buffer.empty()) {
        format = detectFormat(stream_state_->buffer.data(), stream_state_->buffer.size());
    }

    std::vector<quantum::Pattern> patterns;
    if (format == DataFormat::JSON || format == DataFormat::CANDLE) {
        try {
            // Attempt to parse the buffered data
            patterns = parseBuffer(stream_state_->buffer.data(), stream_state_->buffer.size(), format);
            // If successful, clear the buffer
            stream_state_->buffer.clear();
        } catch (const nlohmann::json::parse_error& e) {
            // Incomplete JSON, wait for more data
        }
    } else {
        // For other formats, parse what we have
        patterns = parseBuffer(stream_state_->buffer.data(), stream_state_->buffer.size(), format);
        stream_state_->buffer.clear();
    }

    return patterns;
}

// Parse CSV file
std::vector<quantum::Pattern> DataParser::parseCSV(const std::string& path)
{
    std::vector<sep::quantum::Pattern> patterns;
    std::ifstream file(path.c_str());

    if (!file.is_open()) {
        std::cerr << "Error: Could not open CSV file: " << path << std::endl;
        return patterns;
    }

    std::string line;
    size_t line_num = 0;

    while (std::getline(file, line))
    {
        line_num++;
        
        // Skip empty lines
        if (line.empty()) continue;
        
        // Simple CSV parsing - split by comma
        std::stringstream ss(line.c_str());
        std::vector<float> values;
        std::string field;

        while (std::getline(ss, field, ','))
        {
            try {
                values.push_back(std::stof(field));
            } catch (...) {
                // If not a number, skip
            }
        }

        if (!values.empty()) {
            quantum::Pattern pattern;
            pattern.id = std::to_string(line_num);
            auto current_time = std::chrono::system_clock::now();
            pattern.timestamp = std::chrono::duration_cast<std::chrono::seconds>(current_time.time_since_epoch()).count();

            // Map values to pattern fields
            if (values.size() >= 1) pattern.position.x = values[0];
            if (values.size() >= 2) pattern.position.y = values[1];
            if (values.size() >= 3) pattern.position.z = values[2];
            if (values.size() >= 4) pattern.position.w = values[3];
            
            // Store remaining values in data vector
            for (size_t i = 4; i < values.size(); ++i) {
                pattern.data.push_back(values[i]);
            }
            
            // Initialize other fields
            pattern.generation = 0;
            pattern.coherence = 0.0f;
            pattern.quantum_state = quantum::QuantumState{};
            pattern.velocity = glm::vec4(0.0f);
            pattern.attributes = glm::vec4(0.0f);
            pattern.amplitude = {1.0f, 0.0f};
            pattern.momentum = glm::vec3(0.0f);
            pattern.last_accessed = std::chrono::duration_cast<std::chrono::seconds>(current_time.time_since_epoch()).count();
            pattern.last_modified = std::chrono::duration_cast<std::chrono::seconds>(current_time.time_since_epoch()).count();
    
            patterns.push_back(pattern);
        }
    }

    file.close();
    return patterns;
}

// Parse binary data
std::vector<quantum::Pattern> DataParser::parseBinary(const uint8_t* data, size_t size)
{
    std::vector<sep::quantum::Pattern> patterns;
    const size_t floats_per_pattern = 4;
    const size_t bytes_per_pattern = floats_per_pattern * sizeof(float);

    if (size < bytes_per_pattern) {
        return patterns;
    }

    size_t num_patterns = size / bytes_per_pattern;
    const float* float_data = reinterpret_cast<const float*>(data);

    for (size_t i = 0; i < num_patterns; ++i) {
        quantum::Pattern pattern;
        pattern.id = std::to_string(i);
        auto current_time = std::chrono::system_clock::now();
        pattern.timestamp = std::chrono::duration_cast<std::chrono::seconds>(current_time.time_since_epoch()).count();

        size_t offset = i * floats_per_pattern;
        pattern.position = glm::vec4(float_data[offset], float_data[offset + 1], float_data[offset + 2], float_data[offset + 3]);

        // Derive coherence from variance
        float mean = (pattern.position.x + pattern.position.y + pattern.position.z + pattern.position.w) / 4.0f;
        float variance = ((pattern.position.x - mean) * (pattern.position.x - mean) +
                          (pattern.position.y - mean) * (pattern.position.y - mean) +
                          (pattern.position.z - mean) * (pattern.position.z - mean) +
                          (pattern.position.w - mean) * (pattern.position.w - mean)) / 4.0f;
        pattern.coherence = 1.0f / (1.0f + variance);

        pattern.generation = 0;
        pattern.quantum_state = quantum::QuantumState{};
        pattern.velocity = glm::vec4(0.0f);
        pattern.attributes = glm::vec4(0.0f);
        pattern.amplitude = {1.0f, 0.0f};
        pattern.momentum = glm::vec3(0.0f);
        pattern.last_accessed = std::chrono::duration_cast<std::chrono::seconds>(current_time.time_since_epoch()).count();
        pattern.last_modified = std::chrono::duration_cast<std::chrono::seconds>(current_time.time_since_epoch()).count();
        
        patterns.push_back(pattern);
    }

    return patterns;
}

// Convert patterns to PinStates for engine compatibility
std::vector<PinState> DataParser::toPinStates(
    const std::vector<quantum::Pattern>& patterns)
{
    std::vector<PinState> pin_states;

    for (const auto& pattern : patterns) {
        PinState state;

        // Convert pattern data to uint64_t state
        // Simple approach: use position.x as floating point bits
        state.value = pattern.position.x;
        state.coherence = pattern.coherence;

        // Could also combine multiple fields into the 64-bit state
        // For example: pack x,y into high/low 32 bits
        
        pin_states.push_back(state);
    }
    
    return pin_states;
}

// Format detection
DataFormat DataParser::detectFormat(const uint8_t* data, size_t size) const {
    if (size == 0) return DataFormat::BINARY;

    // Check for JSON
    if (data[0] == '{' || data[0] == '[') {
        std::string str(reinterpret_cast<const char*>(data), std::min(size, size_t(100)));
        // Check if the string contains "candles"
        bool has_candles = false;
        const char* candles_str = "candles";
        size_t candles_len = 7;
        if (str.size() >= candles_len)
        {
            for (size_t i = 0; i <= str.size() - candles_len; ++i)
            {
                bool match = true;
                for (size_t j = 0; j < candles_len; ++j)
                {
                    if (str[i + j] != candles_str[j])
                    {
                        match = false;
                        break;
                    }
                }
                if (match)
                {
                    has_candles = true;
                    break;
                }
            }
        }
        if (has_candles)
        {
            return DataFormat::CANDLE;
        }
        return DataFormat::JSON;
    }

    // Check for CSV
    bool has_comma = false;
    bool has_newline = false;
    for (size_t i = 0; i < std::min(size, size_t(1000)); ++i) {
        if (data[i] == ',') has_comma = true;
        if (data[i] == '\n') has_newline = true;
        if (has_comma && has_newline) return DataFormat::CSV;
    }

    // Check for binary based on entropy
    std::vector<int> counts(256, 0);
    for (size_t i = 0; i < size; ++i) {
        counts[data[i]]++;
    }

    double entropy = 0.0;
    for (int count : counts) {
        if (count > 0) {
            double p = static_cast<double>(count) / static_cast<double>(size);
            entropy -= p * std::log2(p);
        }
    }

    if (entropy > 7.5) { // High entropy suggests binary data
        return DataFormat::BINARY;
    }

    // Default to text-based formats if not clearly binary
    return DataFormat::CSV;
}

DataFormat DataParser::detectFileFormat(const std::string& path) const
{
    // Check file extension
    size_t dot_pos = path.find_last_of('.');
    if (dot_pos != std::string::npos)
    {
        std::string ext = path.substr(dot_pos + 1);
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
        
        if (ext == "json") return DataFormat::JSON;
        if (ext == "csv") return DataFormat::CSV;
        if (ext == "bin" || ext == "dat") return DataFormat::BINARY;
    }

    // Read first few bytes to detect format
    std::ifstream file(path.c_str(), std::ios::binary);
    if (file.is_open()) {
        uint8_t buffer[1024];
        file.read(reinterpret_cast<char*>(buffer), sizeof(buffer));
        size_t bytes_read = file.gcount();
        file.close();
        
        return detectFormat(buffer, bytes_read);
    }
    
    return DataFormat::BINARY;
}

std::vector<sep::common::CandleData> DataParser::parseQuantJSON(const std::string& path)
{
    std::vector<sep::common::CandleData> candles;
    std::ifstream file(path.c_str());

    if (!file.is_open()) {
        std::cerr << "Error: Could not open JSON file: " << path << std::endl;
        return candles;
    }
    
    try {
        nlohmann::json j;
        file >> j;
        
        if (j.is_array()) {
            size_t index = 0;
            for (const auto& candle_json : j) {
                sep::common::CandleData candle;

                bool valid = true;

                if (candle_json.contains("time") && candle_json["time"].is_string()) {
                    candle.timestamp = std::chrono::duration_cast<std::chrono::seconds>(sep::common::parseTimestamp(candle_json["time"].get<std::string>()).time_since_epoch()).count();
                } else {
                    std::cerr << "[DataParser] Missing time at index " << index << "\n";
                    valid = false;
                }

                if (candle_json.contains("volume") && candle_json["volume"].is_number()) {
                    candle.volume = candle_json["volume"].get<uint64_t>();
                } else {
                    candle.volume = 0;
                }

                if (candle_json.contains("open") && candle_json["open"].is_number())
                    candle.open = candle_json["open"].get<double>();
                else
                    valid = false;
                if (candle_json.contains("high") && candle_json["high"].is_number())
                    candle.high = candle_json["high"].get<double>();
                else
                    valid = false;
                if (candle_json.contains("low") && candle_json["low"].is_number())
                    candle.low = candle_json["low"].get<double>();
                else
                    valid = false;
                if (candle_json.contains("close") && candle_json["close"].is_number())
                    candle.close = candle_json["close"].get<double>();
                else
                    valid = false;

                if (!candles.empty()) {
                    auto prev_ts = candles.back().timestamp;
                    auto cur_ts = candle.timestamp;
                    if (cur_ts <= prev_ts) {
                        std::cerr << "[DataParser] Non-increasing timestamp at index " << index << "\n";
                        valid = false;
                    } else if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::seconds(cur_ts) - std::chrono::seconds(prev_ts)).count() != 60000) {
                        std::cerr << "[DataParser] Missing candle between " << candles.size()-1
                                  << " and current" << "\n";
                    }
                }

                if (valid)
                    candles.push_back(candle);
                index++;
            }
        } else {
            if (!j.contains("candles") || !j["candles"].is_array()) {
                std::cerr << "Error: JSON file does not contain 'candles' array" << std::endl;
                return candles;
            }

            size_t index = 0;
            for (const auto& candle_json : j["candles"]) {
            sep::common::CandleData candle;
            
            // Parse time
            if (candle_json.contains("time") && candle_json["time"].is_string()) {
                candle.timestamp = std::chrono::duration_cast<std::chrono::seconds>(sep::common::parseTimestamp(candle_json["time"].get<std::string>()).time_since_epoch()).count();
            }
            
            // Parse volume
            if (candle_json.contains("volume") && candle_json["volume"].is_number()) {
                candle.volume = candle_json["volume"].get<uint64_t>();
            }
            
            // Parse mid prices (OHLC)
            if (candle_json.contains("mid") && candle_json["mid"].is_object()) {
                const auto& mid = candle_json["mid"];
                
                if (mid.contains("o") && mid["o"].is_string()) {
                    candle.open = std::stof(mid["o"].get<std::string>().c_str());
                }
                if (mid.contains("h") && mid["h"].is_string()) {
                    candle.high = std::stof(mid["h"].get<std::string>().c_str());
                }
                if (mid.contains("l") && mid["l"].is_string()) {
                    candle.low = std::stof(mid["l"].get<std::string>().c_str());
                }
                if (mid.contains("c") && mid["c"].is_string()) {
                    candle.close = std::stof(mid["c"].get<std::string>().c_str());
                }
            }
            
            bool valid_prices = true;
            if (candle.high < candle.low ||
                candle.open > candle.high || candle.open < candle.low ||
                candle.close > candle.high || candle.close < candle.low)
            {
                std::cerr << "[DataParser] Invalid OHLC at index " << index << "\n";
                valid_prices = false;
            }

            if (!candles.empty()) {
                auto prev_ts = candles.back().timestamp;
                auto cur_ts = candle.timestamp;
                if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::seconds(cur_ts) - std::chrono::seconds(prev_ts)).count() != 60000) {
                    std::cerr << "[DataParser] Missing candle between previous and current\n";
                }
            }

                if (valid_prices)
                    candles.push_back(candle);
                index++;
            }
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error parsing JSON: " << e.what() << std::endl;
    }
    
    file.close();
    return candles;
}

std::vector<quantum::Pattern> DataParser::candlesToPatterns(
    const std::vector<sep::common::CandleData>& candles)
{
    std::vector<sep::quantum::Pattern> patterns;

    for (const auto& candle : candles) {
        quantum::Pattern pattern;

        // Use timestamp as unique ID
        pattern.id = std::to_string(candle.timestamp);

        // Map OHLC to position vector (raw data, no normalization)
        pattern.position.x = candle.open;
        pattern.position.y = candle.high;
        pattern.position.z = candle.low;
        pattern.position.w = candle.close;
        
        // Store volume in data vector
        pattern.data.push_back(static_cast<float>(candle.volume));
        
        // Set timestamp
        auto tp = std::chrono::system_clock::from_time_t(static_cast<time_t>(candle.timestamp));
        pattern.timestamp = sep::common::time_point_to_nanoseconds(tp);
        pattern.last_accessed = sep::common::time_point_to_nanoseconds(tp);
        pattern.last_modified = sep::common::time_point_to_nanoseconds(tp);
        
        // Initialize quantum state with defaults - let the quantum algorithms determine these
        pattern.generation = 0;
        pattern.coherence = 0.0f;  // Will be calculated by QBSA/QFH
        pattern.quantum_state = {};  // Default initialized

        // Initialize other required fields with defaults
        pattern.velocity = glm::vec4(0.0f);
        pattern.attributes = glm::vec4(0.0f);
        pattern.amplitude = std::complex<float>(1.0f, 0.0f);
        pattern.momentum = glm::vec3(0.0f);
        
        patterns.push_back(pattern);
    }
    
    return patterns;
}

void DataParser::writeQuantJSON(const std::vector<sep::common::CandleData>& candles, const std::string& path) const
{
    nlohmann::json j;
    j["candles"] = nlohmann::json::array();
    for (const auto& c : candles)
    {
        if (!std::isfinite(c.open) || !std::isfinite(c.high) || !std::isfinite(c.low) ||
            !std::isfinite(c.close) || c.high < c.low)
        {
            auto tp = std::chrono::system_clock::from_time_t(static_cast<time_t>(c.timestamp));
            std::time_t tt = std::chrono::system_clock::to_time_t(tp);
            std::cerr << "[DataParser] Invalid candle data at " << std::ctime(&tt) << std::endl;
            continue;
        }

        nlohmann::json cj;
        auto tp = std::chrono::system_clock::from_time_t(static_cast<time_t>(c.timestamp));
        std::time_t tt = std::chrono::system_clock::to_time_t(tp);
        std::stringstream ss;
        ss << std::put_time(std::gmtime(&tt), "%Y-%m-%dT%H:%M:%S");
        cj["time"] = ss.str();
        cj["volume"] = c.volume;
        cj["mid"] = {
            {"o", std::to_string(c.open)},
            {"h", std::to_string(c.high)},
            {"l", std::to_string(c.low)},
            {"c", std::to_string(c.close)}
        };
        j["candles"].push_back(cj);
    }

    std::ofstream file(path);
    if (file.is_open())
    {
        file << j.dump(4);
    }
}

bool DataParser::saveValidatedCandlesJSON(const std::vector<sep::common::CandleData>& candles,
                                          const std::string& path) const
{
    if (candles.empty()) {
        return false;
    }

    // Basic integrity checks and chronological ordering
    for (size_t i = 0; i < candles.size(); ++i) {
        const auto& c = candles[i];
        if (!std::isfinite(c.open) || !std::isfinite(c.high) || !std::isfinite(c.low) ||
            !std::isfinite(c.close) || c.high < c.low || c.volume < 0 ||
            c.open > c.high || c.open < c.low || c.close > c.high || c.close < c.low) {
            std::cerr << "[DataParser] Invalid candle at index " << i << "\n";
            return false;
        }
        if (i > 0) {
            auto prev_ts = candles[i - 1].timestamp;
            auto cur_ts = c.timestamp;
            if (cur_ts <= prev_ts) {
                std::cerr << "[DataParser] Non-increasing timestamp at index " << i << "\n";
                return false;
            }
        }
    }

    writeQuantJSON(candles, path);
    return true;
}

uint64_t DataParser::parseTimestamp(const std::string& timestamp) const
{
    return sep::common::time_point_to_nanoseconds(sep::common::parseTimestamp(timestamp));
}

bool DataParser::exportCorrelationCSV(const std::string& path,
                                      const std::map<std::string, sep::common::CorrelationMetrics>& data) const {
    std::ofstream file(path);
    if (!file.is_open()) {
        return false;
    }
    file << "timeframe,coh_pearson,coh_spearman,stab_pearson,stab_spearman,entropy_pearson,entropy_spearman,samples\n";
    for (const auto& [tf, metrics] : data) {
        file << tf << ','
             << metrics.coherence_pearson << ','
             << metrics.coherence_spearman << ','
             << metrics.stability_pearson << ','
             << metrics.stability_spearman << ','
             << metrics.entropy_pearson << ','
             << metrics.entropy_spearman << ','
             << metrics.sample_count << "\n";
    }
    return true;
}

bool DataParser::exportCorrelationJSON(const std::string& path,
                                       const std::map<std::string, sep::common::CorrelationMetrics>& data) const {
    nlohmann::json j;
    for (const auto& [tf, metrics] : data) {
        j[tf] = {
            {"coh_pearson", metrics.coherence_pearson},
            {"coh_spearman", metrics.coherence_spearman},
            {"stab_pearson", metrics.stability_pearson},
            {"stab_spearman", metrics.stability_spearman},
            {"entropy_pearson", metrics.entropy_pearson},
            {"entropy_spearman", metrics.entropy_spearman},
            {"samples", metrics.sample_count}
        };
    }
    std::ofstream file(path);
    if (!file.is_open()) {
        return false;
    }
    file << j.dump(2);
    return true;
}

bool DataParser::exportCorrelationHistoryCSV(const std::string& path,
                                             const std::map<std::string, std::deque<sep::common::CorrelationMetrics>>& history) const {
    std::ofstream file(path);
    if (!file.is_open()) {
        return false;
    }

    file << "timeframe,index,coh_pearson,coh_spearman,stab_pearson,stab_spearman,entropy_pearson,entropy_spearman\n";
    for (const auto& [tf, metrics_queue] : history) {
        size_t idx = 0;
        for (const auto& m : metrics_queue) {
            file << tf << ',' << idx++ << ','
                 << m.coherence_pearson << ','
                 << m.coherence_spearman << ','
                 << m.stability_pearson << ','
                 << m.stability_spearman << ','
                 << m.entropy_pearson << ','
                 << m.entropy_spearman << "\n";
        }
    }

    return true;
}

bool DataParser::exportCorrelationForBacktester(const std::string& path,
                                                const std::deque<sep::common::CorrelationMetrics>& metrics) const {
    std::ofstream file(path);
    if (!file.is_open()) {
        return false;
    }
    file << "index,coh_pearson,coh_spearman,stab_pearson,stab_spearman,entropy_pearson,entropy_spearman\n";
    size_t idx = 0;
    for (const auto& m : metrics) {
        file << idx++ << ','
             << m.coherence_pearson << ','
             << m.coherence_spearman << ','
             << m.stability_pearson << ','
             << m.stability_spearman << ','
             << m.entropy_pearson << ','
             << m.entropy_spearman << "\n";
    }
    return true;
}

} // namespace sep