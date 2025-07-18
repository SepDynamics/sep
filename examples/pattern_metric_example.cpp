/**
 * @file pattern_metric_example.cpp
 * @brief Example usage of PatternMetricEngine with file processing and benchmarking capabilities
 * 
 * This example demonstrates how to use the PatternMetricEngine to:
 * 1. Process hardcoded pattern examples (original functionality)
 * 2. Process patterns from a specified file
 * 3. Recursively process patterns from all files in a directory
 * 4. Benchmark the pattern processing performance
 * 5. Process data multiple times to show stateful coherence growth
 * 6. Output metrics to a file
 * 
 * Usage:
 * - Default (run original examples): 
 *     ./pattern_metric_example
 * 
 * - Process a file/directory: 
 *     ./pattern_metric_example path/to/your/data
 * 
 * - Multiple iterations (demonstrates stateful learning):
 *     ./pattern_metric_example path/to/your/data --iterations 5 --no-clear
 * 
 * - Output to file:
 *     ./pattern_metric_example path/to/your/data --output metrics.txt
 * 
 * - Run benchmarks: 
 *     ./pattern_metric_example --benchmark
 * 
 */
#include "quantum/pattern_metric_engine.h"
#include "quantum/quantum_processor.h" // For GPUContext
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

// Forward declarations
void printMetrics(std::ostream& out, const std::string& dataType, const std::vector<PatternMetrics>& metrics);
void processFile(PatternMetricEngine& engine, const fs::path& filePath, int iterations, bool no_clear, std::ostream& out);
void runOriginalExamples(PatternMetricEngine& engine, std::ostream& out);

// Helper function to print metrics to a stream
void printMetrics(std::ostream& out, const std::string& dataType, const std::vector<PatternMetrics>& metrics) {
    if (metrics.empty()) {
        out << "No metrics generated for " << dataType << std::endl;
        return;
    }
    out << "\nMetrics for " << dataType << ":\n";
    float total_coherence = 0.0f;
    float total_stability = 0.0f;
    float total_entropy = 0.0f;

    for (const auto& m : metrics) {
        total_coherence += m.coherence;
        total_stability += m.stability;
        total_entropy += m.entropy;
    }

    out << "  Average Coherence: " << std::fixed << std::setprecision(4) << (total_coherence / metrics.size()) << "\n"
        << "  Average Stability: " << std::fixed << std::setprecision(4) << (total_stability / metrics.size()) << "\n"
        << "  Average Entropy:   " << std::fixed << std::setprecision(4) << (total_entropy / metrics.size()) << "\n";
    out << "  Total Patterns:    " << metrics.size() << std::endl;
}

void processFile(PatternMetricEngine& engine, const fs::path& filePath, int iterations, bool no_clear, std::ostream& out) {
    if (!no_clear) {
        engine.clear();
    }

    out << "\n=== Processing File: " << filePath << " (" << iterations << "x, "
        << (no_clear ? "state retained" : "state cleared") << ") ===\n";

    for (int i = 0; i < iterations; ++i) {
        std::ifstream file(filePath, std::ios::binary);
        if (!file) {
            std::cerr << "Error opening file: " << filePath << std::endl;
            return;
        }

        const size_t internal_chunk_size = 65536; // 64KB internal buffer
        std::vector<char> buffer(internal_chunk_size);
        
        while (file.read(buffer.data(), internal_chunk_size) || file.gcount() > 0) {
            engine.ingestData(reinterpret_cast<const uint8_t*>(buffer.data()), file.gcount());
            engine.evolvePatterns();
        }

        if (iterations > 1) {
            out << "--- After Iteration " << i + 1 << " ---";
            printMetrics(out, filePath.string(), engine.computeMetrics());
        }
    }

    if (iterations <= 1) {
        printMetrics(out, filePath.string(), engine.computeMetrics());
    }
}

void runOriginalExamples(PatternMetricEngine& engine, std::ostream& out) {
    engine.clear();
    // Example 1: Process binary data
    out << "\n=== Processing Binary Data ===\n";
    std::vector<uint8_t> binary_data = {0x00, 0xFF, 0x80, 0x40, 0x20, 0x10};
    engine.ingestData(binary_data.data(), binary_data.size());
    engine.evolvePatterns();
    printMetrics(out, "Binary Data", engine.computeMetrics());

    engine.clear();
    // Example 2: Process text string
    out << "\n=== Processing Text Data ===\n";
    std::string text = "Hello, Pattern Metric Engine!";
    engine.ingestData(reinterpret_cast<const uint8_t*>(text.data()), text.size());
    engine.evolvePatterns();
    printMetrics(out, "Text Data", engine.computeMetrics());

    engine.clear();
    // Example 3: Process floating point numbers
    out << "\n=== Processing Numeric Data ===\n";
    std::vector<float> numbers = {1.0f, 2.5f, 3.7f, 4.2f, 5.0f};
    engine.ingestData(reinterpret_cast<const uint8_t*>(numbers.data()),
                     numbers.size() * sizeof(float));
    engine.evolvePatterns();
    printMetrics(out, "Numeric Data", engine.computeMetrics());

    engine.clear();
    // Example 4: Process random data stream
    out << "\n=== Processing Random Stream Data ===\n";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);
    
    std::stringstream stream;
    for (int i = 0; i < 100; ++i) {
        stream.put(static_cast<char>(dis(gen)));
    }
    
    engine.ingestData(stream);
    engine.evolvePatterns();
    printMetrics(out, "Random Stream Data", engine.computeMetrics());

    engine.clear();
    // Example 5: Pattern mutation example (DEACTIVATED due to performance refactoring)
    // out << "\n=== Pattern Mutation Example ===\n";
    // auto patterns = engine.getPatterns();
    // if (!patterns.empty()) {
    //     auto mutated = engine.mutatePattern(patterns[0]);
    //     out << "Original pattern ID: " << patterns[0].id << "\n"
    //         << "Mutated pattern ID: " << mutated.id << "\n"
    //         << "Generation: " << mutated.generation << "\n"
    //         << "Data size: " << mutated.data.size() << " values\n";
    // }
}

