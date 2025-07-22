#include "metrics_monitor.h"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>

#include "quantum/pattern_metric_engine.h"

namespace sep::workbench {

    MetricsMonitor::MetricsMonitor() : engine_(nullptr)
    {
        start_time_ = std::chrono::steady_clock::now();
        last_metrics_update_ = start_time_;
    }

MetricsMonitor::~MetricsMonitor() {
    shutdown();
    if (engine_)
    {
        delete static_cast<sep::quantum::PatternMetricEngine*>(engine_);
    }
}

bool MetricsMonitor::initialize() {
    try
    {
        // Create the actual quantum pattern metric engine
        auto* engine = new sep::quantum::PatternMetricEngine();

        // Initialize for CPU operation (nullptr for GPU context)
        sep::SEPResult result = engine->init(nullptr);

        if (result != sep::SEPResult::SUCCESS)
        {
            std::cerr << "[MetricsMonitor] Failed to initialize PatternMetricEngine" << std::endl;
            delete engine;
            return false;
        }

        engine_ = engine;
        std::cout << "[MetricsMonitor] Initialized with quantum engine" << std::endl;
        return true;
    }
    catch (const std::exception& e)
    {
        std::cerr << "[MetricsMonitor] Initialization failed: " << e.what() << std::endl;
        return false;
    }
}

void MetricsMonitor::shutdown() {
    stopProcessing();
    clear();
}

void MetricsMonitor::clear() {
    std::lock_guard<std::mutex> lock(metrics_mutex_);
    pattern_stats_.clear();
    system_metrics_ = SystemMetrics{};
    system_metrics_.last_update = std::chrono::steady_clock::now();

    if (engine_)
    {
        static_cast<sep::quantum::PatternMetricEngine*>(engine_)->clear();
    }
}

void MetricsMonitor::ingestData(const uint8_t* data, size_t size) {
    if (!data || size == 0 || !engine_) return;

    // Use the real quantum engine to ingest data
    auto* engine = static_cast<sep::quantum::PatternMetricEngine*>(engine_);
    engine->ingestData(data, size);

    // Process if we're in processing mode
    if (processing_)
    {
        processIngestedData();
    }
}

void MetricsMonitor::ingestFile(const std::string& filepath) {
    if (!engine_)
    {
        std::cerr << "[MetricsMonitor] Engine not initialized" << std::endl;
        return;
    }

    // Use the engine's file ingestion capability
    auto* engine = static_cast<sep::quantum::PatternMetricEngine*>(engine_);
    engine->ingestFile(filepath);

    if (processing_)
    {
        processIngestedData();
    }
}

void MetricsMonitor::ingestStream(std::istream& stream) {
    if (!engine_)
    {
        std::cerr << "[MetricsMonitor] Engine not initialized" << std::endl;
        return;
    }

    // Use the engine's stream ingestion capability
    auto* engine = static_cast<sep::quantum::PatternMetricEngine*>(engine_);
    engine->ingestData(stream);

    if (processing_)
    {
        processIngestedData();
    }
}

void MetricsMonitor::startProcessing() { processing_ = true; }

void MetricsMonitor::stopProcessing() { processing_ = false; }

bool MetricsMonitor::isProcessing() const {
    return processing_;
}

const std::vector<MetricsMonitor::PatternStats>& MetricsMonitor::getPatternStats() const {
    return pattern_stats_;
}

const MetricsMonitor::SystemMetrics& MetricsMonitor::getSystemMetrics() const {
    return system_metrics_;
}

void MetricsMonitor::setMinPatternLength(size_t min_length) {
    min_pattern_length_ = min_length;
}

void MetricsMonitor::setMaxPatterns(size_t max_patterns) {
    max_patterns_ = max_patterns;
}

void MetricsMonitor::setActiveWindowSeconds(float seconds) {
    active_window_seconds_ = seconds;
}

std::string MetricsMonitor::exportMetricsAsJSON() const {
    // Note: Can't lock a mutex in a const method, so we'll work with current state

    std::stringstream json;
    json << "{\n";
    json << "  \"system_metrics\": {\n";
    json << "    \"avg_coherence\": " << system_metrics_.avg_coherence << ",\n";
    json << "    \"avg_stability\": " << system_metrics_.avg_stability << ",\n";
    json << "    \"avg_entropy\": " << system_metrics_.avg_entropy << ",\n";
    json << "    \"total_patterns\": " << system_metrics_.total_patterns << ",\n";
    json << "    \"unique_patterns\": " << system_metrics_.unique_patterns << ",\n";
    json << "    \"active_patterns\": " << system_metrics_.active_patterns << ",\n";
    json << "    \"pattern_emergence_rate\": " << system_metrics_.pattern_emergence_rate << "\n";
    json << "  },\n";
    json << "  \"patterns\": [\n";

    for (size_t i = 0; i < pattern_stats_.size(); ++i)
    {
        const auto& p = pattern_stats_[i];
        json << "    {\n";
        json << "      \"id\": \"" << p.pattern_id << "\",\n";
        json << "      \"coherence\": " << p.coherence << ",\n";
        json << "      \"stability\": " << p.stability << ",\n";
        json << "      \"entropy\": " << p.entropy << ",\n";
        json << "      \"length\": " << p.length << ",\n";
        json << "      \"frequency\": " << p.frequency << ",\n";
        json << "      \"persistence\": " << p.persistence << "\n";
        json << "    }";
        if (i < pattern_stats_.size() - 1) json << ",";
        json << "\n";
    }

    json << "  ]\n";
    json << "}\n";

    return json.str();
}

void MetricsMonitor::saveMetricsToFile(const std::string& filepath) const {
    std::ofstream file(filepath);
    if (file)
    {
        file << exportMetricsAsJSON();
    }
}

void MetricsMonitor::updateMetrics() {
    if (!engine_) return;

    auto* engine = static_cast<sep::quantum::PatternMetricEngine*>(engine_);

    // Get metrics from the quantum engine
    auto metrics = engine->computeMetrics();

    std::lock_guard<std::mutex> lock(metrics_mutex_);
    pattern_stats_.clear();

    auto now = std::chrono::steady_clock::now();

    // Convert quantum engine metrics to our format
    for (const auto& metric : metrics)
    {
        PatternStats stats;
        stats.pattern_id = std::string(metric.pattern_id);
        stats.coherence = metric.coherence;
        stats.stability = metric.stability;
        stats.entropy = metric.entropy;
        stats.first_seen = now;  // We don't track this in the engine yet
        stats.last_seen = now;
        stats.frequency = 1;                       // TODO: Track frequency in engine
        stats.length = strlen(metric.pattern_id);  // Approximate
        stats.persistence = 0.0f;

        pattern_stats_.push_back(stats);
    }

    calculateSystemMetrics();
}

void MetricsMonitor::processIngestedData()
{
    if (!engine_) return;

    auto* engine = static_cast<sep::quantum::PatternMetricEngine*>(engine_);

    // Evolve patterns using quantum processing
    engine->evolvePatterns();

    // Update our metrics
    updateMetrics();
}

void MetricsMonitor::calculateSystemMetrics() {
    if (pattern_stats_.empty()) return;

    float total_coherence = 0.0f;
    float total_stability = 0.0f;
    float total_entropy = 0.0f;
    size_t active_count = 0;

    auto now = std::chrono::steady_clock::now();
    auto active_threshold = now - std::chrono::seconds(static_cast<int>(active_window_seconds_));

    for (const auto& pattern : pattern_stats_)
    {
        total_coherence += pattern.coherence;
        total_stability += pattern.stability;
        total_entropy += pattern.entropy;

        if (pattern.last_seen >= active_threshold)
        {
            active_count++;
        }
    }

    system_metrics_.avg_coherence = total_coherence / pattern_stats_.size();
    system_metrics_.avg_stability = total_stability / pattern_stats_.size();
    system_metrics_.avg_entropy = total_entropy / pattern_stats_.size();
    system_metrics_.total_patterns = pattern_stats_.size();
    system_metrics_.unique_patterns = pattern_stats_.size();  // All are unique from engine
    system_metrics_.active_patterns = active_count;

    // Calculate emergence rate
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - start_time_).count();
    if (duration > 0)
    {
        system_metrics_.pattern_emergence_rate =
            static_cast<float>(pattern_stats_.size()) / duration;
    }

    system_metrics_.last_update = now;
}

} // namespace sep::workbench
