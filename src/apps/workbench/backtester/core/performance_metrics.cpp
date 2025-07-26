#include "performance_metrics.h"

#include <cmath>
#include <vector>

PerformanceMetrics::PerformanceMetrics() {
}

PerformanceMetrics::~PerformanceMetrics() {
}

float PerformanceMetrics::computeSharpeRatio(const std::vector<float>& pnl_series) {
    if (pnl_series.empty()) {
        return 0.0f;
    }

    float sum = 0.0f;
    for (float p : pnl_series) {
        sum += p;
    }
    float avg = sum / pnl_series.size();

    float var = 0.0f;
    for (float p : pnl_series) {
        var += (p - avg) * (p - avg);
    }
    float std_dev = std::sqrt(var / pnl_series.size());
    if (std_dev == 0.0f) {
        return 0.0f;
    }
    return avg / std_dev;
}

float PerformanceMetrics::computeMaxDrawdown(const std::vector<float>& pnl_series) {
    float running_total = 0.0f;
    float peak = 0.0f;
    float max_drawdown = 0.0f;

    for (float p : pnl_series) {
        running_total += p;
        if (running_total > peak) {
            peak = running_total;
        }
        float drawdown = peak - running_total;
        if (drawdown > max_drawdown) {
            max_drawdown = drawdown;
        }
    }
    return max_drawdown;
}
