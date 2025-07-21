#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <thread>
#include <chrono>

#include "quantum/pattern_metric_engine.h"

using namespace sep::quantum;

class SimpleDataSandbox {
private:
    PatternMetricEngine engine;
    
    // File processing
    std::string selected_file_path;
    std::vector<uint8_t> file_data;
    bool file_loaded = false;
    
    // Metrics data
    std::vector<PatternMetrics> current_metrics;
    
    // Memory monitoring
    struct MemoryStats {
        size_t current_usage = 0;
        size_t peak_usage = 0;
        size_t data_size = 0;
    } memory_stats;
    
    // Processing options
    struct ProcessingOptions {
        int min_pattern_length = 4;
        int max_patterns = 100;
        bool auto_evolve = true;
    } options;

public:
    SimpleDataSandbox() = default;
    ~SimpleDataSandbox() = default;
    
    bool initialize() {
        std::cout << "=== SEP Data Processing Sandbox ===" << std::endl;
        std::cout << "Initializing pattern metric engine..." << std::endl;
        
        if (engine.init(nullptr) != sep::SEPResult::SUCCESS) {
            std::cerr << "Failed to initialize pattern metric engine" << std::endl;
            return false;
        }
        
        std::cout << "✅ Engine initialized successfully" << std::endl;
        return true;
    }
    
    void run() {
        showMainMenu();
        
        std::string command;
        while (true) {
            std::cout << "\n> ";
            std::getline(std::cin, command);
            
            if (command == "quit" || command == "q") {
                break;
            } else if (command == "help" || command == "h") {
                showMainMenu();
            } else if (command == "load" || command == "l") {
                loadFileInteractive();
            } else if (command == "sample" || command == "s") {
                generateSampleData();
            } else if (command == "process" || command == "p") {
                processCurrentData();
            } else if (command == "metrics" || command == "m") {
                showMetrics();
            } else if (command == "memory" || command == "mem") {
                showMemoryStats();
            } else if (command == "export" || command == "e") {
                exportMetricsJSON();
            } else if (command == "info" || command == "i") {
                showDataInfo();
            } else if (command == "monitor") {
                runContinuousMonitoring();
            } else {
                std::cout << "Unknown command. Type 'help' for available commands." << std::endl;
            }
        }
        
        std::cout << "Goodbye!" << std::endl;
    }
    
private:
    void showMainMenu() {
        std::cout << "\n=== Available Commands ===" << std::endl;
        std::cout << "load (l)    - Load a file for processing" << std::endl;
        std::cout << "sample (s)  - Generate sample data" << std::endl;
        std::cout << "process (p) - Process current data and extract patterns" << std::endl;
        std::cout << "metrics (m) - Show current pattern metrics" << std::endl;
        std::cout << "memory      - Show memory usage statistics" << std::endl;
        std::cout << "info (i)    - Show information about loaded data" << std::endl;
        std::cout << "export (e)  - Export metrics to JSON file" << std::endl;
        std::cout << "monitor     - Run continuous monitoring mode" << std::endl;
        std::cout << "help (h)    - Show this menu" << std::endl;
        std::cout << "quit (q)    - Exit the program" << std::endl;
    }
    
    void loadFileInteractive() {
        std::cout << "Enter file path: ";
        std::string path;
        std::getline(std::cin, path);
        
        if (loadFile(path)) {
            selected_file_path = path;
            std::cout << "✅ File loaded successfully: " << file_data.size() << " bytes" << std::endl;
        } else {
            std::cout << "❌ Failed to load file" << std::endl;
        }
    }
    
    bool loadFile(const std::string& path) {
        try {
            std::ifstream file(path, std::ios::binary | std::ios::ate);
            if (!file.is_open()) {
                std::cout << "Failed to open file: " << path << std::endl;
                return false;
            }
            
            auto size = file.tellg();
            file.seekg(0, std::ios::beg);
            
            file_data.resize(size);
            file.read(reinterpret_cast<char*>(file_data.data()), size);
            
            file_loaded = true;
            updateMemoryStats();
            return true;
        } catch (const std::exception& e) {
            std::cout << "Error loading file: " << e.what() << std::endl;
            return false;
        }
    }
    
