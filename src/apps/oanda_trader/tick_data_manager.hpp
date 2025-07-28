#pragma once

#include "connectors/oanda_connector.h"

// Forward declarations for CUDA types to avoid header conflicts
namespace sep::apps::cuda {
    struct CudaContext;
    struct WindowResult;
}
#include <chrono>
#include <memory>
#include <vector>
#include <deque>
#include <string>
#include <mutex>
#include <atomic>
#include <array>

namespace sep::apps {

/**
 * Manages tick-level data and rolling window calculations
 * Handles both historical tick retrieval and real-time tick processing
 */
class TickDataManager {
public:
    struct TickData {
        double price;
        double bid;
        double ask;
        uint64_t timestamp; // nanoseconds since epoch
        double volume;
    };
    
    struct WindowCalculation {
        double mean_price;
        double volatility; 
        double price_change;
        double pip_change;
        size_t tick_count;
        uint64_t window_start;
        uint64_t window_end;
    };
    
    static constexpr size_t MAX_TICK_HISTORY = 1000000; // 1M ticks for 48H
    static constexpr size_t CALCULATION_ARRAY_SIZE = 10000; // Rolling calculations storage
    
    TickDataManager();
    ~TickDataManager();

    /**
     * Initialize with OANDA connector
     */
    bool initialize(sep::connectors::OandaConnector* connector);

    /**
     * Retrieve tick-level historical data for 48H
     * Uses OANDA streaming to get all ticks, not just M1 candles
     */
    bool loadHistoricalTicks(const std::string& instrument = "EUR_USD");

    /**
     * Process new incoming tick and update rolling calculations
     */
    void processNewTick(const sep::connectors::MarketData& tick);

    /**
     * Get rolling window calculations (updated on each tick)
     */
    const std::vector<WindowCalculation>& getHourlyCalculations() const { return hourly_calculations_; }
    const std::vector<WindowCalculation>& getDailyCalculations() const { return daily_calculations_; }

    /**
     * Get current window sizes (configurable)
     */
    std::chrono::minutes getHourlyWindow() const { return hourly_window_; }
    std::chrono::hours getDailyWindow() const { return daily_window_; }

    /**
     * Update window sizes (triggers recalculation)
     */
    void setHourlyWindow(std::chrono::minutes window);
    void setDailyWindow(std::chrono::hours window);

    /**
     * Get tick count and data quality metrics
     */
    size_t getTickCount() const { return tick_history_.size(); }
    double getAverageTicksPerMinute() const;
    bool isDataReady() const { return data_ready_; }

    /**
     * Get latest calculations for plotting
     */
    std::vector<double> getHourlyPrices() const;
    std::vector<double> getDailyPrices() const;
    std::vector<uint64_t> getTimestamps() const;

private:
    sep::connectors::OandaConnector* oanda_connector_;
    mutable std::mutex data_mutex_;
    std::atomic<bool> data_ready_{false};
    
    // Tick storage (circular buffer for memory efficiency)
    std::deque<TickData> tick_history_;
    
    // Rolling window calculations
    std::vector<WindowCalculation> hourly_calculations_;
    std::vector<WindowCalculation> daily_calculations_;
    
    // Configurable window sizes (reduced for faster testing)
    std::chrono::minutes hourly_window_{60};    // 1 hour default
    std::chrono::hours daily_window_{1};        // 1 hour default (reduced from 24H)
    
    // Performance optimization
    size_t last_calculation_index_ = 0;
    uint64_t last_calculation_time_ = 0;
    
    // Helper methods
    void calculateRollingWindows(const TickData& new_tick);
    WindowCalculation calculateWindow(const std::deque<TickData>& ticks, 
                                    uint64_t window_start, 
                                    uint64_t window_end) const;
    void maintainTickHistory();
    void recalculateAllWindows();
    void calculateWindowsCPU(); // CPU-only calculation without CUDA recursion
    
    // CUDA acceleration
    bool initializeCuda();
    void calculateWindowsCudaAccelerated();
    bool cuda_enabled_ = false;
    std::unique_ptr<cuda::CudaContext> cuda_context_;
};

} // namespace sep::apps
