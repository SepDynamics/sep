#include "rolling_window_chart.hpp"
#include "tick_data_manager.hpp"
#include <imgui.h>
#include <implot.h>
#include <iostream>
#include <chrono>

namespace sep::apps {

RollingWindowChart::RollingWindowChart() = default;

void RollingWindowChart::setTickManager(std::shared_ptr<TickDataManager> tick_manager) {
    tick_manager_ = tick_manager;
}

void RollingWindowChart::render() {
    if (!tick_manager_ || !tick_manager_->isDataReady()) {
        ImGui::Begin("Rolling Window Analysis - Initializing...");
        ImGui::Text("Loading tick data...");
        if (tick_manager_) {
            ImGui::Text("Current tick count: %zu", tick_manager_->getTickCount());
        }
        ImGui::End();
        return;
    }

    ImGui::Begin("🔄 Rolling Window Analysis - Real-Time", nullptr, ImGuiWindowFlags_NoCollapse);
    
    // Window configuration controls
    ImGui::Text("📊 Live Rolling Window Calculations");
    ImGui::Separator();
    
    ImGui::Text("Data Status:");
    ImGui::BulletText("Total ticks: %zu", tick_manager_->getTickCount());
    ImGui::BulletText("Avg ticks/min: %.1f", tick_manager_->getAverageTicksPerMinute());
    ImGui::BulletText("Hourly calculations: %zu", tick_manager_->getHourlyCalculations().size());
    ImGui::BulletText("Daily calculations: %zu", tick_manager_->getDailyCalculations().size());
    
    ImGui::Separator();
    
    // Window size controls
    ImGui::Text("Window Configuration:");
    bool window_changed = false;
    
    if (ImGui::SliderInt("Hourly Window (minutes)", &hourly_window_minutes_, 5, 300)) {
        window_changed = true;
    }
    
    if (ImGui::SliderInt("Daily Window (hours)", &daily_window_hours_, 1, 72)) {
        window_changed = true;
    }
    
    if (window_changed) {
        updateWindowSizes(hourly_window_minutes_, daily_window_hours_);
    }
    
    ImGui::Separator();
    
    // Display toggles
    ImGui::Checkbox("Show Hourly Windows", &show_hourly_);
    ImGui::SameLine();
    ImGui::Checkbox("Show Daily Windows", &show_daily_);
    ImGui::SameLine();
    ImGui::Checkbox("Auto Scale", &auto_scale_);
    
    // Update chart data
    updateChartData();
    
    // Render plots
    if (ImPlot::BeginPlot("Rolling Window Prices", ImVec2(-1, 200))) {
        ImPlot::SetupAxes("Time", "Price", 
                         auto_scale_ ? ImPlotAxisFlags_AutoFit : 0, 
                         auto_scale_ ? ImPlotAxisFlags_AutoFit : 0);
        
        if (show_hourly_ && !hourly_prices_.empty()) {
            ImPlot::SetNextLineStyle(ImVec4(0, 1, 0, 1)); // Green for hourly
            ImPlot::PlotLine("Hourly Mean", 
                           timestamps_plot_.data(), hourly_prices_.data(), 
                           static_cast<int>(std::min(hourly_prices_.size(), timestamps_plot_.size())));
        }
        
        if (show_daily_ && !daily_prices_.empty()) {
            ImPlot::SetNextLineStyle(ImVec4(0, 0, 1, 1)); // Blue for daily
            ImPlot::PlotLine("Daily Mean", 
                           timestamps_plot_.data(), daily_prices_.data(), 
                           static_cast<int>(std::min(daily_prices_.size(), timestamps_plot_.size())));
        }
        
        ImPlot::EndPlot();
    }
    
    if (ImPlot::BeginPlot("Rolling Window Volatility", ImVec2(-1, 200))) {
        ImPlot::SetupAxes("Time", "Volatility", 
                         auto_scale_ ? ImPlotAxisFlags_AutoFit : 0, 
                         auto_scale_ ? ImPlotAxisFlags_AutoFit : 0);
        
        if (show_hourly_ && !hourly_volatility_.empty()) {
            ImPlot::SetNextLineStyle(ImVec4(1, 0.5, 0, 1)); // Orange for hourly
            ImPlot::PlotLine("Hourly Volatility", 
                           timestamps_plot_.data(), hourly_volatility_.data(), 
                           static_cast<int>(std::min(hourly_volatility_.size(), timestamps_plot_.size())));
        }
        
        if (show_daily_ && !daily_volatility_.empty()) {
            ImPlot::SetNextLineStyle(ImVec4(1, 0, 1, 1)); // Magenta for daily
            ImPlot::PlotLine("Daily Volatility", 
                           timestamps_plot_.data(), daily_volatility_.data(), 
                           static_cast<int>(std::min(daily_volatility_.size(), timestamps_plot_.size())));
        }
        
        ImPlot::EndPlot();
    }
    
    // Show latest calculations
    ImGui::Separator();
    ImGui::Text("Latest Calculations:");
    
    auto hourly_calcs = tick_manager_->getHourlyCalculations();
    auto daily_calcs = tick_manager_->getDailyCalculations();
    
    if (!hourly_calcs.empty()) {
        const auto& latest_hourly = hourly_calcs.back();
        ImGui::Text("Hourly Window (%d min):", hourly_window_minutes_);
        ImGui::BulletText("Mean Price: %.5f", latest_hourly.mean_price);
        ImGui::BulletText("Volatility: %.5f", latest_hourly.volatility);
        ImGui::BulletText("Pip Change: %.2f", latest_hourly.pip_change);
        ImGui::BulletText("Tick Count: %zu", latest_hourly.tick_count);
    }
    
    if (!daily_calcs.empty()) {
        const auto& latest_daily = daily_calcs.back();
        ImGui::Text("Daily Window (%d hrs):", daily_window_hours_);
        ImGui::BulletText("Mean Price: %.5f", latest_daily.mean_price);
        ImGui::BulletText("Volatility: %.5f", latest_daily.volatility);
        ImGui::BulletText("Pip Change: %.2f", latest_daily.pip_change);
        ImGui::BulletText("Tick Count: %zu", latest_daily.tick_count);
    }
    
    ImGui::End();
}

void RollingWindowChart::updateWindowSizes(int hourly_minutes, int daily_hours) {
    if (!tick_manager_) return;
    
    std::cout << "[RollingWindowChart] Updating window sizes: " 
              << hourly_minutes << " min, " << daily_hours << " hrs" << std::endl;
    
    tick_manager_->setHourlyWindow(std::chrono::minutes(hourly_minutes));
    tick_manager_->setDailyWindow(std::chrono::hours(daily_hours));
    
    hourly_window_minutes_ = hourly_minutes;
    daily_window_hours_ = daily_hours;
}

void RollingWindowChart::updateChartData() {
    if (!tick_manager_) return;
    
    // Get latest calculation arrays
    hourly_prices_ = tick_manager_->getHourlyPrices();
    daily_prices_ = tick_manager_->getDailyPrices();
    
    // Extract volatility from calculations
    hourly_volatility_.clear();
    daily_volatility_.clear();
    
    auto hourly_calcs = tick_manager_->getHourlyCalculations();
    auto daily_calcs = tick_manager_->getDailyCalculations();
    
    for (const auto& calc : hourly_calcs) {
        hourly_volatility_.push_back(calc.volatility);
    }
    
    for (const auto& calc : daily_calcs) {
        daily_volatility_.push_back(calc.volatility);
    }
    
    // Convert timestamps for plotting
    auto timestamps = tick_manager_->getTimestamps();
    timestamps_plot_ = convertTimestamps(timestamps);
}

std::vector<double> RollingWindowChart::convertTimestamps(const std::vector<uint64_t>& timestamps) {
    std::vector<double> converted;
    converted.reserve(timestamps.size());
    
    // Convert nanoseconds to seconds for plotting
    for (uint64_t ts : timestamps) {
        double seconds = static_cast<double>(ts) / 1e9;
        converted.push_back(seconds);
    }
    
    return converted;
}

} // namespace sep::apps
