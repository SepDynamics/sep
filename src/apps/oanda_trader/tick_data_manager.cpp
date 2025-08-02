#include "tick_data_manager.hpp"
#include "tick_cuda_kernels.cuh"
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

TickDataManager::~TickDataManager() {
    // Cleanup CUDA resources if initialized
    if (cuda_enabled_ && cuda_context_ && cuda_context_->initialized) {
        std::cout << "[TickDataManager] Cleaning up CUDA resources..." << std::endl;
        cuda::cleanupCudaDevice(*cuda_context_);
    }
}

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
    std::cout << "[TickDataManager] Loading 2H tick-level data for " << instrument << "..." << std::endl;
    
    if (!oanda_connector_) {
        std::cerr << "[TickDataManager] No OANDA connector available" << std::endl;
        return false;
    }
    
    // Calculate 2 hours ago (reduced from 48H for faster initialization)
    auto now = std::chrono::system_clock::now();
    auto start_time = now - std::chrono::hours(2);
    
    std::cout << "[TickDataManager] Starting intensive tick data collection..." << std::endl;
    std::cout << "[TickDataManager] Note: This will collect ALL price updates over 2H" << std::endl;
    
    // Use OANDA's proper historical data API
    std::cout << "[TickDataManager] Fetching historical data from OANDA..." << std::endl;
    
    // Calculate time strings for OANDA API
    auto now_time = std::chrono::system_clock::now();
    auto from_time = now_time - std::chrono::hours(2);
    
    // Format times for OANDA API
    auto from_time_t = std::chrono::system_clock::to_time_t(from_time);
    auto to_time_t = std::chrono::system_clock::to_time_t(now_time);
    
    char from_str[32], to_str[32];
    std::strftime(from_str, sizeof(from_str), "%Y-%m-%dT%H:%M:%SZ", std::gmtime(&from_time_t));
    std::strftime(to_str, sizeof(to_str), "%Y-%m-%dT%H:%M:%SZ", std::gmtime(&to_time_t));
    
    std::mutex collection_mutex;
    std::condition_variable collection_done;
    bool collection_complete = false;
    
    std::cout << "[TickDataManager] Requesting data from " << from_str << " to " << to_str << std::endl;
    
    // Use getHistoricalData with S5 granularity (5-second candles for tick-like data)
    auto candles = oanda_connector_->getHistoricalData(
        instrument,
        "S5", // 5-second candles for high frequency
        from_str,
        to_str
    );

    std::lock_guard<std::mutex> lock(collection_mutex);
    
    std::cout << "[TickDataManager] Received " << candles.size() << " candles from OANDA" << std::endl;
    
    // Convert candles to ticks
    static auto base_time = std::chrono::system_clock::now() - std::chrono::hours(2);
    size_t index = 0;
    for (const auto& candle : candles) {
        TickData tick;
        tick.price = (candle.open + candle.close) / 2.0; // Mid price
        tick.bid = candle.close - 0.00005; // Approximate bid
        tick.ask = candle.close + 0.00005; // Approximate ask
        
        // Convert time string to timestamp
        // Use proper historical timestamps with appropriate spacing (5-second intervals)
        auto historical_time = base_time + std::chrono::seconds(index * 5);
        tick.timestamp = std::chrono::duration_cast<std::chrono::nanoseconds>(
            historical_time.time_since_epoch()).count();
        
        tick.volume = static_cast<double>(candle.volume);
        
        tick_history_.push_back(tick);
        index++;
    }
    
    collection_complete = true;
    collection_done.notify_one();
    
    // Wait for historical data to arrive
    std::unique_lock<std::mutex> ulock(collection_mutex);
    if (!collection_done.wait_for(ulock, std::chrono::seconds(30), [&]{ return collection_complete; })) {
        std::cerr << "[TickDataManager] Timeout fetching historical data" << std::endl;
        return false;
    }
    
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
                  << ", hourly calcs: " << hourly_calculations_.size()
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
    // Keep only last 2 hours worth of ticks (memory management)
    if (tick_history_.size() > MAX_TICK_HISTORY) {
        size_t remove_count = tick_history_.size() - MAX_TICK_HISTORY;
        tick_history_.erase(tick_history_.begin(), tick_history_.begin() + remove_count);
    }
}

void TickDataManager::recalculateAllWindows() {
    std::cout << "[TickDataManager] Recalculating all rolling windows..." << std::endl;
    
    // Use CUDA acceleration if available
    if (cuda_enabled_ && cuda_context_ && cuda_context_->initialized) {
        calculateWindowsCudaAccelerated();
        return;
    }
    
    // Fallback to CPU calculation
    std::cout << "[TickDataManager] Using CPU calculations..." << std::endl;
    
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
    
    std::cout << "[TickDataManager] CPU calculations completed! Generated " << hourly_calculations_.size() 
              << " hourly and " << daily_calculations_.size() << " daily calculations" << std::endl;
}

