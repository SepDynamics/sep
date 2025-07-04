#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <glm/vec3.hpp>

// Include SEP engine headers
#include "sep_engine_wrapper.h"
#include "quantum/quantum_processor_qfh.h"
#include "quantum/qbsa_qfh.h"

using json = nlohmann::json;

// Function to convert JSON data to pattern vectors
std::vector<glm::vec3> convertJsonToPatterns(const json& jsonData) {
    std::vector<glm::vec3> patterns;
    
    // Check if the JSON is an array
    if (jsonData.is_array()) {
        std::cout << "Processing JSON array with " << jsonData.size() << " elements\n";
        
        // Process each element in the array
        for (const auto& item : jsonData) {
            // Try to extract a pattern vector from each item
            try {
                // Assuming each item has x, y, z components or can be converted to them
                float x = 0.5f, y = 0.5f, z = 0.5f; // Default values
                
                // Try different possible formats
                if (item.is_array() && item.size() >= 3) {
                    // Direct array format [x, y, z]
                    x = item[0].get<float>();
                    y = item[1].get<float>();
                    z = item[2].get<float>();
                } 
                else if (item.is_object()) {
                    // Object format with named fields
                    if (item.contains("x") && item.contains("y") && item.contains("z")) {
                        x = item["x"].get<float>();
                        y = item["y"].get<float>();
                        z = item["z"].get<float>();
                    }
                    else if (item.contains("vector") && item["vector"].is_array() && item["vector"].size() >= 3) {
                        x = item["vector"][0].get<float>();
                        y = item["vector"][1].get<float>();
                        z = item["vector"][2].get<float>();
                    }
                    else if (item.contains("embedding") && item["embedding"].is_array()) {
                        // Take the first 3 elements of the embedding if available
                        auto& embedding = item["embedding"];
                        if (embedding.size() >= 3) {
                            x = embedding[0].get<float>();
                            y = embedding[1].get<float>();
                            z = embedding[2].get<float>();
                        }
                        else if (embedding.size() >= 1) {
                            // If fewer than 3 elements, use what's available and default the rest
                            x = embedding[0].get<float>();
                            if (embedding.size() >= 2) {
                                y = embedding[1].get<float>();
                            }
                        }
                    }
                    // Add more formats as needed based on your JSON structure
                }
                
                // Normalize values to [0,1] range if needed
                x = std::max(0.0f, std::min(1.0f, x));
                y = std::max(0.0f, std::min(1.0f, y));
                z = std::max(0.0f, std::min(1.0f, z));
                
                // Add the pattern to our collection
                patterns.push_back(glm::vec3(x, y, z));
                
                // Print progress every 1000 patterns
                if (patterns.size() % 1000 == 0) {
                    std::cout << "Processed " << patterns.size() << " patterns\n";
                }
            }
            catch (const std::exception& e) {
                std::cerr << "Error processing JSON item: " << e.what() << std::endl;
                // Continue with the next item
            }
        }
    }
    else if (jsonData.is_object()) {
        std::cout << "Processing JSON object\n";
        
        // Check for common object structures
        if (jsonData.contains("patterns") && jsonData["patterns"].is_array()) {
            return convertJsonToPatterns(jsonData["patterns"]);
        }
        else if (jsonData.contains("embeddings") && jsonData["embeddings"].is_array()) {
            return convertJsonToPatterns(jsonData["embeddings"]);
        }
        else if (jsonData.contains("data") && jsonData["data"].is_array()) {
            return convertJsonToPatterns(jsonData["data"]);
        }
        else if (jsonData.contains("candles") && jsonData["candles"].is_array()) {
            // Handle Oanda ticker data format
            std::cout << "Processing Oanda ticker data with " << jsonData["candles"].size() << " candles\n";
            std::vector<glm::vec3> patterns;
            
            for (const auto& candle : jsonData["candles"]) {
                try {
                    if (candle.contains("mid")) {
                        // Extract OHLC values and convert to float
                        float open = std::stof(candle["mid"]["o"].get<std::string>());
                        float high = std::stof(candle["mid"]["h"].get<std::string>());
                        float low = std::stof(candle["mid"]["l"].get<std::string>());
                        float close = std::stof(candle["mid"]["c"].get<std::string>());
                        int volume = candle["volume"].get<int>();
                        
                        // Extract more meaningful features from the candle data
                        
                        // 1. Candle body size (relative to full range)
                        float bodySize = std::abs(close - open);
                        
                        // 2. Candle direction (bullish or bearish)
                        float direction = close > open ? 1.0f : (close < open ? -1.0f : 0.0f);
                        
                        // 3. Upper shadow length
                        float upperShadow = high - std::max(open, close);
                        
                        // 4. Lower shadow length
                        float lowerShadow = std::min(open, close) - low;
                        
                        // 5. Position within range (where did it close relative to high-low range)
                        float positionInRange = 0.0f;
                        float range = high - low;
                        
                        if (range > 0) {
                            positionInRange = (close - low) / range;
                        }
                        
                        // 6. Volatility (high-low range relative to open)
                        float volatility = 0.0f;
                        if (open > 0) {
                            volatility = range / open;
                        }
                        
                        // Create multiple pattern vectors to capture different aspects of the data
                        
                        // Pattern 1: Basic price action
                        // x: position in range (0-1)
                        // y: body size relative to range (0-1)
                        // z: direction (-1 to 1, normalized to 0-1)
                        float x1 = positionInRange;
                        float y1 = range > 0 ? bodySize / range : 0.5f;
                        float z1 = (direction + 1.0f) / 2.0f; // Normalize from [-1,1] to [0,1]
                        patterns.push_back(glm::vec3(x1, y1, z1));
                        
                        // Pattern 2: Shadow analysis
                        // x: upper shadow relative to range
                        // y: lower shadow relative to range
                        // z: volume (normalized)
                        float x2 = range > 0 ? upperShadow / range : 0.5f;
                        float y2 = range > 0 ? lowerShadow / range : 0.5f;
                        float z2 = std::min(1.0f, volume / 100.0f); // Normalize volume, cap at 1.0
                        patterns.push_back(glm::vec3(x2, y2, z2));
                        
                        // Pattern 3: Volatility and momentum
                        // x: volatility (normalized)
                        // y: momentum (close relative to open, normalized)
                        // z: range relative to previous close (if available)
                        float x3 = std::min(1.0f, volatility * 10.0f); // Scale and cap volatility
                        float y3 = 0.5f;
                        if (open > 0) {
                            y3 = 0.5f + ((close - open) / open) * 5.0f; // Center at 0.5, scale
                            y3 = std::max(0.0f, std::min(1.0f, y3)); // Clamp to [0,1]
                        }
                        float z3 = 0.5f; // Default value
                        patterns.push_back(glm::vec3(x3, y3, z3));
                        
                        // Print progress every 1000 patterns
                        if (patterns.size() % 1000 == 0) {
                            std::cout << "Processed " << patterns.size() << " candles\n";
                        }
                    }
                } catch (const std::exception& e) {
                    std::cerr << "Error processing candle: " << e.what() << std::endl;
                }
            }
            
            return patterns;
        }
        else {
            std::cerr << "JSON object format not recognized\n";
        }
    }
    else {
        std::cerr << "Unsupported JSON format\n";
    }
    
    return patterns;
}

