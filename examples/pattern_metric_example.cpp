/**
 * @file pattern_metric_example.cpp
 * @brief Example usage of PatternMetricEngine with file processing and benchmarking capabilities
 * 
 * This example demonstrates how to use the PatternMetricEngine to:
 * 1. Process hardcoded pattern examples (original functionality)
 * 2. Process patterns from a specified file
 * 3. Recursively process patterns from all files in a directory
 * 4. Benchmark the pattern processing performance
 * 
 * Usage:
 * - Run with no arguments: 
 *     ./pattern_metric_example
 *     Runs the original hardcoded examples
 * 
 * - Run with a file path: 
 *     ./pattern_metric_example path/to/file.txt
 *     Processes the specified file and outputs metrics
 * 
 * - Run with a directory path: 
 *     ./pattern_metric_example path/to/directory
 *     Recursively processes all files in the directory and outputs metrics for each
 * 
 * - Run benchmarks: 
 *     ./pattern_metric_example -benchmark
 *     Runs performance benchmarks on pattern processing
 *     (accepts both -benchmark and --benchmark flags)
 * 
 * Benchmark Results:
 * The benchmark measures the time required to process a sample data file,
 * providing both real time and CPU time metrics. This helps quantify the
 * performance of the pattern coherence and stability calculations, which
 * are core to the SEP Engine's quantum-inspired processing capabilities.
 * 
 * This enhanced example supports the capabilities described in the SEP Dynamics 
 * business proposal, specifically demonstrating the pattern evolution tracking
 * and coherence quantification that powers financial data analysis.
 */
#include "quantum/pattern_metric_engine.h"
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <iomanip>
#include <filesystem>
#include <fstream>
#include <benchmark/benchmark.h>

namespace fs = std::filesystem;
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

void processFile(PatternMetricEngine& engine, const fs::path& filePath) {
    engine.clear(); // Ensure a clean state for each file
    std::cout << "\n=== Processing File: " << filePath << " ===\n";
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        std::cerr << "Error opening file: " << filePath << std::endl;
        return;
    }

    std::vector<uint8_t> bytes((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    engine.ingestData(bytes.data(), bytes.size());
    engine.evolvePatterns();
    printMetrics(filePath.string(), engine.computeMetrics());
}

void runOriginalExamples(PatternMetricEngine& engine) {
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
}

int main(int argc, char* argv[]) {

    bool benchmark_mode = false;
    
    // Process arguments to find and remove our custom benchmark flag
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            if (std::string(argv[i]) == "-benchmark" || std::string(argv[i]) == "--benchmark") {
                benchmark_mode = true;
                
                // Shift all arguments after the flag down by one
                for (int j = i; j < argc - 1; j++) {
                    argv[j] = argv[j + 1];
                }
                
                // Reduce argument count
                argc--;
                break;
            }
        }
    }
    
    if (benchmark_mode) {
        
        ::benchmark::Initialize(&argc, argv);
        if (::benchmark::ReportUnrecognizedArguments(argc, argv)) return 1;
        ::benchmark::RunSpecifiedBenchmarks();
        ::benchmark::Shutdown();
        return 0;
    }

    // Create and initialize the engine
    PatternMetricEngine engine;
    if (engine.init(nullptr) != sep::SEPResult::SUCCESS) {
        std::cerr << "Failed to initialize pattern metric engine\n";
        return 1;
    }

    if (argc < 2) {
        std::cout << "No file or directory specified. Running original examples." << std::endl;
        runOriginalExamples(engine);
        return 0;
    }
    

    fs::path targetPath = argv[1];
    if (!fs::exists(targetPath)) {
        std::cerr << "Error: Path does not exist: " << targetPath << std::endl;
        return 1;
    }

    if (fs::is_directory(targetPath)) {
        for (const auto& entry : fs::recursive_directory_iterator(targetPath)) {
            if (fs::is_regular_file(entry)) {
                processFile(engine, entry.path());
            }
        }
    } else {
        processFile(engine, targetPath);
    }

    return 0;
}

static void BM_ProcessFile(benchmark::State& state, const char* filepath) {
    for (auto _ : state) {
        state.PauseTiming();
        PatternMetricEngine engine;
        engine.init(nullptr);
        fs::path targetPath(filepath);
        state.ResumeTiming();

        processFile(engine, targetPath);
    }
}
BENCHMARK_CAPTURE(BM_ProcessFile, "assets/test_data/benchmark_data.txt", "assets/test_data/benchmark_data.txt");