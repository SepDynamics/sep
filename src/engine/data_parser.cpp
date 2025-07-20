#include "data_parser.h"

#include <nlohmann/json.hpp>

#include "engine/shim.h"
#include "quantum/types.h"
#include "types.h"

namespace sep {

// Parse from file (auto-detects format)
shim::vector<Pattern> DataParser::parseFile(const shim::string& path, DataFormat format)
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
            shim::ifstream file(path.c_str(), std::ios::binary);
            if (!file.is_open()) {
                shim::cerr << "Error: Could not open file: " << path << std::endl;
                return {};
            }
            
            // Read entire file into buffer
            file.seekg(0, std::ios::end);
            size_t size = file.tellg();
            file.seekg(0, std::ios::beg);

            shim::vector<uint8_t> buffer(size);
            file.read(reinterpret_cast<char*>(buffer.data()), size);
            file.close();
            
            return parseBinary(buffer.data(), size);
        }
        default:
            shim::cerr << "Error: Unsupported format" << std::endl;
            return {};
    }
}

// Parse from memory buffer (binary/non-UTF8 safe)
shim::vector<Pattern> DataParser::parseBuffer(const uint8_t* data, size_t size, DataFormat format)
{
    if (format == DataFormat::AUTO) {
        format = detectFormat(data, size);
    }
    
    switch (format) {
        case DataFormat::JSON:
        case DataFormat::CANDLE: {
            // Convert buffer to string for JSON parsing
            shim::string json_str(reinterpret_cast<const char*>(data), size);
            try {
                nlohmann::json j = nlohmann::json::parse(json_str);
                shim::vector<CandleData> candles;

                if (j.contains("candles") && j["candles"].is_array()) {
                    for (const auto& candle_json : j["candles"]) {
                        CandleData candle;
                        
                        if (candle_json.contains("time") && candle_json["time"].is_string()) {
                            candle.time = candle_json["time"].get<shim::string>();
                        }
                        
                        if (candle_json.contains("volume") && candle_json["volume"].is_number()) {
                            candle.volume = candle_json["volume"].get<uint64_t>();
                        }
                        
                        if (candle_json.contains("mid") && candle_json["mid"].is_object()) {
                            const auto& mid = candle_json["mid"];
                            
                            if (mid.contains("o") && mid["o"].is_string()) {
                                candle.open = std::stof(mid["o"].get<shim::string>());
                            }
                            if (mid.contains("h") && mid["h"].is_string()) {
                                candle.high = std::stof(mid["h"].get<shim::string>());
                            }
                            if (mid.contains("l") && mid["l"].is_string()) {
                                candle.low = std::stof(mid["l"].get<shim::string>());
                            }
                            if (mid.contains("c") && mid["c"].is_string()) {
                                candle.close = std::stof(mid["c"].get<shim::string>());
                            }
                        }
                        
                        candles.push_back(candle);
                    }
                }
                
                return candlesToPatterns(candles);
            } catch (const std::exception& e) {
                shim::cerr << "Error parsing JSON from buffer: " << e.what() << std::endl;
                return {};
            }
        }
        case DataFormat::BINARY:
            return parseBinary(data, size);
        default:
            shim::cerr << "Error: Unsupported format for buffer parsing" << std::endl;
            return {};
    }
}