    void generateSampleData() {
        std::cout << "Generating sample data..." << std::endl;
        
        file_data.clear();
        file_data.resize(1024 * 8); // 8KB sample
        
        std::random_device rd;
        std::mt19937 gen(rd());
        std::normal_distribution<> d(128, 32);
        
        for (auto& byte : file_data) {
            byte = static_cast<uint8_t>(std::clamp(static_cast<int>(d(gen)), 0, 255));
        }
        
        file_loaded = true;
        selected_file_path = "Generated Sample Data";
        updateMemoryStats();
        std::cout << "✅ Generated 8KB sample data" << std::endl;
    }
    
    void processCurrentData() {
        if (!file_loaded) {
            std::cout << "❌ No data loaded. Use 'load' or 'sample' first." << std::endl;
            return;
        }
        
        std::cout << "Processing data..." << std::endl;
        
        // Ingest data into engine
        engine.ingestData(file_data.data(), file_data.size());
        
        // Evolve patterns
        engine.evolvePatterns();
        
        // Compute metrics
        current_metrics = engine.computeMetrics();
        
        std::cout << "✅ Processing complete. Found " << current_metrics.size() << " patterns." << std::endl;
        updateMemoryStats();
    }
    
    void showMetrics() {
        if (current_metrics.empty()) {
            std::cout << "No metrics available. Process data first." << std::endl;
            return;
        }
        
        std::cout << "\n=== Pattern Metrics ===" << std::endl;
        std::cout << "Total patterns: " << current_metrics.size() << std::endl;
        std::cout << std::setw(8) << "Pattern" << std::setw(12) << "Coherence" 
                  << std::setw(12) << "Stability" << std::setw(12) << "Entropy" << std::endl;
        std::cout << std::string(44, '-') << std::endl;
        
        for (size_t i = 0; i < std::min(current_metrics.size(), size_t(20)); ++i) {
            const auto& m = current_metrics[i];
            std::cout << std::setw(8) << i 
                     << std::setw(12) << std::fixed << std::setprecision(3) << m.coherence
                     << std::setw(12) << std::fixed << std::setprecision(3) << m.stability
                     << std::setw(12) << std::fixed << std::setprecision(3) << m.entropy << std::endl;
        }
        
        if (current_metrics.size() > 20) {
            std::cout << "... and " << (current_metrics.size() - 20) << " more patterns" << std::endl;
        }
        
        // Show averages
        float avg_coherence = 0, avg_stability = 0, avg_entropy = 0;
        for (const auto& m : current_metrics) {
            avg_coherence += m.coherence;
            avg_stability += m.stability;
            avg_entropy += m.entropy;
        }
        
        avg_coherence /= current_metrics.size();
        avg_stability /= current_metrics.size();
        avg_entropy /= current_metrics.size();
        
        std::cout << std::string(44, '-') << std::endl;
        std::cout << std::setw(8) << "AVG" 
                 << std::setw(12) << std::fixed << std::setprecision(3) << avg_coherence
                 << std::setw(12) << std::fixed << std::setprecision(3) << avg_stability
                 << std::setw(12) << std::fixed << std::setprecision(3) << avg_entropy << std::endl;
    }
    
    void showMemoryStats() {
        std::cout << "\n=== Memory Statistics ===" << std::endl;
        std::cout << "Data Size:      " << std::setw(8) << (memory_stats.data_size / 1024.0f) << " KB" << std::endl;
        std::cout << "Current Usage:  " << std::setw(8) << (memory_stats.current_usage / 1024.0f) << " KB" << std::endl;
        std::cout << "Peak Usage:     " << std::setw(8) << (memory_stats.peak_usage / 1024.0f) << " KB" << std::endl;
        std::cout << "Pattern Count:  " << std::setw(8) << current_metrics.size() << std::endl;
        
        if (memory_stats.data_size > 0) {
            float ratio = static_cast<float>(memory_stats.current_usage) / memory_stats.data_size;
            std::cout << "Memory Ratio:   " << std::setw(8) << std::fixed << std::setprecision(2) << ratio << "x" << std::endl;
        }
    }
    
