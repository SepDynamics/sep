#include <iostream>
#include <vector>
#include "quantum/bitspace/forward_window_result.h"

// Simple test of our enhanced pattern logic
namespace {

// TrendAcceleration Pattern: Increasing frequency of changes towards the end
bool detectTrendAcceleration(const std::vector<uint8_t>& window) {
    if (window.size() < 6) return false;
    
    size_t half_size = window.size() / 2;
    int first_half_flips = 0, second_half_flips = 0;
    
    // Count flips in first half
    for (size_t i = 1; i < half_size; ++i) {
        if (window[i] != window[i-1]) first_half_flips++;
    }
    
    // Count flips in second half
    for (size_t i = half_size + 1; i < window.size(); ++i) {
        if (window[i] != window[i-1]) second_half_flips++;
    }
    
    // Trend acceleration: second half has significantly more flips
    return second_half_flips >= first_half_flips * 2;
}

// MeanReversion Pattern: High-low-high or low-high-low oscillation
bool detectMeanReversion(const std::vector<uint8_t>& window) {
    if (window.size() < 4) return false;
    
    // Look for patterns like 0-1-0-1-0 or 1-0-1-0-1 (mean reverting)
    int peaks = 0, valleys = 0;
    
    for (size_t i = 1; i < window.size() - 1; ++i) {
        // Peak: 1 surrounded by 0s
        if (window[i] == 1 && window[i-1] == 0 && window[i+1] == 0) peaks++;
        // Valley: 0 surrounded by 1s  
        if (window[i] == 0 && window[i-1] == 1 && window[i+1] == 1) valleys++;
    }
    
    // Mean reversion pattern: significant number of peaks/valleys
    return (peaks + valleys) >= static_cast<int>(window.size()) / 3;
}

// VolatilityBreakout Pattern: Quiet period followed by sudden activity
bool detectVolatilityBreakout(const std::vector<uint8_t>& window) {
    if (window.size() < 6) return false;
    
    size_t quiet_threshold = window.size() / 3;
    size_t active_threshold = window.size() / 3;
    
    // Find the longest quiet period (consecutive same values)
    size_t max_quiet_length = 0, current_quiet = 1;
    for (size_t i = 1; i < window.size(); ++i) {
        if (window[i] == window[i-1]) {
            current_quiet++;
        } else {
            max_quiet_length = std::max(max_quiet_length, current_quiet);
            current_quiet = 1;
        }
    }
    max_quiet_length = std::max(max_quiet_length, current_quiet);
    
    // Count total flips (activity)
    int total_flips = 0;
    for (size_t i = 1; i < window.size(); ++i) {
        if (window[i] != window[i-1]) total_flips++;
    }
    
    // Volatility breakout: long quiet period AND significant activity
    return max_quiet_length >= quiet_threshold && total_flips >= static_cast<int>(active_threshold);
}

} // anonymous namespace

int main() {
    // Test data for different patterns
    
    // Test 1: TrendAcceleration pattern (more flips in second half)
    std::vector<uint8_t> trend_accel = {0, 0, 0, 1, 0, 1, 0, 1, 0, 1};
    bool is_trend_accel = detectTrendAcceleration(trend_accel);
    std::cout << "TrendAcceleration pattern {0,0,0,1,0,1,0,1,0,1}:\n";
    std::cout << "  Detected: " << (is_trend_accel ? "YES" : "NO") << std::endl;
    std::cout << "  Should be detected: YES" << std::endl << std::endl;
    
    // Test 2: MeanReversion pattern (oscillating)
    std::vector<uint8_t> mean_revert = {1, 0, 1, 0, 1, 0, 1};
    bool is_mean_revert = detectMeanReversion(mean_revert);
    std::cout << "MeanReversion pattern {1,0,1,0,1,0,1}:\n";
    std::cout << "  Detected: " << (is_mean_revert ? "YES" : "NO") << std::endl;
    std::cout << "  Should be detected: YES" << std::endl << std::endl;
    
    // Test 3: VolatilityBreakout pattern (quiet then active)
    std::vector<uint8_t> vol_breakout = {0, 0, 0, 0, 1, 0, 1, 0, 1};
    bool is_vol_breakout = detectVolatilityBreakout(vol_breakout);
    std::cout << "VolatilityBreakout pattern {0,0,0,0,1,0,1,0,1}:\n";
    std::cout << "  Detected: " << (is_vol_breakout ? "YES" : "NO") << std::endl;
    std::cout << "  Should be detected: YES" << std::endl << std::endl;
    
    // Test 4: Standard alternating pattern (should NOT trigger enhanced patterns)
    std::vector<uint8_t> alternating = {0, 1, 0, 1, 0, 1, 0, 1};
    bool alt_trend = detectTrendAcceleration(alternating);
    bool alt_mean = detectMeanReversion(alternating);
    bool alt_vol = detectVolatilityBreakout(alternating);
    std::cout << "Standard alternating pattern {0,1,0,1,0,1,0,1}:\n";
    std::cout << "  TrendAccel: " << (alt_trend ? "YES" : "NO") << " (should be NO)" << std::endl;
    std::cout << "  MeanReversion: " << (alt_mean ? "YES" : "NO") << " (should be NO)" << std::endl;
    std::cout << "  VolBreakout: " << (alt_vol ? "YES" : "NO") << " (should be NO)" << std::endl << std::endl;
    
    return 0;
}
