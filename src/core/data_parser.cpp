#include "data_parser.h"
#include "core/types.h"
#include "core/common.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <cstring>
#include <algorithm>
#include <nlohmann/json.hpp>

namespace sep {

// Parse from file (auto-detects format)
std::vector<Pattern> DataParser::parseFile(const std::string& path, DataFormat format) {
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
            std::ifstream file(path, std::ios::binary);
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
std::vector<Pattern> DataParser::parseBuffer(const uint8_t* data, size_t size, DataFormat format) {
    if (format == DataFormat::AUTO) {
        format = detectFormat(data, size);
    }
    
    switch (format) {
        case DataFormat::JSON:
        case DataFormat::CANDLE: {
            // Convert buffer to string for JSON parsing
            std::string json_str(reinterpret_cast<const char*>(data), size);
            try {
                nlohmann::json j = nlohmann::json::parse(json_str);
                std::vector<CandleData> candles;
                
                if (j.contains("candles") && j["candles"].is_array()) {
                    for (const auto& candle_json : j["candles"]) {
                        CandleData candle;
                        
                        if (candle_json.contains("time") && candle_json["time"].is_string()) {
                            candle.time = candle_json["time"].get<std::string>();
                        }
                        
                        if (candle_json.contains("volume") && candle_json["volume"].is_number()) {
                            candle.volume = candle_json["volume"].get<uint64_t>();
                        }
                        
                        if (candle_json.contains("mid") && candle_json["mid"].is_object()) {
                            const auto& mid = candle_json["mid"];
                            
                            if (mid.contains("o") && mid["o"].is_string()) {
                                candle.open = std::stof(mid["o"].get<std::string>());
                            }
                            if (mid.contains("h") && mid["h"].is_string()) {
                                candle.high = std::stof(mid["h"].get<std::string>());
                            }
                            if (mid.contains("l") && mid["l"].is_string()) {
                                candle.low = std::stof(mid["l"].get<std::string>());
                            }
                            if (mid.contains("c") && mid["c"].is_string()) {
                                candle.close = std::stof(mid["c"].get<std::string>());
                            }
                        }
                        
                        candles.push_back(candle);
                    }
                }
                
                return candlesToPatterns(candles);
            } catch (const std::exception& e) {
                std::cerr << "Error parsing JSON from buffer: " << e.what() << std::endl;
                return {};
            }
        }
        case DataFormat::BINARY:
            return parseBinary(data, size);
        default:
            std::cerr << "Error: Unsupported format for buffer parsing" << std::endl;
            return {};
    }
}

// Parse from stream (maintains state for continuous data)
std::vector<Pattern> DataParser::parseStream(std::istream& stream, DataFormat format) {
    if (!stream_state_) {
        stream_state_ = std::make_unique<StreamState>();
    }
    
    // Read available data from stream
    std::vector<uint8_t> chunk;
    char ch;
    while (stream.get(ch)) {
        chunk.push_back(static_cast<uint8_t>(ch));
    }
    
    // Append to buffer
    stream_state_->buffer.insert(stream_state_->buffer.end(), chunk.begin(), chunk.end());
    
    // Try to parse complete patterns from buffer
    std::vector<Pattern> patterns;
    
    if (format == DataFormat::AUTO) {
        format = detectFormat(stream_state_->buffer.data(), stream_state_->buffer.size());
    }
    
    // For now, simple implementation - parse when we have enough data
    // In production, this would need more sophisticated boundary detection
    if (stream_state_->buffer.size() > 1024) {  // Arbitrary threshold
        patterns = parseBuffer(stream_state_->buffer.data(), stream_state_->buffer.size(), format);
        stream_state_->buffer.clear();  // Clear after successful parse
    }
    
    return patterns;
}

// Parse CSV file
std::vector<Pattern> DataParser::parseCSV(const std::string& path) {
    std::vector<Pattern> patterns;
    std::ifstream file(path);
    
    if (!file.is_open()) {
        std::cerr << "Error: Could not open CSV file: " << path << std::endl;
        return patterns;
    }
    
    std::string line;
    int line_num = 0;
    
    while (std::getline(file, line)) {
        line_num++;
        
        // Skip empty lines
        if (line.empty()) continue;
        
        // Simple CSV parsing - split by comma
        std::stringstream ss(line);
        std::vector<float> values;
        std::string field;
        
        while (std::getline(ss, field, ',')) {
            try {
                values.push_back(std::stof(field));
            } catch (...) {
                // If not a number, skip
            }
        }
        
        if (!values.empty()) {
            Pattern pattern;
            pattern.id = std::to_string(line_num);
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
std::vector<Pattern> DataParser::parseBinary(const uint8_t* data, size_t size) {
    std::vector<Pattern> patterns;
    
    // Treat binary data as raw float arrays
    // Each pattern needs at least 4 floats for position
    const size_t floats_per_pattern = 4;
    const size_t bytes_per_pattern = floats_per_pattern * sizeof(float);
    
    if (size < bytes_per_pattern) {
        return patterns;
    }
    
    size_t num_patterns = size / bytes_per_pattern;
    const float* float_data = reinterpret_cast<const float*>(data);
    
    for (size_t i = 0; i < num_patterns; ++i) {
        Pattern pattern;
        pattern.id = std::to_string(i);
        pattern.timestamp = std::chrono::system_clock::now().time_since_epoch().count();
        
        // Map binary floats directly to position
        size_t offset = i * floats_per_pattern;
        pattern.position.x = float_data[offset + 0];
        pattern.position.y = float_data[offset + 1];
        pattern.position.z = float_data[offset + 2];
        pattern.position.w = float_data[offset + 3];
        
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
    
    return patterns;
}

// Convert patterns to PinStates for engine compatibility
std::vector<PinState> DataParser::toPinStates(const std::vector<Pattern>& patterns) {
    std::vector<PinState> pin_states;
    
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
        // Simple check for "candles" keyword
        std::string str(reinterpret_cast<const char*>(data), std::min(size, size_t(100)));
        if (str.find("candles") != std::string::npos) {
            return DataFormat::CANDLE;
        }
        return DataFormat::JSON;
    }
    
    // Check for CSV (contains commas and newlines)
    bool has_comma = false;
    bool has_newline = false;
    for (size_t i = 0; i < std::min(size, size_t(1000)); ++i) {
        if (data[i] == ',') has_comma = true;
        if (data[i] == '\n') has_newline = true;
        if (has_comma && has_newline) return DataFormat::CSV;
    }
    
    // Default to binary
    return DataFormat::BINARY;
}

DataFormat DataParser::detectFileFormat(const std::string& path) const {
    // Check file extension
    size_t dot_pos = path.find_last_of('.');
    if (dot_pos != std::string::npos) {
        std::string ext = path.substr(dot_pos + 1);
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
        
        if (ext == "json") return DataFormat::JSON;
        if (ext == "csv") return DataFormat::CSV;
        if (ext == "bin" || ext == "dat") return DataFormat::BINARY;
    }
    
    // Read first few bytes to detect format
    std::ifstream file(path, std::ios::binary);
    if (file.is_open()) {
        uint8_t buffer[1024];
        file.read(reinterpret_cast<char*>(buffer), sizeof(buffer));
        size_t bytes_read = file.gcount();
        file.close();
        
        return detectFormat(buffer, bytes_read);
    }
    
    return DataFormat::BINARY;
}

std::vector<CandleData> DataParser::parseQuantJSON(const std::string& path) {
    std::vector<CandleData> candles;
    std::ifstream file(path);
    
    if (!file.is_open()) {
        std::cerr << "Error: Could not open JSON file: " << path << std::endl;
        return candles;
    }
    
    try {
        nlohmann::json j;
        file >> j;
        
        if (!j.contains("candles") || !j["candles"].is_array()) {
            std::cerr << "Error: JSON file does not contain 'candles' array" << std::endl;
            return candles;
        }
        
        for (const auto& candle_json : j["candles"]) {
            CandleData candle;
            
            // Parse time
            if (candle_json.contains("time") && candle_json["time"].is_string()) {
                candle.time = candle_json["time"].get<std::string>();
            }
            
            // Parse volume
            if (candle_json.contains("volume") && candle_json["volume"].is_number()) {
                candle.volume = candle_json["volume"].get<uint64_t>();
            }
            
            // Parse mid prices (OHLC)
            if (candle_json.contains("mid") && candle_json["mid"].is_object()) {
                const auto& mid = candle_json["mid"];
                
                if (mid.contains("o") && mid["o"].is_string()) {
                    candle.open = std::stof(mid["o"].get<std::string>());
                }
                if (mid.contains("h") && mid["h"].is_string()) {
                    candle.high = std::stof(mid["h"].get<std::string>());
                }
                if (mid.contains("l") && mid["l"].is_string()) {
                    candle.low = std::stof(mid["l"].get<std::string>());
                }
                if (mid.contains("c") && mid["c"].is_string()) {
                    candle.close = std::stof(mid["c"].get<std::string>());
                }
            }
            
            candles.push_back(candle);
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error parsing JSON: " << e.what() << std::endl;
    }
    
    file.close();
    return candles;
}

std::vector<Pattern> DataParser::candlesToPatterns(const std::vector<CandleData>& candles) {
    std::vector<Pattern> patterns;
    
    for (const auto& candle : candles) {
        Pattern pattern;
        
        // Use timestamp as unique ID
        pattern.id = std::to_string(parseTimestamp(candle.time));
        
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

uint64_t DataParser::parseTimestamp(const std::string& timestamp) const {
    // Parse ISO 8601 format with nanoseconds: "2021-04-07T00:00:00.000000000Z"
    std::tm tm = {};
    std::istringstream ss(timestamp);
    
    // Parse up to seconds
    ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S");
    
    if (!ss.fail()) {
        // Skip the fractional seconds part
        std::string fractional;
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