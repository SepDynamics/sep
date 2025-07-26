#include "performance_metrics.h"

#include <cmath>
#include <numeric>
#include <vector>

PerformanceMetrics::PerformanceMetrics() {
}

PerformanceMetrics::~PerformanceMetrics() {
}

float PerformanceMetrics::computeSharpeRatio(const std::vector<float>& pnl_series) {
    if (pnl_series.empty()) {
        return 0.0f;
    }

    float sum = std::accumulate(pnl_series.begin(), pnl_series.end(), 0.0f);
    float mean = sum / static_cast<float>(pnl_series.size());

    float variance = 0.0f;
    for (float p : pnl_series) {
        float diff = p - mean;
        variance += diff * diff;
    }
    if (pnl_series.size() > 1) {
        variance /= static_cast<float>(pnl_series.size() - 1);
    }

    float std_dev = std::sqrt(variance);
    if (std_dev == 0.0f) {
        return 0.0f;
    }

    float sharpe = mean / std_dev;
    sharpe *= std::sqrt(static_cast<float>(pnl_series.size()));
    return sharpe;
}

float PerformanceMetrics::computeMaxDrawdown(const std::vector<float>& pnl_series) {
    if (pnl_series.empty()) {
        return 0.0f;
    }

    float equity = 0.0f;
    float peak = 0.0f;
    float max_dd = 0.0f;
    for (float p : pnl_series) {
        equity += p;
        if (equity > peak) {
            peak = equity;
        }
        float drawdown = peak - equity;
        if (drawdown > max_dd) {
            max_dd = drawdown;
        }
    }
    return max_dd;
}
