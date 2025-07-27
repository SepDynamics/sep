#pragma once
#include <chrono>
#include <deque>
#include <cstddef>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "imgui.h"
#include "common/financial_data_types.h"





namespace sep::workbench {


struct ChartZoom {
    double price_min = 0, price_max = 0;
    std::chrono::system_clock::time_point time_start, time_end;
    bool is_zoomed = false;
    ImVec2 zoom_start, zoom_end;
    bool is_selecting = false;
    size_t index_start = 0;
    size_t index_end = 0;
};


struct TechnicalIndicator {
    std::string name;
    std::vector<float> values;
    ImU32 color;
    float line_thickness = 1.5f;
    bool enabled = true;
    
    TechnicalIndicator() : name(""), color(IM_COL32(255, 255, 255, 255)) {}
    TechnicalIndicator(const std::string& n, ImU32 c) : name(n), color(c) {}
};

struct TrendLine {
    ImVec2 start_point;
    ImVec2 end_point;
    double start_price;
    double end_price;
    std::chrono::system_clock::time_point start_time;
    std::chrono::system_clock::time_point end_time;
    ImU32 color;
    float thickness = 2.0f;
    bool is_support = true; // true for support, false for resistance
    float strength = 1.0f; // number of touches/confirmations
    
    TrendLine() : start_price(0), end_price(0), color(IM_COL32(255, 255, 0, 255)) {}
};



struct EnhancedHoverInfo {
    bool active = false;
    ImVec2 position;
    double price;
    std::chrono::system_clock::time_point time;
        sep::common::CandleData* nearest_candle = nullptr;
        sep::common::SEPSignalData* nearest_sep_signal = nullptr;
    
    // Enhanced derived metrics
    float price_momentum = 0.0f;      // Rate of price change
    float volume_profile = 0.0f;      // Volume at this price level
    float sep_divergence = 0.0f;      // Divergence between price and SEP signal
    
    // Multi-timeframe data
    std::map<std::string, float> mtf_coherence; // 1m, 5m, 15m, 1h coherence
    
    // Support/Resistance proximity
    float nearest_support = 0.0f;
    float nearest_resistance = 0.0f;
    float s_r_strength = 0.0f;
    
    // Traditional indicators at hover point
    float rsi_value = 0.0f;
    float macd_value = 0.0f;
    float bb_position = 0.0f;  // Position within Bollinger Bands (0-1)
    
    // Market context
    float market_correlation = 0.0f;  // Correlation with other pairs
    std::string market_regime = "Unknown";  // Trending, Ranging, Breakout
};

}  // namespace sep::workbench