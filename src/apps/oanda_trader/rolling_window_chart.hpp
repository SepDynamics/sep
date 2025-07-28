#pragma once

#include <memory>
#include <vector>
#include <cstdint>

namespace sep::apps {

// Forward declarations
class TickDataManager;

/**
 * Chart component for displaying rolling window calculations
 * Shows real-time windowing data with configurable parameters
 */
class RollingWindowChart {
public:
    RollingWindowChart();
    ~RollingWindowChart() = default;

    /**
     * Initialize with tick data manager
     */
    void setTickManager(std::shared_ptr<TickDataManager> tick_manager);

    /**
     * Render the chart UI with ImPlot
     */
    void render();

    /**
     * Update window parameters and trigger recalculation
     */
    void updateWindowSizes(int hourly_minutes, int daily_hours);

private:
    std::shared_ptr<TickDataManager> tick_manager_;
    
    // UI state
    int hourly_window_minutes_ = 60;
    int daily_window_hours_ = 24;
    bool show_hourly_ = true;
    bool show_daily_ = true;
    bool auto_scale_ = true;
    
    // Chart data (cached for performance)
    std::vector<double> hourly_prices_;
    std::vector<double> daily_prices_;
    std::vector<double> hourly_volatility_;
    std::vector<double> daily_volatility_;
    std::vector<double> timestamps_plot_;
    
    // Update cached data
    void updateChartData();
    
    // Helper to convert timestamps for plotting
    std::vector<double> convertTimestamps(const std::vector<uint64_t>& timestamps);
};

} // namespace sep::apps
