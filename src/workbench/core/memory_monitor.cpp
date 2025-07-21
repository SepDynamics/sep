#include "memory_monitor.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#ifdef __linux__
#include <unistd.h>
#include <sys/resource.h>
#elif _WIN32
#include <windows.h>
#include <psapi.h>
#elif __APPLE__
#include <mach/mach.h>
#include <mach/mach_host.h>
#endif

namespace sep::workbench {

MemoryMonitor::MemoryMonitor() {
    start_time_ = std::chrono::steady_clock::now();
    last_snapshot_time_ = start_time_;
}

MemoryMonitor::~MemoryMonitor() {
    shutdown();
}

bool MemoryMonitor::initialize() {
    // Capture initial snapshot
    captureSnapshot();
    return true;
}

void MemoryMonitor::shutdown() {
    shutdown_requested_ = true;
    stopMonitoring();
}

void MemoryMonitor::reset() {
    std::lock_guard<std::mutex> lock(data_mutex_);
    history_.clear();
    current_stats_ = MemoryStats{};
    start_time_ = std::chrono::steady_clock::now();
    last_snapshot_time_ = start_time_;
    captureSnapshot();
}

void MemoryMonitor::startMonitoring(std::chrono::milliseconds interval) {
    if (monitoring_.load()) return;
    
    monitoring_ = true;
    monitoring_interval_ = interval;
    shutdown_requested_ = false;
    
    monitor_thread_ = std::make_unique<std::thread>(&MemoryMonitor::monitoringThread, this);
    std::cout << "[MemoryMonitor] Started monitoring with " << interval.count() << "ms interval" << std::endl;
}

void MemoryMonitor::stopMonitoring() {
    if (!monitoring_.load()) return;
    
    monitoring_ = false;
    if (monitor_thread_ && monitor_thread_->joinable()) {
        monitor_thread_->join();
    }
    monitor_thread_.reset();
    std::cout << "[MemoryMonitor] Stopped monitoring" << std::endl;
}

MemoryMonitor::MemoryStats MemoryMonitor::getCurrentStats() const {
    std::lock_guard<std::mutex> lock(data_mutex_);
    return current_stats_;
}

std::vector<MemoryMonitor::MemorySnapshot> MemoryMonitor::getHistory(size_t max_samples) const {
    std::lock_guard<std::mutex> lock(data_mutex_);
    
    if (history_.size() <= max_samples) {
        return history_;
    }
    
    // Return evenly spaced samples if we have too many
    std::vector<MemorySnapshot> result;
    result.reserve(max_samples);
    
    size_t step = history_.size() / max_samples;
    for (size_t i = 0; i < history_.size(); i += step) {
        result.push_back(history_[i]);
    }
    
    // Always include the latest
    if (!history_.empty() && result.back().timestamp != history_.back().timestamp) {
        result.push_back(history_.back());
    }
    
    return result;
}

MemoryMonitor::MemorySnapshot MemoryMonitor::getLatestSnapshot() const {
    std::lock_guard<std::mutex> lock(data_mutex_);
    if (history_.empty()) {
        return MemorySnapshot{};
    }
    return history_.back();
}

void MemoryMonitor::updateTierUsage(size_t tier1, size_t tier2, size_t tier3) {
    tier1_bytes_ = tier1;
    tier2_bytes_ = tier2;
    tier3_bytes_ = tier3;
}

bool MemoryMonitor::isUnderMemoryPressure() const {
    return getMemoryPressureLevel() > 0.7f;
}

float MemoryMonitor::getMemoryPressureLevel() const {
    size_t current_rss = getCurrentRSS();
    
#ifdef __linux__
    // Check available system memory
    std::ifstream meminfo("/proc/meminfo");
    std::string line;
    size_t mem_total = 0, mem_available = 0;
    
    while (std::getline(meminfo, line)) {
        std::istringstream iss(line);
        std::string key;
        size_t value;
        std::string unit;
        
        if (iss >> key >> value >> unit) {
            if (key == "MemTotal:") mem_total = value * 1024;
            else if (key == "MemAvailable:") mem_available = value * 1024;
        }
    }
    
    if (mem_total > 0) {
        float system_pressure = 1.0f - (float(mem_available) / float(mem_total));
        float process_pressure = float(current_rss) / float(mem_total);
        return std::max(system_pressure, process_pressure);
    }
#endif
    
    // Fallback: assume pressure if using > 4GB
    const size_t pressure_threshold = 4ULL * 1024 * 1024 * 1024;  // 4GB
    return std::min(1.0f, float(current_rss) / float(pressure_threshold));
}

void MemoryMonitor::monitoringThread() {
    while (monitoring_.load() && !shutdown_requested_.load()) {
        captureSnapshot();
        updateStats();
        pruneHistory();
        
        std::this_thread::sleep_for(monitoring_interval_);
    }
}

void MemoryMonitor::captureSnapshot() {
    MemorySnapshot snapshot;
    snapshot.timestamp = std::chrono::steady_clock::now();
    snapshot.rss_bytes = getCurrentRSS();
    snapshot.vms_bytes = getCurrentVMS();
    snapshot.heap_bytes = getCurrentHeap();
    snapshot.pattern_count = current_pattern_count_.load();
    snapshot.tier1_bytes = tier1_bytes_;
    snapshot.tier2_bytes = tier2_bytes_;
    snapshot.tier3_bytes = tier3_bytes_;
    snapshot.cache_hit_rate = cache_hit_rate_.load();
    
    std::lock_guard<std::mutex> lock(data_mutex_);
    history_.push_back(snapshot);
    last_snapshot_time_ = snapshot.timestamp;
}

void MemoryMonitor::updateStats() {
    std::lock_guard<std::mutex> lock(data_mutex_);
    
    if (history_.empty()) return;
    
    const auto& latest = history_.back();
    current_stats_.current_rss = latest.rss_bytes;
    current_stats_.current_heap = latest.heap_bytes;
    
    // Update peaks
    current_stats_.peak_rss = std::max(current_stats_.peak_rss, latest.rss_bytes);
    current_stats_.peak_heap = std::max(current_stats_.peak_heap, latest.heap_bytes);
    
    // Calculate growth rate
    if (history_.size() > 10) {
        const auto& old_snapshot = history_[history_.size() - 10];
        auto time_diff = std::chrono::duration_cast<std::chrono::milliseconds>(
            latest.timestamp - old_snapshot.timestamp).count() / 1000.0f;
        
        if (time_diff > 0) {
            float rss_diff_mb = (latest.rss_bytes - old_snapshot.rss_bytes) / (1024.0f * 1024.0f);
            current_stats_.growth_rate_mb_per_sec = rss_diff_mb / time_diff;
        }
    }
    
    // Calculate bytes per pattern
    if (latest.pattern_count > 0) {
        current_stats_.avg_bytes_per_pattern = 
            float(latest.tier1_bytes + latest.tier2_bytes + latest.tier3_bytes) / 
            float(latest.pattern_count);
    }
}

void MemoryMonitor::pruneHistory() {
    std::lock_guard<std::mutex> lock(data_mutex_);
    
    if (history_.size() > max_history_size_) {
        // Keep every Nth element to maintain history coverage
        size_t keep_every = 2;
        std::vector<MemorySnapshot> pruned;
        pruned.reserve(history_.size() / keep_every);
        
        for (size_t i = 0; i < history_.size(); i += keep_every) {
            pruned.push_back(history_[i]);
        }
        
        // Always keep the latest
        if (!history_.empty()) {
            pruned.push_back(history_.back());
        }
        
        history_ = std::move(pruned);
    }
}

size_t MemoryMonitor::getCurrentRSS() const {
#ifdef __linux__
    std::ifstream stat("/proc/self/status");
    std::string line;
    while (std::getline(stat, line)) {
        if (line.substr(0, 6) == "VmRSS:") {
            std::istringstream iss(line.substr(6));
            size_t rss_kb;
            iss >> rss_kb;
            return rss_kb * 1024;  // Convert to bytes
        }
    }
#elif _WIN32
    PROCESS_MEMORY_COUNTERS_EX pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
        return pmc.WorkingSetSize;
    }
#elif __APPLE__
    struct mach_task_basic_info info;
    mach_msg_type_number_t size = MACH_TASK_BASIC_INFO_COUNT;
    if (task_info(mach_task_self(), MACH_TASK_BASIC_INFO, (task_info_t)&info, &size) == KERN_SUCCESS) {
        return info.resident_size;
    }
#endif
    return 0;
}

