#pragma once

#include <ctime>
#include <string>
#include <sstream>
#include <iomanip>

// Candle structure for multi-timeframe analysis
struct Candle {
    std::string time;
    time_t timestamp;
    double open, high, low, close, volume;
};

// Helper function to parse ISO timestamp to time_t
inline time_t parseTimestamp(const std::string& time_str) {
    std::tm tm = {};
    std::istringstream ss(time_str);
    ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S");
    return std::mktime(&tm);
}