    void showDataInfo() {
        if (!file_loaded) {
            std::cout << "No data loaded." << std::endl;
            return;
        }
        
        std::cout << "\n=== Data Information ===" << std::endl;
        std::cout << "Source: " << selected_file_path << std::endl;
        std::cout << "Size: " << file_data.size() << " bytes (" << (file_data.size() / 1024.0f) << " KB)" << std::endl;
        std::cout << "Type: " << detectDataType() << std::endl;
        
        // Show first 32 bytes as hex
        std::cout << "First 32 bytes (hex): ";
        for (size_t i = 0; i < std::min(file_data.size(), size_t(32)); ++i) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(file_data[i]) << " ";
        }
        std::cout << std::dec << std::setfill(' ') << std::endl;
    }
    
    void runContinuousMonitoring() {
        if (!file_loaded) {
            std::cout << "❌ No data loaded. Use 'load' or 'sample' first." << std::endl;
            return;
        }
        
        std::cout << "Starting continuous monitoring. Press Ctrl+C to stop." << std::endl;
        std::cout << "Processing data every 2 seconds..." << std::endl;
        
        int iteration = 0;
        while (iteration < 10) { // Limit to 10 iterations for demo
            std::cout << "\n--- Iteration " << (++iteration) << " ---" << std::endl;
            
            // Process data
            engine.ingestData(file_data.data(), file_data.size());
            engine.evolvePatterns();
            current_metrics = engine.computeMetrics();
            updateMemoryStats();
            
            // Show summary
            if (!current_metrics.empty()) {
                float avg_coherence = 0;
                for (const auto& m : current_metrics) {
                    avg_coherence += m.coherence;
                }
                avg_coherence /= current_metrics.size();
                
                std::cout << "Patterns: " << current_metrics.size() 
                         << ", Avg Coherence: " << std::fixed << std::setprecision(3) << avg_coherence
                         << ", Memory: " << (memory_stats.current_usage / 1024.0f) << " KB" << std::endl;
            }
            
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
        
        std::cout << "Monitoring complete." << std::endl;
    }
    
    std::string detectDataType() {
        if (!file_loaded || file_data.empty()) return "Unknown";
        
        // Simple data type detection
        bool has_text = true;
        int printable_count = 0;
        
        for (size_t i = 0; i < std::min(file_data.size(), size_t(100)); ++i) {
            uint8_t byte = file_data[i];
            if (byte >= 32 && byte <= 126) {
                printable_count++;
            } else if (byte != 9 && byte != 10 && byte != 13) {
                has_text = false;
            }
        }
        
        if (has_text && printable_count > 50) {
            return "Text/ASCII";
        } else {
            return "Binary";
        }
    }
    
    void updateMemoryStats() {
        memory_stats.data_size = file_data.size();
        memory_stats.current_usage = memory_stats.data_size + (current_metrics.size() * sizeof(PatternMetrics));
        
        if (memory_stats.current_usage > memory_stats.peak_usage) {
            memory_stats.peak_usage = memory_stats.current_usage;
        }
    }
    
    void exportMetricsJSON() {
        if (current_metrics.empty()) {
            std::cout << "No metrics to export." << std::endl;
            return;
        }
        
        std::string filename = "metrics_export_" + std::to_string(std::time(nullptr)) + ".json";
        std::ofstream file(filename);
        
        file << "{\n";
        file << "  \"timestamp\": " << std::time(nullptr) << ",\n";
        file << "  \"file_path\": \"" << selected_file_path << "\",\n";
        file << "  \"data_size\": " << file_data.size() << ",\n";
        file << "  \"pattern_count\": " << current_metrics.size() << ",\n";
        file << "  \"memory_usage\": " << memory_stats.current_usage << ",\n";
        file << "  \"patterns\": [\n";
        
        for (size_t i = 0; i < current_metrics.size(); ++i) {
            const auto& m = current_metrics[i];
            file << "    {\n";
            file << "      \"id\": " << i << ",\n";
            file << "      \"coherence\": " << m.coherence << ",\n";
            file << "      \"stability\": " << m.stability << ",\n";
            file << "      \"entropy\": " << m.entropy << "\n";
            file << "    }" << (i < current_metrics.size() - 1 ? "," : "") << "\n";
        }
        
        file << "  ]\n";
        file << "}\n";
        
        std::cout << "✅ Metrics exported to " << filename << std::endl;
    }
};

int main() {
    SimpleDataSandbox sandbox;
    
    if (!sandbox.initialize()) {
        std::cerr << "Failed to initialize sandbox" << std::endl;
        return 1;
    }
    
    sandbox.run();
    
    return 0;
}
