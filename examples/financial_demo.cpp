#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "quantum/pattern_metric_engine.h"

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

// Generate mock financial data
std::vector<float> generateFinancialData(int num_points) {
    std::vector<float> data;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> d(100, 5); // Mean 100, stddev 5

    for (int i = 0; i < num_points; ++i) {
        data.push_back(static_cast<float>(d(gen)));
    }
    return data;
}

int main() {
    // Create and initialize the engine
    PatternMetricEngine engine;
    if (engine.init(nullptr) != sep::SEPResult::SUCCESS) {
        std::cerr << "Failed to initialize pattern metric engine\n";
        return 1;
    }

    // Generate and process mock financial data
    std::cout << "\n=== Processing Mock Financial Data ===\n";
    auto financial_data = generateFinancialData(100);
    engine.ingestData(reinterpret_cast<const uint8_t*>(financial_data.data()),
                     financial_data.size() * sizeof(float));
    engine.evolvePatterns();
    printMetrics("Mock Financial Data", engine.computeMetrics());

    return 0;
}