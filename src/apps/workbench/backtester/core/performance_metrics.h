#pragma once

class PerformanceMetrics {
public:
    PerformanceMetrics();
    ~PerformanceMetrics();

    static float computeSharpeRatio(const std::vector<float>& pnl_series);
    static float computeMaxDrawdown(const std::vector<float>& pnl_series);
};
