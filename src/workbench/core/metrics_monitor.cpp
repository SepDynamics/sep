#include "metrics_monitor.h"
#include <algorithm>
#include <sstream>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace sep::workbench {

MetricsMonitor::MetricsMonitor() 
    : engine_(std::make_unique<sep::quantum::PatternMetricEngine>()) {
    start_time_ = std::chrono::steady_clock::now();
    last_metrics_update_ = start_time_;
}

MetricsMonitor::~MetricsMonitor() {
    shutdown();
}

bool MetricsMonitor::initialize() {
    auto result = engine_->init(nullptr);  // CPU-only for now
    return result == sep::SEPResult::SUCCESS;
}

void MetricsMonitor::shutdown() {
    stopProcessing();
    shutdown_requested_ = true;
}

void MetricsMonitor::clear() {
    std::lock_guard<std::mutex> lock(metrics_mutex_);
    
    engine_->clear();
    pattern_stats_.clear();
    system_metrics_ = SystemMetrics{};
    
    start_time_ = std::chrono::steady_clock::now();
    last_metrics_update_ = start_time_;
    
    std::cout << "[MetricsMonitor] Data cleared - ready for fresh analysis" << std::endl;
}

void MetricsMonitor::ingestData(const uint8_t* data, size_t size) {
    engine_->ingestData(data, size);
    processIngestedData();
}

void MetricsMonitor::ingestFile(const std::string& filepath) {
    std::cout << "[MetricsMonitor] Ingesting file: " << filepath << std::endl;
    engine_->ingestFile(filepath);
    processIngestedData();
}

void MetricsMonitor::ingestStream(std::istream& stream) {
    engine_->ingestData(stream);
    processIngestedData();
}

void MetricsMonitor::startProcessing() {
    processing_ = true;
    std::cout << "[MetricsMonitor] Started real-time processing" << std::endl;
}

void MetricsMonitor::stopProcessing() {
    processing_ = false;
    std::cout << "[MetricsMonitor] Stopped real-time processing" << std::endl;
}

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
    std::lock_guard<std::mutex> lock(metrics_mutex_);
    
    json result;
    result["timestamp"] = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    
    // System metrics
    result["system"] = {
        {"avg_coherence", system_metrics_.avg_coherence},
        {"avg_stability", system_metrics_.avg_stability},
        {"avg_entropy", system_metrics_.avg_entropy},
        {"total_patterns", system_metrics_.total_patterns},
        {"unique_patterns", system_metrics_.unique_patterns},
        {"active_patterns", system_metrics_.active_patterns},
        {"pattern_emergence_rate", system_metrics_.pattern_emergence_rate}
    };
    
    // Individual patterns
    json patterns = json::array();
    for (const auto& stats : pattern_stats_) {
        json pattern = {
            {"id", stats.pattern_id},
            {"coherence", stats.coherence},
            {"stability", stats.stability},
            {"entropy", stats.entropy},
            {"length", stats.length},
            {"frequency", stats.frequency},
            {"persistence", stats.persistence}
        };
        patterns.push_back(pattern);
    }
    result["patterns"] = patterns;
    
    return result.dump(2);
}

void MetricsMonitor::saveMetricsToFile(const std::string& filepath) const {
    std::ofstream file(filepath);
    if (file.is_open()) {
        file << exportMetricsAsJSON();
        std::cout << "[MetricsMonitor] Metrics saved to " << filepath << std::endl;
    } else {
        std::cerr << "[MetricsMonitor] Failed to save metrics to " << filepath << std::endl;
    }
}

void MetricsMonitor::processIngestedData() {
    // Process patterns in the engine
    engine_->evolvePatterns();
    
    // Update our metrics
    updateMetrics();
}

void MetricsMonitor::updateMetrics() {
    std::lock_guard<std::mutex> lock(metrics_mutex_);
    
    auto now = std::chrono::steady_clock::now();
    auto active_threshold = now - std::chrono::duration<float>(active_window_seconds_);
    
    // Get metrics from engine
    const auto& engine_metrics = engine_->computeMetrics();
    const auto& patterns = engine_->getPatterns();
    
    // Update pattern stats
    pattern_stats_.clear();
    pattern_stats_.reserve(std::min(engine_metrics.size(), max_patterns_));
    
    for (size_t i = 0; i < std::min(engine_metrics.size(), max_patterns_); ++i) {
        const auto& metric = engine_metrics[i];
        
        PatternStats stats;
        stats.pattern_id = metric.pattern_id;
        stats.coherence = metric.coherence;
        stats.stability = metric.stability;
        stats.entropy = metric.entropy;
        stats.length = patterns[i].size;
        stats.frequency = 1;  // TODO: Track frequency over time
        stats.first_seen = now;  // TODO: Track actual first seen time
        stats.last_seen = now;
        
        // Calculate persistence (simplified for now)
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
            stats.last_seen - stats.first_seen);
        stats.persistence = duration.count() / 1000.0f;
        
        pattern_stats_.push_back(stats);
    }
    
    // Calculate system metrics
    calculateSystemMetrics();
    
    last_metrics_update_ = now;
}

void MetricsMonitor::calculateSystemMetrics() {
    if (pattern_stats_.empty()) {
        system_metrics_ = SystemMetrics{};
        return;
    }
    
    auto now = std::chrono::steady_clock::now();
    auto active_threshold = now - std::chrono::duration<float>(active_window_seconds_);
    
    float total_coherence = 0.0f;
    float total_stability = 0.0f;
    float total_entropy = 0.0f;
    size_t active_count = 0;
    
    for (const auto& stats : pattern_stats_) {
        total_coherence += stats.coherence;
        total_stability += stats.stability;
        total_entropy += stats.entropy;
        
        if (stats.last_seen > active_threshold) {
            active_count++;
        }
    }
    
    size_t total = pattern_stats_.size();
    system_metrics_.avg_coherence = total_coherence / total;
    system_metrics_.avg_stability = total_stability / total;
    system_metrics_.avg_entropy = total_entropy / total;
    system_metrics_.total_patterns = total;
    system_metrics_.unique_patterns = total;  // All patterns are unique by definition
    system_metrics_.active_patterns = active_count;
    
    // Calculate emergence rate (patterns per second)
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start_time_);
    if (elapsed.count() > 0) {
        system_metrics_.pattern_emergence_rate = float(total) / elapsed.count();
    }
    
    system_metrics_.last_update = now;
}

} // namespace sep::workbench
