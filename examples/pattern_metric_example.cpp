#include "quantum/pattern_metric_engine.h"
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <iomanip>

using namespace sep::quantum;

// Helper function to print metrics
void printMetrics(const std::string& dataType, const std::vector<PatternMetrics>& metrics) {
    std::cout << "\nMetrics for " << dataType << ":\n";
    for (size_t i = 0; i < metrics.size(); ++i) {
        const auto& m = metrics[i];
        std::cout << "Pattern " << i << ":\n"
                  << "  Coherence: " << std::fixed << std::setprecision(3) << m.coherence << "\n"
                  << "  Stability: " << m.stability << "\n"
                  << "  Entropy: " << m.entropy << "\n";
    }
    std::cout << std::endl;
}

int main() {
    // Create and initialize the engine
    PatternMetricEngine engine;
    if (engine.init(nullptr) != sep::SEPResult::SUCCESS) {
        std::cerr << "Failed to initialize pattern metric engine\n";
        return 1;
    }

    // Example 1: Process binary data
    std::cout << "\n=== Processing Binary Data ===\n";
    std::vector<uint8_t> binary_data = {0x00, 0xFF, 0x80, 0x40, 0x20, 0x10};
    engine.ingestData(binary_data.data(), binary_data.size());
    engine.evolvePatterns();
    printMetrics("Binary Data", engine.computeMetrics());

    // Example 2: Process text string
    std::cout << "\n=== Processing Text Data ===\n";
    std::string text = "Hello, Pattern Metric Engine!";
    engine.ingestData(reinterpret_cast<const uint8_t*>(text.data()), text.size());
    engine.evolvePatterns();
    printMetrics("Text Data", engine.computeMetrics());

    // Example 3: Process floating point numbers
    std::cout << "\n=== Processing Numeric Data ===\n";
    std::vector<float> numbers = {1.0f, 2.5f, 3.7f, 4.2f, 5.0f};
    engine.ingestData(reinterpret_cast<const uint8_t*>(numbers.data()), 
                     numbers.size() * sizeof(float));
    engine.evolvePatterns();
    printMetrics("Numeric Data", engine.computeMetrics());

    // Example 4: Process random data stream
    std::cout << "\n=== Processing Random Stream Data ===\n";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);
    
    std::stringstream stream;
    for (int i = 0; i < 100; ++i) {
        stream.put(static_cast<char>(dis(gen)));
    }
    
    engine.ingestData(stream);
    engine.evolvePatterns();
    printMetrics("Random Stream Data", engine.computeMetrics());

    // Example 5: Pattern mutation example
    std::cout << "\n=== Pattern Mutation Example ===\n";
    auto patterns = engine.getPatterns();
    if (!patterns.empty()) {
        auto mutated = engine.mutatePattern(patterns[0]);
        std::cout << "Original pattern ID: " << patterns[0].id << "\n"
                  << "Mutated pattern ID: " << mutated.id << "\n"
                  << "Generation: " << mutated.generation << "\n"
                  << "Data size: " << mutated.data.size() << " values\n";
    }

    return 0;
}