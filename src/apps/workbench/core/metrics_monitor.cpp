#include "metrics_monitor.h"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>

#include "quantum/pattern_metric_engine.h"

namespace sep::workbench {

    MetricsMonitor::MetricsMonitor() : engine_(nullptr), metrics_mutex_()
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
    pattern_stats_map_.clear();
    system_metrics_ = SystemMetrics{};
    system_metrics_.last_update = std::chrono::steady_clock::now();
    rolling_metrics_ = RollingMetrics{};
    latest_signal_ = ThresholdSignal{};
    metrics_history_.clear();

    if (engine_)
    {
        static_cast<sep::quantum::PatternMetricEngine*>(engine_)->clear();
    }
    
    latest_signal_.entropy_spike = false;
    latest_signal_.cumulative_score = 0.0f;
    latest_signal_.trade_confidence = 0.0f;
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

const MetricsMonitor::RollingMetrics& MetricsMonitor::getRollingMetrics() const {
    return rolling_metrics_;
}

std::unordered_map<std::string, double> MetricsMonitor::getMetrics() const {
    std::lock_guard<std::mutex> lock(metrics_mutex_);
    return metrics_;
}

void MetricsMonitor::set(const std::string& key, double value) {
    std::lock_guard<std::mutex> lock(metrics_mutex_);
    metrics_[key] = value;
    metrics_history_dirty_ = true;
}

const MetricsMonitor::ThresholdSignal& MetricsMonitor::getLatestSignal() const {
    return latest_signal_;
}

sep::connectors::MarketData MetricsMonitor::getLatestMarketData() const {
    std::lock_guard<std::mutex> lock(metrics_mutex_);
    return latest_market_data_;
}

void MetricsMonitor::setLatestMarketData(const sep::connectors::MarketData& data) {
    std::lock_guard<std::mutex> lock(metrics_mutex_);
    latest_market_data_ = data;
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

    auto now = std::chrono::steady_clock::now();

    for (const auto& metric : metrics)
    {
        std::string id(metric.pattern_id);
        auto it = pattern_stats_map_.find(id);
        if (it == pattern_stats_map_.end()) {
            PatternStats stats;
            stats.pattern_id = id;
            stats.coherence = metric.coherence;
            stats.stability = metric.stability;
            stats.entropy = metric.entropy;
            stats.first_seen = now;
            stats.last_seen = now;
            stats.frequency = 1;
            stats.length = strlen(metric.pattern_id);
            stats.persistence = 0.0f;
            pattern_stats_map_[id] = stats;
        } else {
            auto& s = it->second;
            s.coherence = metric.coherence;
            s.stability = metric.stability;
            s.entropy = metric.entropy;
            s.last_seen = now;
            s.frequency += 1;
        }
    }

    pattern_stats_.clear();
    pattern_stats_.reserve(pattern_stats_map_.size());
    for (const auto& kv : pattern_stats_map_) {
        pattern_stats_.push_back(kv.second);
    }

    calculateSystemMetrics();
    calculateRollingMetrics();
    detectThresholdSignals();
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

void MetricsMonitor::calculateRollingMetrics() {
    if (pattern_stats_.empty()) return;

    auto now = std::chrono::steady_clock::now();
    
    // Store current metrics in history
    MetricsSnapshot snapshot;
    snapshot.coherence = system_metrics_.avg_coherence;
    snapshot.stability = system_metrics_.avg_stability;
    snapshot.entropy = system_metrics_.avg_entropy;
    snapshot.timestamp = now;
    metrics_history_.push_back(snapshot);

    // Remove old entries (keep 24 hours + 1 hour for trend calculation)
    auto cutoff_25h = now - std::chrono::hours(25);
    metrics_history_.erase(
        std::remove_if(metrics_history_.begin(), metrics_history_.end(),
            [cutoff_25h](const MetricsSnapshot& s) { return s.timestamp < cutoff_25h; }),
        metrics_history_.end());

    if (metrics_history_.size() < 2) return;

    // Calculate 24h averages
    auto cutoff_24h = now - std::chrono::hours(24);
    auto cutoff_4h = now - std::chrono::hours(4);
    auto cutoff_1h = now - std::chrono::hours(1);
    
    float sum_coherence_24h = 0, sum_stability_24h = 0, sum_entropy_24h = 0;
    float sum_coherence_4h = 0, sum_stability_4h = 0, sum_entropy_4h = 0;
    float sum_coherence_1h = 0, sum_stability_1h = 0, sum_entropy_1h = 0;
    int count_24h = 0, count_4h = 0, count_1h = 0;
    
    // Collect oldest and newest values for trend calculation
    float oldest_coherence = 0, oldest_stability = 0, oldest_entropy = 0;
    float newest_coherence = 0, newest_stability = 0, newest_entropy = 0;
    bool found_oldest = false;

    for (const auto& snap : metrics_history_) {
        if (snap.timestamp >= cutoff_24h) {
            sum_coherence_24h += snap.coherence;
            sum_stability_24h += snap.stability;
            sum_entropy_24h += snap.entropy;
            count_24h++;
            
            if (!found_oldest) {
                oldest_coherence = snap.coherence;
                oldest_stability = snap.stability;
                oldest_entropy = snap.entropy;
                found_oldest = true;
            }
            
            newest_coherence = snap.coherence;
            newest_stability = snap.stability;
            newest_entropy = snap.entropy;
        }

        if (snap.timestamp >= cutoff_4h) {
            sum_coherence_4h += snap.coherence;
            sum_stability_4h += snap.stability;
            sum_entropy_4h += snap.entropy;
            count_4h++;
        }
        
        if (snap.timestamp >= cutoff_1h) {
            sum_coherence_1h += snap.coherence;
            sum_stability_1h += snap.stability;
            sum_entropy_1h += snap.entropy;
            count_1h++;
        }
    }

    // Calculate averages and trends
    if (count_24h > 0) {
        rolling_metrics_.coherence_24h_avg = sum_coherence_24h / count_24h;
        rolling_metrics_.stability_24h_avg = sum_stability_24h / count_24h;
        rolling_metrics_.entropy_24h_avg = sum_entropy_24h / count_24h;
        
        // Trend = (newest - oldest) / oldest * 100 (percentage change)
        if (found_oldest && oldest_coherence != 0) {
            rolling_metrics_.coherence_trend = (newest_coherence - oldest_coherence) / std::abs(oldest_coherence) * 100.0f;
        }
        if (found_oldest && oldest_stability != 0) {
            rolling_metrics_.stability_trend = (newest_stability - oldest_stability) / std::abs(oldest_stability) * 100.0f;
        }
        if (found_oldest && oldest_entropy != 0) {
            rolling_metrics_.entropy_trend = (newest_entropy - oldest_entropy) / std::abs(oldest_entropy) * 100.0f;
        }
    }
    
    if (count_1h > 0) {
        rolling_metrics_.coherence_1h_avg = sum_coherence_1h / count_1h;
        rolling_metrics_.stability_1h_avg = sum_stability_1h / count_1h;
        rolling_metrics_.entropy_1h_avg = sum_entropy_1h / count_1h;
    }

    if (count_4h > 0) {
        rolling_metrics_.coherence_4h_avg = sum_coherence_4h / count_4h;
        rolling_metrics_.stability_4h_avg = sum_stability_4h / count_4h;
        rolling_metrics_.entropy_4h_avg = sum_entropy_4h / count_4h;
    }

    rolling_metrics_.last_calculation = now;
}

MetricsMonitor::SimpleSignals MetricsMonitor::getSimpleThresholdSignals() const {
    // Note: removing lock for const method - consider making metrics_mutex_ mutable if needed
    
    SimpleSignals signals;
    signals.stability = system_metrics_.avg_stability;
    signals.entropy = system_metrics_.avg_entropy;
    signals.coherence = system_metrics_.avg_coherence;
    
    // Dynamic signal generation based on adaptive thresholds
    float stability_sell_threshold = std::max(0.2f, rolling_metrics_.stability_24h_avg * 0.7f);
    float entropy_sell_threshold = std::min(0.8f, rolling_metrics_.entropy_24h_avg * 1.3f);
    float stability_buy_threshold = std::min(0.8f, rolling_metrics_.stability_24h_avg * 1.2f);
    float entropy_buy_threshold = std::max(0.2f, rolling_metrics_.entropy_24h_avg * 0.8f);
    
    signals.sellSignal = (signals.stability < stability_sell_threshold && signals.entropy > entropy_sell_threshold);
    signals.buySignal = (signals.stability > stability_buy_threshold && signals.entropy < entropy_buy_threshold);
    
    return signals;
}

sep::quantum::SignalThresholds MetricsMonitor::calculateSignalThresholds() const {
    sep::quantum::SignalThresholds th;
    th.buy_min_coherence = std::clamp(rolling_metrics_.coherence_24h_avg * 1.2f, 0.0f, 1.0f);
    th.buy_min_stability = std::clamp(rolling_metrics_.stability_24h_avg * 1.2f, 0.0f, 1.0f);
    th.buy_max_entropy = std::clamp(rolling_metrics_.entropy_24h_avg * 0.8f, 0.0f, 1.0f);
    th.sell_max_stability = std::clamp(rolling_metrics_.stability_24h_avg * 0.8f, 0.0f, 1.0f);
    th.sell_min_entropy = std::clamp(rolling_metrics_.entropy_24h_avg * 1.2f, 0.0f, 1.0f);
    return th;
}

void MetricsMonitor::detectThresholdSignals() {
    if (metrics_history_.size() < 10) return;  // Need some history for confidence

    auto now = std::chrono::steady_clock::now();
    ThresholdSignal signal;
    signal.timestamp = now;
    signal.signal_type = ThresholdSignal::HOLD;
    signal.confidence = 0.0f;

    // Current metrics
    float current_stability = system_metrics_.avg_stability;
    float current_entropy = system_metrics_.avg_entropy;
    float current_coherence = system_metrics_.avg_coherence;
    
    // Rolling averages for context
    float stability_24h = rolling_metrics_.stability_24h_avg;
    float entropy_24h = rolling_metrics_.entropy_24h_avg;
    float coherence_24h = rolling_metrics_.coherence_24h_avg;

    std::string reasons;
    float confidence_score = 0.0f;

    // Dynamic threshold calculations based on historical context
    float stability_threshold = std::max(0.2f, stability_24h * 0.7f);
    float entropy_threshold = std::min(0.8f, entropy_24h * 1.3f);
    float coherence_threshold = coherence_24h * 0.8f;

    // Signal condition 1: Low stability indicates market uncertainty (potential sell)
    if (current_stability < stability_threshold) {
        signal.low_stability = true;
        confidence_score += 25.0f;
        reasons += "Low market stability (" + std::to_string(current_stability) + " < " + std::to_string(stability_threshold) + "); ";
    }

    // Signal condition 2: High entropy indicates chaos/volatility (potential sell)
    if (current_entropy > entropy_threshold) {
        signal.high_entropy = true;
        confidence_score += 25.0f;
        reasons += "High market entropy (" + std::to_string(current_entropy) + " > " + std::to_string(entropy_threshold) + "); ";
    }

    // Signal condition 3: Coherence drop indicates pattern breakdown (sell)
    if (current_coherence < coherence_threshold && rolling_metrics_.coherence_trend < -10.0f) {
        signal.coherence_drop = true;
        confidence_score += 30.0f;
        reasons += "Coherence drop (" + std::to_string(current_coherence) + " < " + std::to_string(coherence_threshold) + ") with negative trend; ";
    }

    // Signal condition 4: Rapid change detection (buy opportunity after sell-off)
    if (std::abs(rolling_metrics_.stability_trend) > 20.0f || 
        std::abs(rolling_metrics_.entropy_trend) > 25.0f) {
        signal.rapid_change = true;
        confidence_score += 15.0f;
        reasons += "Rapid market state change; ";
    }

    // Signal condition 5: Entropy spike (rapid increase)
    if (rolling_metrics_.entropy_trend > 35.0f && current_entropy > entropy_24h * 1.5f) {
        signal.entropy_spike = true;
        confidence_score += 20.0f;
        reasons += "Entropy spike detected; ";
    }

    // Determine signal type based on conditions
    if (signal.entropy_spike && signal.coherence_drop && current_stability < 0.4f) {
        signal.signal_type = ThresholdSignal::SELL;
        confidence_score += 40.0f; // High-confidence sell signal
        reasons += "Reversal pattern: entropy spike + coherence drop; ";
    } else if (signal.low_stability && signal.high_entropy) {
        signal.signal_type = ThresholdSignal::SELL;
        confidence_score += 10.0f;  // Bonus for multiple sell indicators
    } else if (signal.coherence_drop) {
        signal.signal_type = ThresholdSignal::SELL;
    } else if (signal.rapid_change && current_stability > 0.6f && current_entropy < 0.4f) {
        // Stable conditions after rapid change could indicate buy opportunity
        signal.signal_type = ThresholdSignal::BUY;
        reasons += "Stabilization after volatility; ";
    }

    // Dynamic Cumulative Signal Scoring based on relative performance
    float coherence_weight = 0.4f + (current_coherence > coherence_24h ? 0.2f : -0.1f);
    float stability_weight = 0.3f + (current_stability > stability_24h ? 0.1f : -0.1f);
    float entropy_weight = 0.3f + (current_entropy < entropy_24h ? 0.1f : -0.2f);
    
    signal.cumulative_score = (current_coherence * coherence_weight) + 
                             (current_stability * stability_weight) - 
                             (current_entropy * entropy_weight);

    // Dynamic Trade Confidence Index based on signal strength and historical context
    float stability_confidence = signal.low_stability ? (stability_24h - current_stability) / stability_24h * 0.25f : 0.0f;
    float entropy_confidence = signal.high_entropy ? (current_entropy - entropy_24h) / entropy_24h * 0.25f : 0.0f;
    float coherence_confidence = signal.coherence_drop ? (coherence_24h - current_coherence) / coherence_24h * 0.3f : 0.0f;
    float spike_confidence = signal.entropy_spike ? 0.2f : 0.0f;
    
    float confidence_from_metrics = stability_confidence + entropy_confidence + coherence_confidence + spike_confidence;
    
    signal.trade_confidence = std::min(1.0f, confidence_from_metrics + (signal.cumulative_score * 0.1f));


    signal.confidence = std::min(100.0f, confidence_score);
    signal.reason = reasons;

    // Only update if confidence is meaningful (>15%)
    if (signal.confidence > 15.0f) {
        latest_signal_ = signal;
    }
}

} // namespace sep::workbench