void TickDataManager::calculateWindowsCPU() {
    std::cout << "[TickDataManager] Using CPU calculations..." << std::endl;
    
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
    
    std::cout << "[TickDataManager] CPU calculations completed! Generated " << hourly_calculations_.size() 
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
    std::cout << "[TickDataManager] Initializing CUDA acceleration..." << std::endl;
    
    cuda_context_ = std::make_unique<cuda::CudaContext>();
    cudaError_t error = cuda::initializeCudaDevice(*cuda_context_);
    if (error != cudaSuccess) {
        std::cerr << "[TickDataManager] CUDA initialization failed: " << cudaGetErrorString(error) << std::endl;
        cuda_context_.reset();
        return false;
    }
    
    std::cout << "[TickDataManager] CUDA acceleration initialized successfully!" << std::endl;
    return true;
}

void TickDataManager::calculateWindowsCudaAccelerated() {
    if (!cuda_enabled_ || !cuda_context_ || !cuda_context_->initialized) {
        std::cerr << "[TickDataManager] CUDA not available, falling back to CPU" << std::endl;
        // Don't call recalculateAllWindows() - would cause recursion
        return;
    }
    
    std::cout << "[TickDataManager] Running CUDA-accelerated window calculations..." << std::endl;
    
    // TEMPORARY: Disable CUDA for debugging - use CPU calculation directly
    std::cout << "[TickDataManager] CUDA temporarily disabled for debugging, using CPU" << std::endl;
    
    // Call CPU calculation directly to avoid recursion
    calculateWindowsCPU();
    return;
    
    // Convert tick history to CUDA format
    std::vector<cuda::TickData> cuda_ticks;
    cuda_ticks.reserve(tick_history_.size());
    
    for (const auto& tick : tick_history_) {
        cuda::TickData cuda_tick;
        cuda_tick.price = tick.price;
        cuda_tick.bid = tick.bid;
        cuda_tick.ask = tick.ask;
        cuda_tick.timestamp = tick.timestamp;
        cuda_tick.volume = tick.volume;
        cuda_ticks.push_back(cuda_tick);
    }
    
    // Prepare result arrays
    size_t calculation_count = std::min(CALCULATION_ARRAY_SIZE, tick_history_.size());
    std::vector<cuda::WindowResult> hourly_cuda_results(calculation_count);
    std::vector<cuda::WindowResult> daily_cuda_results(calculation_count);
    
    // Calculate current time and window sizes in nanoseconds
    uint64_t current_time = tick_history_.empty() ? 0 : tick_history_.back().timestamp;
    uint64_t hourly_window_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(hourly_window_).count();
    uint64_t daily_window_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(daily_window_).count();
    
    // Launch CUDA calculation
    cudaError_t error = cuda::calculateWindowsCuda(
        *cuda_context_,
        cuda_ticks,
        hourly_cuda_results,
        daily_cuda_results,
        current_time,
        hourly_window_ns,
        daily_window_ns
    );
    
    if (error != cudaSuccess) {
        std::cerr << "[TickDataManager] CUDA calculation failed: " << cudaGetErrorString(error) << std::endl;
        std::cerr << "[TickDataManager] Falling back to CPU calculations" << std::endl;
        recalculateAllWindows();
        return;
    }
    
    // Convert results back to internal format
    hourly_calculations_.clear();
    daily_calculations_.clear();
    
    for (const auto& cuda_result : hourly_cuda_results) {
        WindowCalculation calc;
        calc.mean_price = cuda_result.mean_price;
        calc.volatility = cuda_result.volatility;
        calc.price_change = cuda_result.price_change;
        calc.pip_change = cuda_result.pip_change;
        calc.tick_count = cuda_result.tick_count;
        calc.window_start = cuda_result.window_start;
        calc.window_end = cuda_result.window_end;
        hourly_calculations_.push_back(calc);
    }
    
    for (const auto& cuda_result : daily_cuda_results) {
        WindowCalculation calc;
        calc.mean_price = cuda_result.mean_price;
        calc.volatility = cuda_result.volatility;
        calc.price_change = cuda_result.price_change;
        calc.pip_change = cuda_result.pip_change;
        calc.tick_count = cuda_result.tick_count;
        calc.window_start = cuda_result.window_start;
        calc.window_end = cuda_result.window_end;
        daily_calculations_.push_back(calc);
    }
    
    std::cout << "[TickDataManager] CUDA calculations completed! Generated " 
              << hourly_calculations_.size() << " hourly and " 
              << daily_calculations_.size() << " daily calculations" << std::endl;
}

} // namespace sep::apps