// Parse from stream (maintains state for continuous data)
shim::vector<Pattern> DataParser::parseStream(std::istream& stream, DataFormat format)
{
    if (!stream_state_) {
        stream_state_ = shim::make_unique<StreamState>();
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

    shim::vector<Pattern> patterns;
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
shim::vector<Pattern> DataParser::parseCSV(const shim::string& path)
{
    shim::vector<Pattern> patterns;
    shim::ifstream file(path.c_str());

    if (!file.is_open()) {
        shim::cerr << "Error: Could not open CSV file: " << path << std::endl;
        return patterns;
    }

    shim::string line;
    int line_num = 0;
    
    while (std::getline(file, line)) {
        line_num++;
        
        // Skip empty lines
        if (line.empty()) continue;
        
        // Simple CSV parsing - split by comma
        shim::stringstream ss(line.c_str());
        shim::vector<float> values;
        shim::string field;

        while (std::getline(ss, field, ',')) {
            try {
                values.push_back(shim::stof(field));
            } catch (...) {
                // If not a number, skip
            }
        }
        
        if (!values.empty()) {
            Pattern pattern;
            pattern.id = shim::to_string(line_num);
            pattern.timestamp = std::chrono::system_clock::now().time_since_epoch().count();
            
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
            pattern.amplitude = std::complex<float>(1.0f, 0.0f);
            pattern.momentum = glm::vec3(0.0f);
            pattern.last_accessed = pattern.timestamp;
            pattern.last_modified = pattern.timestamp;
            
            patterns.push_back(pattern);
        }
    }
    
    file.close();
    return patterns;
}

// Parse binary data
shim::vector<Pattern> DataParser::parseBinary(const uint8_t* data, size_t size)
{
    shim::vector<Pattern> patterns;
    const size_t floats_per_pattern = 4;
    const size_t bytes_per_pattern = floats_per_pattern * sizeof(float);

    if (size < bytes_per_pattern) {
        return patterns;
    }

    size_t num_patterns = size / bytes_per_pattern;
    const float* float_data = reinterpret_cast<const float*>(data);

    for (size_t i = 0; i < num_patterns; ++i) {
        Pattern pattern;
        pattern.id = shim::to_string(i);
        pattern.timestamp = std::chrono::system_clock::now().time_since_epoch().count();

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
        pattern.amplitude = std::complex<float>(1.0f, 0.0f);
        pattern.momentum = glm::vec3(0.0f);
        pattern.last_accessed = pattern.timestamp;
        pattern.last_modified = pattern.timestamp;

        patterns.push_back(pattern);
    }

    return patterns;
}

// Convert patterns to PinStates for engine compatibility
shim::vector<PinState> DataParser::toPinStates(const shim::vector<Pattern>& patterns)
{
    shim::vector<PinState> pin_states;

    for (const auto& pattern : patterns) {
        PinState state;
        
        // Convert pattern data to uint64_t state
        // Simple approach: use position.x as floating point bits
        float value = pattern.position.x;
        std::memcpy(&state.state, &value, sizeof(float));
        
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
        shim::string str(reinterpret_cast<const char*>(data), std::min(size, size_t(100)));
        if (str.find("candles") != shim::string::npos)
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
    shim::vector<int> counts(256, 0);
    for (size_t i = 0; i < size; ++i) {
        counts[data[i]]++;
    }

    double entropy = 0.0;
    for (int count : counts) {
        if (count > 0) {
            double p = static_cast<double>(count) / size;
            entropy -= p * shim::log2(p);
        }
    }

    if (entropy > 7.5) { // High entropy suggests binary data
        return DataFormat::BINARY;
    }

    // Default to text-based formats if not clearly binary
    return DataFormat::CSV;
}

DataFormat DataParser::detectFileFormat(const shim::string& path) const
{
    // Check file extension
    size_t dot_pos = path.find_last_of('.');
    if (dot_pos != shim::string::npos)
    {
        shim::string ext = path.substr(dot_pos + 1);
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
        
        if (ext == "json") return DataFormat::JSON;
        if (ext == "csv") return DataFormat::CSV;
        if (ext == "bin" || ext == "dat") return DataFormat::BINARY;
    }

    // Read first few bytes to detect format
    shim::ifstream file(path.c_str(), std::ios::binary);
    if (file.is_open()) {
        uint8_t buffer[1024];
        file.read(reinterpret_cast<char*>(buffer), sizeof(buffer));
        size_t bytes_read = file.gcount();
        file.close();
        
        return detectFormat(buffer, bytes_read);
    }
    
    return DataFormat::BINARY;
}

shim::vector<CandleData> DataParser::parseQuantJSON(const shim::string& path)
{
    shim::vector<CandleData> candles;
    shim::ifstream file(path.c_str());

    if (!file.is_open()) {
        shim::cerr << "Error: Could not open JSON file: " << path << std::endl;
        return candles;
    }
    
    try {
        nlohmann::json j;
        file >> j;
        
        if (!j.contains("candles") || !j["candles"].is_array()) {
            shim::cerr << "Error: JSON file does not contain 'candles' array" << std::endl;
            return candles;
        }
        
        for (const auto& candle_json : j["candles"]) {
            CandleData candle;
            
            // Parse time
            if (candle_json.contains("time") && candle_json["time"].is_string()) {
                candle.time = candle_json["time"].get<shim::string>();
            }
            
            // Parse volume
            if (candle_json.contains("volume") && candle_json["volume"].is_number()) {
                candle.volume = candle_json["volume"].get<uint64_t>();
            }
            
            // Parse mid prices (OHLC)
            if (candle_json.contains("mid") && candle_json["mid"].is_object()) {
                const auto& mid = candle_json["mid"];
                
                if (mid.contains("o") && mid["o"].is_string()) {
                    candle.open = std::stof(mid["o"].get<shim::string>());
                }
                if (mid.contains("h") && mid["h"].is_string()) {
                    candle.high = std::stof(mid["h"].get<shim::string>());
                }
                if (mid.contains("l") && mid["l"].is_string()) {
                    candle.low = std::stof(mid["l"].get<shim::string>());
                }
                if (mid.contains("c") && mid["c"].is_string()) {
                    candle.close = std::stof(mid["c"].get<shim::string>());
                }
            }
            
            candles.push_back(candle);
        }
        
    } catch (const std::exception& e) {
        shim::cerr << "Error parsing JSON: " << e.what() << std::endl;
    }
    
    file.close();
    return candles;
}

shim::vector<Pattern> DataParser::candlesToPatterns(const shim::vector<CandleData>& candles)
{
    shim::vector<Pattern> patterns;

    for (const auto& candle : candles) {
        Pattern pattern;
        
        // Use timestamp as unique ID
        pattern.id = shim::to_string(parseTimestamp(candle.time));

        // Map OHLC to position vector (raw data, no normalization)
        pattern.position.x = candle.open;
        pattern.position.y = candle.high;
        pattern.position.z = candle.low;
        pattern.position.w = candle.close;
        
        // Store volume in data vector
        pattern.data.push_back(static_cast<float>(candle.volume));
        
        // Set timestamp
        pattern.timestamp = parseTimestamp(candle.time);
        pattern.last_accessed = pattern.timestamp;
        pattern.last_modified = pattern.timestamp;
        
        // Initialize quantum state with defaults - let the quantum algorithms determine these
        pattern.generation = 0;
        pattern.coherence = 0.0f;  // Will be calculated by QBSA/QFH
        pattern.quantum_state = quantum::QuantumState{};  // Default initialized
        
        // Initialize other required fields with defaults
        pattern.velocity = glm::vec4(0.0f);
        pattern.attributes = glm::vec4(0.0f);
        pattern.amplitude = std::complex<float>(1.0f, 0.0f);
        pattern.momentum = glm::vec3(0.0f);
        
        patterns.push_back(pattern);
    }
    
    return patterns;
}

uint64_t DataParser::parseTimestamp(const shim::string& timestamp) const
{
    // Parse ISO 8601 format with nanoseconds: "2021-04-07T00:00:00.000000000Z"
    std::tm tm = {};
    shim::istringstream ss(timestamp.c_str());

    // Parse up to seconds
    ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S");
    
    if (!ss.fail()) {
        // Skip the fractional seconds part
        shim::string fractional;
        std::getline(ss, fractional, 'Z');
        
        auto tp = std::chrono::system_clock::from_time_t(std::mktime(&tm));
        return std::chrono::duration_cast<std::chrono::milliseconds>(
            tp.time_since_epoch()).count();
    }
    
    // If parsing fails, return current time
    auto now = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()).count();
}

} // namespace sep