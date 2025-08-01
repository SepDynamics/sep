#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <filesystem>
#include <chrono>
#include <iomanip>
#include <sstream>

// SEP Engine includes
#include "connectors/market_data_converter.h"
#include "engine/internal/standard_includes.h"
#include "quantum/config.h"
#include "quantum/pattern_metric_engine.h"

// JSON support
#include <nlohmann/json.hpp>

using json = nlohmann::json;
namespace fs = std::filesystem;

struct AnalysisResult {
    double coherence = 0.0;
    double stability = 0.0;
    double entropy = 0.0;
    double energy = 0.0;
    size_t pattern_count = 0;
    std::string timestamp;
};

class PatternMetricAnalyzer {
private:
    std::unique_ptr<sep::quantum::PatternMetricEngine> engine_;
    bool json_output_ = false;
    bool clear_state_ = true;
    
public:
    PatternMetricAnalyzer() {
        engine_ = std::make_unique<sep::quantum::PatternMetricEngine>();
        
        // Initialize the engine (passing nullptr for CPU-only operation)
        auto result = engine_->init(nullptr);
        if (result != sep::SEPResult::SUCCESS) {
            throw std::runtime_error("Failed to initialize PatternMetricEngine");
        }
    }
    
    void setJsonOutput(bool enable) { json_output_ = enable; }
    void setClearState(bool clear) { clear_state_ = clear; }
    
    AnalysisResult analyzeData(const std::vector<uint8_t>& data, double confidence_threshold = 0.0) {
        if (clear_state_) {
            engine_->clear();
        }
        
        // Ingest and process the data through the engine
        engine_->ingestData(data.data(), data.size());
        engine_->evolvePatterns();
        
        // Get metrics
        const auto& metrics = engine_->computeMetrics();
        
        AnalysisResult result;
        
        // Filter metrics by confidence
        std::vector<sep::quantum::PatternMetrics> filtered_metrics;
        if (confidence_threshold > 0.0) {
            for (const auto& metric : metrics) {
                if (metric.coherence >= confidence_threshold) {
                    filtered_metrics.push_back(metric);
                }
            }
        } else {
            filtered_metrics = metrics;
        }

        // Calculate aggregate metrics from all patterns
        if (!filtered_metrics.empty()) {
            double total_coherence = 0.0, total_stability = 0.0, total_entropy = 0.0, total_energy = 0.0;
            for (const auto& metric : filtered_metrics) {
                total_coherence += metric.coherence;
                total_stability += metric.stability;
                total_entropy += metric.entropy;
                total_energy += metric.energy;
            }
            result.coherence = total_coherence / filtered_metrics.size();
            result.stability = total_stability / filtered_metrics.size();
            result.entropy = total_entropy / filtered_metrics.size();
            result.energy = total_energy / filtered_metrics.size();
        }
        result.pattern_count = filtered_metrics.size();
        
        // Get current timestamp
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()) % 1000;
        
        std::ostringstream oss;
        oss << std::put_time(std::gmtime(&time_t), "%Y-%m-%dT%H:%M:%S");
        oss << '.' << std::setfill('0') << std::setw(3) << ms.count() << 'Z';
        result.timestamp = oss.str();
        