size_t MemoryMonitor::getCurrentVMS() const {
#ifdef __linux__
    std::ifstream stat("/proc/self/status");
    std::string line;
    while (std::getline(stat, line)) {
        if (line.substr(0, 7) == "VmSize:") {
            std::istringstream iss(line.substr(7));
            size_t vms_kb;
            iss >> vms_kb;
            return vms_kb * 1024;  // Convert to bytes
        }
    }
#elif _WIN32
    PROCESS_MEMORY_COUNTERS_EX pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
        return pmc.PrivateUsage;
    }
#elif __APPLE__
    struct mach_task_basic_info info;
    mach_msg_type_number_t size = MACH_TASK_BASIC_INFO_COUNT;
    if (task_info(mach_task_self(), MACH_TASK_BASIC_INFO, (task_info_t)&info, &size) == KERN_SUCCESS) {
        return info.virtual_size;
    }
#endif
    return 0;
}

size_t MemoryMonitor::getCurrentHeap() const {
    // This is a simplified version - in production you might want to use
    // malloc_info() on Linux or similar platform-specific APIs
    
#ifdef __linux__
    // Parse /proc/self/status for heap info
    std::ifstream maps("/proc/self/maps");
    std::string line;
    size_t heap_size = 0;
    
    while (std::getline(maps, line)) {
        if (line.find("[heap]") != std::string::npos) {
            std::istringstream iss(line);
            std::string addr_range;
            iss >> addr_range;
            
            size_t dash_pos = addr_range.find('-');
            if (dash_pos != std::string::npos) {
                std::string start_str = addr_range.substr(0, dash_pos);
                std::string end_str = addr_range.substr(dash_pos + 1);
                
                size_t start = std::stoull(start_str, nullptr, 16);
                size_t end = std::stoull(end_str, nullptr, 16);
                heap_size = end - start;
            }
        }
    }
    return heap_size;
#else
    // Fallback: return a portion of RSS as heap estimate
    return getCurrentRSS() / 2;
#endif
}

} // namespace sep::workbench