int main(int argc, char* argv[]) {
    bool benchmark_mode = false;
    bool no_clear = false;
    bool gpu_mode = false;
    int iterations = 1;
    std::string output_path;
    std::string target_path_str;

    std::vector<char*> benchmark_args;
    benchmark_args.push_back(argv[0]);

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--benchmark") {
            benchmark_mode = true;
        } else if (arg == "--gpu") {
            gpu_mode = true;
        } else if (arg == "--iterations") {
            if (i + 1 < argc) {
                iterations = std::stoi(argv[++i]);
            } else {
                std::cerr << "--iterations requires a number" << std::endl;
                return 1;
            }
        } else if (arg == "--output") {
            if (i + 1 < argc) {
                output_path = argv[++i];
            } else {
                std::cerr << "--output requires a filepath" << std::endl;
                return 1;
            }
        } else if (arg == "--no-clear") {
            no_clear = true;
        } else if (arg.rfind("--", 0) == 0) {
            benchmark_args.push_back(argv[i]);
        }
        else {
            if (target_path_str.empty()) {
                target_path_str = arg;
            } else {
                 benchmark_args.push_back(argv[i]);
            }
        }
    }
    
    if (benchmark_mode) {
        int bench_argc = benchmark_args.size();
        ::benchmark::Initialize(&bench_argc, benchmark_args.data());
        if (::benchmark::ReportUnrecognizedArguments(bench_argc, benchmark_args.data())) return 1;
        ::benchmark::RunSpecifiedBenchmarks();
        ::benchmark::Shutdown();
        return 0;
    }

    std::ofstream outfile;
    if (!output_path.empty()) {
        outfile.open(output_path);
        if (!outfile) {
            std::cerr << "Error opening output file: " << output_path << std::endl;
            return 1;
        }
    }
    std::ostream& out = outfile.is_open() ? outfile : std::cout;

    PatternMetricEngine engine;
    std::unique_ptr<GPUContext> gpu_context;

    if (gpu_mode) {
        out << "GPU mode enabled." << std::endl;
        gpu_context = std::make_unique<GPUContext>();
        // Simple initialization, assuming device 0
        gpu_context->device_id = 0;
        gpu_context->initialized = true;
    }

    if (engine.init(gpu_context.get()) != sep::SEPResult::SUCCESS) {
        std::cerr << "Failed to initialize pattern metric engine\n";
        return 1;
    }

    if (target_path_str.empty()) {
        out << "No file or directory specified. Running original examples." << std::endl;
        runOriginalExamples(engine, out);
        return 0;
    }

    fs::path targetPath = target_path_str;
    if (!fs::exists(targetPath)) {
        std::cerr << "Error: Path does not exist: " << targetPath << std::endl;
        return 1;
    }

    if (fs::is_directory(targetPath)) {
        if(no_clear) {
            out << "Processing directory with state retained between files." << std::endl;
        } else {
            out << "Processing directory with state cleared between files." << std::endl;
        }
        for (const auto& entry : fs::recursive_directory_iterator(targetPath)) {
            if (fs::is_regular_file(entry)) {
                processFile(engine, entry.path(), iterations, no_clear, out);
            }
        }
    } else {
        processFile(engine, targetPath, iterations, no_clear, out);
    }

    return 0;
}

static void BM_ProcessFile(benchmark::State& state, const char* filepath, bool use_gpu) {
    for (auto _ : state) {
        state.PauseTiming();
        PatternMetricEngine engine;
        std::unique_ptr<GPUContext> gpu_context;
        if (use_gpu) {
            gpu_context = std::make_unique<GPUContext>();
            gpu_context->device_id = 0;
            gpu_context->initialized = true;
        }
        engine.init(gpu_context.get());
        fs::path targetPath(filepath);
        std::ofstream dev_null("/dev/null");
        state.ResumeTiming();

        processFile(engine, targetPath, 1, false, dev_null);
    }
}
BENCHMARK_CAPTURE(BM_ProcessFile, CPU, "assets/test_data/benchmark_data.txt", false);
BENCHMARK_CAPTURE(BM_ProcessFile, GPU, "assets/test_data/benchmark_data.txt", true);