        return result;
    }
    
    AnalysisResult analyzeOandaFile(const std::string& filepath, double confidence_threshold = 0.0) {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file: " + filepath);
        }
        
        json oanda_data;
        file >> oanda_data;
        
        // Extract price data from OANDA JSON and convert to byte stream
        std::vector<double> prices;
        if (oanda_data.contains("candles")) {
            for (const auto& candle : oanda_data["candles"]) {
                if (candle.contains("mid") && candle["mid"].contains("c")) {
                    prices.push_back(std::stod(candle["mid"]["c"].get<std::string>()));
                }
            }
        }
        
        // Convert prices to byte stream for analysis
        auto raw_data = sep::connectors::MarketDataConverter::convertToBitstream(prices);
        
        return analyzeData(raw_data, confidence_threshold);
    }
    
    void outputResult(const AnalysisResult& result) {
        if (json_output_) {
            json output;
            output["timestamp"] = result.timestamp;
            output["metrics"]["coherence"] = result.coherence;
            output["metrics"]["stability"] = result.stability;
            output["metrics"]["entropy"] = result.entropy;
            output["metrics"]["energy"] = result.energy;
            output["pattern_count"] = result.pattern_count;
            
            std::cout << output.dump(2) << std::endl;
        } else {
            std::cout << "=== Pattern Metrics Analysis ===" << std::endl;
            std::cout << "Timestamp: " << result.timestamp << std::endl;
            std::cout << "Coherence: " << std::fixed << std::setprecision(6) << result.coherence << std::endl;
            std::cout << "Stability: " << std::fixed << std::setprecision(6) << result.stability << std::endl;
            std::cout << "Entropy: " << std::fixed << std::setprecision(6) << result.entropy << std::endl;
            std::cout << "Energy: " << std::fixed << std::setprecision(6) << result.energy << std::endl;
            std::cout << "Pattern Count: " << result.pattern_count << std::endl;
        }
    }
};

void printUsage(const char* program_name) {
    std::cout << "Usage: " << program_name << " [OPTIONS] <input_file_or_directory>" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  --json        Output results in JSON format" << std::endl;
    std::cout << "  --no-clear    Don't clear state between analyses (for stateful processing)" << std::endl;
    std::cout << "  --confidence  Minimum confidence threshold (e.g., 0.7)" << std::endl;
    std::cout << "  --help        Show this help message" << std::endl;
    std::cout << std::endl;
    std::cout << "Examples:" << std::endl;
    std::cout << "  " << program_name << " Testing/OANDA/sample_48h.json" << std::endl;
    std::cout << "  " << program_name << " --json Testing/OANDA/" << std::endl;
    std::cout << "  " << program_name << " --json --no-clear data.bin" << std::endl;
}

int main(int argc, char* argv[]) {
    try {
        std::vector<std::string> args(argv + 1, argv + argc);
        
        if (args.empty() || 
            std::find(args.begin(), args.end(), "--help") != args.end()) {
            printUsage(argv[0]);
            return 0;
        }
        
        PatternMetricAnalyzer analyzer;
        
        // Parse options
        bool json_output = false;
        bool no_clear = false;
        double confidence_threshold = 0.0;
        std::string input_path;
        
        for (size_t i = 0; i < args.size(); ++i) {
            if (args[i] == "--json") {
                json_output = true;
            } else if (args[i] == "--no-clear") {
                no_clear = true;
            } else if (args[i] == "--confidence") {
                if (i + 1 < args.size()) {
                    confidence_threshold = std::stod(args[++i]);
                }
            } else if (args[i][0] != '-') {
                input_path = args[i];
            }
        }
        
        if (input_path.empty()) {
            std::cerr << "Error: No input file or directory specified" << std::endl;
            return 1;
        }
        
        analyzer.setJsonOutput(json_output);
        analyzer.setClearState(!no_clear);
        
        // Check if input is a file or directory
        fs::path path(input_path);
        
        if (fs::is_regular_file(path)) {
            // Analyze single file
            auto result = analyzer.analyzeOandaFile(input_path, confidence_threshold);
            analyzer.outputResult(result);
        } else if (fs::is_directory(path)) {
            // Analyze all JSON files in directory
            std::vector<fs::path> json_files;
            
            for (const auto& entry : fs::directory_iterator(path)) {
                if (entry.is_regular_file() && entry.path().extension() == ".json") {
                    json_files.push_back(entry.path());
                }
            }
            
            std::sort(json_files.begin(), json_files.end());
            
            if (json_files.empty()) {
                std::cerr << "No JSON files found in directory: " << input_path << std::endl;
                return 1;
            }
            
            for (const auto& file : json_files) {
                if (!json_output) {
                    std::cout << "\n--- Analyzing: " << file.filename() << " ---" << std::endl;
                }
                
                auto result = analyzer.analyzeOandaFile(file.string(), confidence_threshold);
                analyzer.outputResult(result);
            }
        } else {
            std::cerr << "Error: Input path does not exist or is not a file/directory: " << input_path << std::endl;
            return 1;
        }
        
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
