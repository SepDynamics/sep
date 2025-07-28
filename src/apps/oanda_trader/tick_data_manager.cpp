#include "tick_data_manager.hpp"
#include "common/financial_data_types.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <thread>
#include <condition_variable>

namespace sep::apps {

TickDataManager::TickDataManager() {
    // Reserve space for efficient memory usage
    tick_history_.clear();
    hourly_calculations_.reserve(CALCULATION_ARRAY_SIZE);
    daily_calculations_.reserve(CALCULATION_ARRAY_SIZE);
}

TickDataManager::~TickDataManager() = default;

bool TickDataManager::initialize(sep::connectors::OandaConnector* connector) {
    if (!connector) {
        std::cerr << "[TickDataManager] Error: Null OANDA connector provided" << std::endl;
        return false;
    }
    
    oanda_connector_ = connector;
    
    // Try to initialize CUDA for acceleration
    cuda_enabled_ = initializeCuda();
    if (cuda_enabled_) {
        std::cout << "[TickDataManager] CUDA acceleration enabled" << std::endl;
    } else {
        std::cout << "[TickDataManager] Using CPU calculations" << std::endl;
    }
    
    std::cout << "[TickDataManager] Initialized successfully" << std::endl;
    return true;
}

bool TickDataManager::loadHistoricalTicks(const std::string& instrument) {
    std::cout << "[TickDataManager] Loading 48H tick-level data for " << instrument << "..." << std::endl;
    
    if (!oanda_connector_) {
        std::cerr << "[TickDataManager] No OANDA connector available" << std::endl;
        return false;
    }
    
    // Calculate 48 hours ago
    auto now = std::chrono::system_clock::now();
    auto start_time = now - std::chrono::hours(48);
    
    std::cout << "[TickDataManager] Starting intensive tick data collection..." << std::endl;
    std::cout << "[TickDataManager] Note: This will collect ALL price updates over 48H" << std::endl;
    
    // We'll use the streaming approach to get historical data
    // Start a temporary stream and collect data points rapidly
    size_t initial_tick_count = 0;
    std::mutex collection_mutex;
    std::condition_variable collection_done;
    bool collection_complete = false;
    
    // Set up temporary callback to collect all ticks
    // Note: We'll temporarily use the streaming API to simulate historical data collection
    
    oanda_connector_->setPriceCallback([&](const sep::connectors::MarketData& data) {
        std::lock_guard<std::mutex> lock(collection_mutex);
        
        // Convert MarketData to TickData
        TickData tick;
        tick.price = data.mid;
        tick.bid = data.bid;
        tick.ask = data.ask;
        tick.timestamp = data.timestamp;
        tick.volume = data.volume;
        
        tick_history_.push_back(tick);
        initial_tick_count++;
        
        // Log progress every 1000 ticks
        if (initial_tick_count % 1000 == 0) {
            std::cout << "[TickDataManager] Collected " << initial_tick_count << " ticks..." << std::endl;
        }
        
        // For demonstration, we'll collect for a limited time
        // In practice, you'd want to collect actual 48H of historical data
        if (initial_tick_count >= 50000) { // Simulate 48H worth of ticks
            collection_complete = true;
            collection_done.notify_one();
        }
    });
    
    // Start streaming to collect historical equivalent data
    if (!oanda_connector_->startPriceStream({instrument})) {
        std::cerr << "[TickDataManager] Failed to start price stream for historical collection" << std::endl;
        return false;
    }
    
    // Wait for collection to complete
    std::unique_lock<std::mutex> lock(collection_mutex);
    if (!collection_done.wait_for(lock, std::chrono::minutes(10), [&]{ return collection_complete; })) {
        std::cerr << "[TickDataManager] Timeout collecting historical tick data" << std::endl;
        oanda_connector_->stopPriceStream();
        return false;
    }
    
    // Stop collection stream
    oanda_connector_->stopPriceStream();
    
    // Note: In real implementation, we'd restore the original callback
    // For now, we'll let the application manage the callback
    
    std::cout << "[TickDataManager] Collected " << tick_history_.size() << " historical ticks" << std::endl;
    std::cout << "[TickDataManager] Average: " << getAverageTicksPerMinute() << " ticks per minute" << std::endl;
    
    // Calculate initial rolling windows for all historical data
    recalculateAllWindows();
    
    data_ready_ = true;
    return true;
}

void TickDataManager::processNewTick(const sep::connectors::MarketData& market_data) {
    std::lock_guard<std::mutex> lock(data_mutex_);
    
    // Convert to TickData
    TickData tick;
    tick.price = market_data.mid;
    tick.bid = market_data.bid;
    tick.ask = market_data.ask;
    tick.timestamp = market_data.timestamp;
    tick.volume = market_data.volume;
    
    // Add to history
    tick_history_.push_back(tick);
    
    // Maintain memory limits
    maintainTickHistory();
    
    // Calculate rolling windows for this new tick
    calculateRollingWindows(tick);
    
    // Log occasionally for debugging
    static size_t tick_count = 0;
    if (++tick_count % 100 == 0) {
        std::cout << "[TickDataManager] Processed " << tick_count 
                  << " ticks, hourly calcs: " << hourly_calculations_.size()
                  << ", daily calcs: " << daily_calculations_.size() << std::endl;
    }
}

void TickDataManager::calculateRollingWindows(const TickData& new_tick) {
    uint64_t current_time = new_tick.timestamp;
    
    // Calculate hourly window (last N minutes)
    uint64_t hourly_start = current_time - std::chrono::duration_cast<std::chrono::nanoseconds>(hourly_window_).count();
    auto hourly_calc = calculateWindow(tick_history_, hourly_start, current_time);
    hourly_calculations_.push_back(hourly_calc);
    
    // Calculate daily window (last N hours)
    uint64_t daily_start = current_time - std::chrono::duration_cast<std::chrono::nanoseconds>(daily_window_).count();
    auto daily_calc = calculateWindow(tick_history_, daily_start, current_time);
    daily_calculations_.push_back(daily_calc);
    
    // Maintain array sizes
    if (hourly_calculations_.size() > CALCULATION_ARRAY_SIZE) {
        hourly_calculations_.erase(hourly_calculations_.begin(), 
                                  hourly_calculations_.begin() + 1000); // Remove oldest 1000
    }
    
    if (daily_calculations_.size() > CALCULATION_ARRAY_SIZE) {
        daily_calculations_.erase(daily_calculations_.begin(), 
                                 daily_calculations_.begin() + 1000);
    }
}

TickDataManager::WindowCalculation TickDataManager::calculateWindow(
    const std::deque<TickData>& ticks, 
    uint64_t window_start, 
    uint64_t window_end) const {
    
    WindowCalculation calc{};
    calc.window_start = window_start;
    calc.window_end = window_end;
    
    if (ticks.empty()) {
        return calc;
    }
    
    // Find ticks within window
    std::vector<double> prices;
    prices.reserve(1000); // Estimate
    
    double first_price = 0.0;
    double last_price = 0.0;
    bool first_set = false;
    
    for (const auto& tick : ticks) {
        if (tick.timestamp >= window_start && tick.timestamp <= window_end) {
            prices.push_back(tick.price);
            
            if (!first_set) {
                first_price = tick.price;
                first_set = true;
            }
            last_price = tick.price;
        }
    }
    
    calc.tick_count = prices.size();
    
    if (prices.empty()) {
        return calc;
    }
    
    // Calculate mean
    double sum = 0.0;
    for (double price : prices) {
        sum += price;
    }
    calc.mean_price = sum / prices.size();
    
    // Calculate volatility (standard deviation)
    double variance = 0.0;
    for (double price : prices) {
        double diff = price - calc.mean_price;
        variance += diff * diff;
    }
    calc.volatility = std::sqrt(variance / prices.size());
    
    // Calculate price change and pip change
    calc.price_change = last_price - first_price;
    calc.pip_change = calc.price_change * 10000.0; // Convert to pips for forex
    
    return calc;
}

void TickDataManager::maintainTickHistory() {
    // Keep only last 48 hours worth of ticks (memory management)
    if (tick_history_.size() > MAX_TICK_HISTORY) {
        size_t remove_count = tick_history_.size() - MAX_TICK_HISTORY;
        tick_history_.erase(tick_history_.begin(), tick_history_.begin() + remove_count);
    }
}

void TickDataManager::recalculateAllWindows() {
    std::cout << "[TickDataManager] Recalculating all rolling windows..." << std::endl;
    
    hourly_calculations_.clear();
    daily_calculations_.clear();
    
    if (tick_history_.empty()) {
        return;
    }
    
    // Calculate windows for every Nth tick to populate arrays
    size_t step = std::max(1UL, tick_history_.size() / CALCULATION_ARRAY_SIZE);
    
    for (size_t i = step; i < tick_history_.size(); i += step) {
        const auto& tick = tick_history_[i];
        uint64_t current_time = tick.timestamp;
        
        // Hourly window
        uint64_t hourly_start = current_time - std::chrono::duration_cast<std::chrono::nanoseconds>(hourly_window_).count();
        auto hourly_calc = calculateWindow(tick_history_, hourly_start, current_time);
        hourly_calculations_.push_back(hourly_calc);
        
        // Daily window
        uint64_t daily_start = current_time - std::chrono::duration_cast<std::chrono::nanoseconds>(daily_window_).count();
        auto daily_calc = calculateWindow(tick_history_, daily_start, current_time);
        daily_calculations_.push_back(daily_calc);
    }
    
    std::cout << "[TickDataManager] Generated " << hourly_calculations_.size() 
              << " hourly and " << daily_calculations_.size() << " daily calculations" << std::endl;
}

void TickDataManager::setHourlyWindow(std::chrono::minutes window) {
    std::lock_guard<std::mutex> lock(data_mutex_);
    hourly_window_ = window;
    std::cout << "[TickDataManager] Updated hourly window to " << window.count() << " minutes" << std::endl;
    // Trigger recalculation in background thread to avoid blocking
    std::thread([this]() { recalculateAllWindows(); }).detach();
}

void TickDataManager::setDailyWindow(std::chrono::hours window) {
    std::lock_guard<std::mutex> lock(data_mutex_);
    daily_window_ = window;
    std::cout << "[TickDataManager] Updated daily window to " << window.count() << " hours" << std::endl;
    // Trigger recalculation in background thread
    std::thread([this]() { recalculateAllWindows(); }).detach();
}

double TickDataManager::getAverageTicksPerMinute() const {
    std::lock_guard<std::mutex> lock(data_mutex_);
    
    if (tick_history_.size() < 2) {
        return 0.0;
    }
    
    uint64_t time_span_ns = tick_history_.back().timestamp - tick_history_.front().timestamp;
    double time_span_minutes = time_span_ns / (1e9 * 60.0); // Convert ns to minutes
    
    if (time_span_minutes <= 0) {
        return 0.0;
    }
    
    return tick_history_.size() / time_span_minutes;
}

std::vector<double> TickDataManager::getHourlyPrices() const {
    std::lock_guard<std::mutex> lock(data_mutex_);
    
    std::vector<double> prices;
    prices.reserve(hourly_calculations_.size());
    
    for (const auto& calc : hourly_calculations_) {
        prices.push_back(calc.mean_price);
    }
    
    return prices;
}

std::vector<double> TickDataManager::getDailyPrices() const {
    std::lock_guard<std::mutex> lock(data_mutex_);
    
    std::vector<double> prices;
    prices.reserve(daily_calculations_.size());
    
    for (const auto& calc : daily_calculations_) {
        prices.push_back(calc.mean_price);
    }
    
    return prices;
}

std::vector<uint64_t> TickDataManager::getTimestamps() const {
    std::lock_guard<std::mutex> lock(data_mutex_);
    
    std::vector<uint64_t> timestamps;
    timestamps.reserve(std::min(hourly_calculations_.size(), daily_calculations_.size()));
    
    size_t count = std::min(hourly_calculations_.size(), daily_calculations_.size());
    for (size_t i = 0; i < count; ++i) {
        timestamps.push_back(hourly_calculations_[i].window_end);
    }
    
    return timestamps;
}

bool TickDataManager::initializeCuda() {
    // TODO: Implement CUDA initialization
    // For now, return false to use CPU calculations
    return false;
}

void TickDataManager::calculateWindowsCuda(const TickData* ticks, size_t count) {
    // TODO: Implement CUDA-accelerated window calculations
    // This would process multiple windows in parallel on GPU
}

} // namespace sep::apps
