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
#include <nlohmann/json.hpp>

namespace fs = std::filesystem;
using namespace sep::quantum;
using json = nlohmann::json;

// Forward declarations
void printMetrics(std::ostream& out, const std::string& dataType, const std::vector<PatternMetrics>& metrics);
void printJsonMetrics(std::ostream& out, const std::string& dataType, const std::vector<PatternMetrics>& metrics, const PatternMetricEngine& engine);
void processFile(PatternMetricEngine& engine, const fs::path& filePath, int iterations, bool no_clear, bool json_output, std::ostream& out);
void runOriginalExamples(PatternMetricEngine& engine, bool json_output, std::ostream& out);

// Helper function to print metrics in human-readable format
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

// Helper function to print metrics in JSON format
void printJsonMetrics(std::ostream& out, const std::string& dataType, const std::vector<PatternMetrics>& metrics, const PatternMetricEngine& engine) {
    if (metrics.empty()) {
        out << "[]" << std::endl;
        return;
    }

    json result = json::array();
    const auto& patterns = engine.getPatterns();
    
    for (size_t i = 0; i < metrics.size() && i < patterns.size(); ++i) {
        const auto& m = metrics[i];
        const auto& pattern = patterns[i];
        json pattern_obj = {
            {"id", std::string(pattern.id)},
            {"coherence", m.coherence},
            {"stability", m.stability},
            {"entropy", m.entropy}
        };
        result.push_back(pattern_obj);
    }

    out << result.dump(4) << std::endl;
}

void processFile(PatternMetricEngine& engine, const fs::path& filePath, int iterations, bool no_clear, bool json_output, std::ostream& out) {
    if (!no_clear) {
        engine.clear();
    }

    if (!json_output) {
        out << "\n=== Processing File: " << filePath << " (" << iterations << "x, "
            << (no_clear ? "state retained" : "state cleared") << ") ===\n";
    }

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

        if (iterations > 1 && !json_output) {
            out << "--- After Iteration " << i + 1 << " ---";
            printMetrics(out, filePath.string(), engine.computeMetrics());
        }
    }

    if (iterations <= 1) {
        if (json_output) {
            printJsonMetrics(out, filePath.string(), engine.computeMetrics(), engine);
        } else {
            printMetrics(out, filePath.string(), engine.computeMetrics());
        }
    }
}

void runOriginalExamples(PatternMetricEngine& engine, bool json_output, std::ostream& out) {
    // This function will not produce JSON output as it's for demonstration.
    if (json_output) {
        out << "[]" << std::endl;
        return;
    }
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
}

int main(int argc, char* argv[]) {
    bool benchmark_mode = false;
    bool no_clear = false;
    bool gpu_mode = false;
    bool json_output = false;
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
        } else if (arg == "--json") {
            json_output = true;
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

    if (gpu_mode && !json_output) {
        out << "GPU mode enabled." << std::endl;
    }
    if (gpu_mode) {
        gpu_context = std::make_unique<GPUContext>();
        gpu_context->device_id = 0;
        gpu_context->initialized = true;
    }

    if (engine.init(gpu_context.get()) != sep::SEPResult::SUCCESS) {
        std::cerr << "Failed to initialize pattern metric engine\n";
        return 1;
    }

    if (target_path_str.empty()) {
        if (!json_output) out << "No file or directory specified. Running original examples." << std::endl;
        runOriginalExamples(engine, json_output, out);
        return 0;
    }

    fs::path targetPath = target_path_str;
    if (!fs::exists(targetPath)) {
        std::cerr << "Error: Path does not exist: " << targetPath << std::endl;
        return 1;
    }

    if (fs::is_directory(targetPath)) {
        if (json_output) {
            json all_results = json::array();
            for (const auto& entry : fs::recursive_directory_iterator(targetPath)) {
                if (fs::is_regular_file(entry)) {
                    std::stringstream ss;
                    engine.clear(); // Clear state for each file in directory mode
                    std::ifstream file(entry.path(), std::ios::binary);
                    if (file) {
                        const size_t internal_chunk_size = 65536;
                        std::vector<char> buffer(internal_chunk_size);
                        while (file.read(buffer.data(), internal_chunk_size) || file.gcount() > 0) {
                            engine.ingestData(reinterpret_cast<const uint8_t*>(buffer.data()), file.gcount());
                            engine.evolvePatterns();
                        }
                        printJsonMetrics(ss, entry.path().string(), engine.computeMetrics(), engine);
                    }
                    std::string json_str = ss.str();
                    if (!json_str.empty()) {
                        try {
                            json j = json::parse(json_str);
                            if (!j.empty()) {
                                all_results.push_back(j);
                            }
                        }
                        catch (json::parse_error& e) {
                            std::cerr << "Failed to parse JSON for " << entry.path() << ": " << e.what() << std::endl;
                        }
                    }
                }
            }
            out << all_results.dump(4) << std::endl;
        }
        else {
            if(no_clear) {
                out << "Processing directory with state retained between files." << std::endl;
            } else {
                out << "Processing directory with state cleared between files." << std::endl;
            }
            for (const auto& entry : fs::recursive_directory_iterator(targetPath)) {
                if (fs::is_regular_file(entry)) {
                    processFile(engine, entry.path(), iterations, no_clear, json_output, out);
                }
            }
        }
    } else {
        processFile(engine, targetPath, iterations, no_clear, json_output, out);
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

        processFile(engine, targetPath, 1, false, false, dev_null);
    }
}
BENCHMARK_CAPTURE(BM_ProcessFile, CPU, "assets/test_data/benchmark_data.txt", false);
BENCHMARK_CAPTURE(BM_ProcessFile, GPU, "assets/test_data/benchmark_data.txt", true);