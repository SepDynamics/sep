#include <iostream>
#include <string>
#include <vector>
#include "utils/dataset_loader.h"
#include <glm/vec3.hpp>

// Include SEP engine headers
#include "sep_engine_wrapper.h"
#include "quantum/quantum_processor_qfh.h"
#include "quantum/qbsa_qfh.h"


int main(int argc, char* argv[]) {
    // Default file path
    std::string filePath = "examples/train_data_2021_to_2025.json";
    
    // Allow overriding the file path from command line
    if (argc > 1) {
        filePath = argv[1];
    }
    
    std::cout << "Processing dataset file: " << filePath << std::endl;

    std::vector<glm::vec3> patterns;
    try {
        patterns = sep::utils::loadDataset(filePath);
        std::cout << "Loaded " << patterns.size() << " patterns\n";

        if (patterns.empty()) {
            std::cerr << "No patterns were extracted from the dataset\n";
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
        std::cerr << "Error processing dataset: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}