int main(int argc, char* argv[]) {
    // Default file path
    std::string filePath = "examples/train_data_2021_to_2025.json";
    
    // Allow overriding the file path from command line
    if (argc > 1) {
        filePath = argv[1];
    }
    
    std::cout << "Processing JSON file: " << filePath << std::endl;
    
    // Load and parse the JSON file
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return 1;
    }
    
    json jsonData;
    try {
        // Parse the JSON file
        file >> jsonData;
        file.close();
        
        std::cout << "JSON file loaded successfully\n";
        
        // Convert JSON data to pattern vectors
        std::vector<glm::vec3> patterns = convertJsonToPatterns(jsonData);
        
        std::cout << "Converted " << patterns.size() << " patterns from JSON\n";
        
        if (patterns.empty()) {
            std::cerr << "No patterns were extracted from the JSON file\n";
            return 1;
        }
        
        // Initialize the SEP engine
        sep::core::Engine engine;
        sep::config::APIConfig config;
        if (!engine.init(config)) {
            std::cerr << "Failed to initialize SEP engine\n";
            return 1;
        }
        
        // Create a quantum processor with default config
        sep::quantum::ProcessingConfig processorConfig;
        auto processor = sep::quantum::createProcessor(processorConfig);
        
        // Initialize quantum state
        sep::quantum::QuantumState initialState;
        initialState.coherence = 0.5f;
        initialState.stability = 0.5f;
        initialState.evolution_rate = 0.1f;
        
        // Process patterns in batches to avoid memory issues
        const size_t batchSize = 1000;
        size_t totalPatterns = patterns.size();
        size_t processedCount = 0;
        
        std::cout << "Processing patterns through SEP engine...\n";
        
        // Process each batch
        for (size_t i = 0; i < totalPatterns; i += batchSize) {
            size_t currentBatchSize = std::min(batchSize, totalPatterns - i);
            
            // Process each pattern in the current batch
            for (size_t j = 0; j < currentBatchSize; ++j) {
                const glm::vec3& vec = patterns[i + j];
                
                // Convert glm::vec3 to std::vector<float>
                std::vector<float> patternVector = {vec.x, vec.y, vec.z};
                
                // Create a pattern object
                sep::quantum::Pattern sepPattern;
                sepPattern.id = "pattern_" + std::to_string(processedCount);
                sepPattern.data = patternVector;
                
                // Initialize the quantum state for the pattern
                sepPattern.quantum_state = initialState;
                
                // Add the pattern to the processor
                processor->addPattern(sepPattern);
                
                // Process and evolve the pattern
                processor->processPattern(sepPattern.id);
                processor->evolvePattern(sepPattern.id);
                
                processedCount++;
            }
            
            // Print progress
            std::cout << "Processed " << processedCount << " of " << totalPatterns << " patterns\n";
            
            // Get all patterns from the processor
            auto patterns = processor->getPatterns();
            
            // Print information about the patterns
            std::cout << "Current processor has " << patterns.size() << " patterns\n";
            
            // If we have some patterns, print a few examples
            if (!patterns.empty()) {
                std::cout << "Sample patterns:\n";
                size_t sampleCount = std::min(size_t(5), patterns.size());
                int count = 0;
                
                // Iterate through the patterns
                for (const auto& pattern : patterns) {
                    if (count >= sampleCount) break;
                    
                    std::cout << "  Pattern " << pattern.id << ": (";
                    if (!pattern.data.empty()) {
                        for (size_t i = 0; i < std::min(pattern.data.size(), size_t(3)); ++i) {
                            std::cout << pattern.data[i];
                            if (i < std::min(pattern.data.size(), size_t(3)) - 1) {
                                std::cout << ", ";
                            }
                        }
                    }
                    std::cout << ") - State: [coherence: " << pattern.quantum_state.coherence
                              << ", stability: " << pattern.quantum_state.stability
                              << ", gen: " << pattern.quantum_state.generation
                              << "]\n";
                    count++;
                }
            }
            
            std::cout << "-----------------------------------\n";
        }
        
        // Get all patterns from the processor for final results
        auto allPatterns = processor->getPatterns();
        
        std::cout << "\nFinal Results:\n";
        std::cout << "Total patterns processed: " << processedCount << std::endl;
        std::cout << "Total patterns in processor: " << allPatterns.size() << std::endl;
        
        // Print some of the patterns
        size_t patternsToPrint = std::min(size_t(10), allPatterns.size());
        std::cout << "Sample patterns:\n";
        int count = 0;
        
        // Iterate through the patterns
        for (const auto& pattern : allPatterns) {
            if (count >= patternsToPrint) break;
            
            std::cout << "Pattern " << pattern.id << ": (";
            if (!pattern.data.empty()) {
                for (size_t i = 0; i < std::min(pattern.data.size(), size_t(3)); ++i) {
                    std::cout << pattern.data[i];
                    if (i < std::min(pattern.data.size(), size_t(3)) - 1) {
                        std::cout << ", ";
                    }
                }
            }
            std::cout << ")\n";
            count++;
        }
        
        std::cout << "\nProcessing complete!\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Error processing JSON: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}