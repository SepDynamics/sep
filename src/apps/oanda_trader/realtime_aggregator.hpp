#pragma once
#include "candle_types.h" // For the Candle struct
#include <vector>
#include <functional>
#include <map>
#include <ctime>

class RealTimeAggregator {
public:
    // Callback to notify when a new candle is complete
    using CandleCallback = std::function<void(const Candle&, int)>; // Candle, timeframe_minutes

    RealTimeAggregator(CandleCallback cb);

    // Main entry point: process a new M1 candle
    void addM1Candle(const Candle& m1_candle);

private:
    void processTimeframe(const Candle& m1_candle, int timeframe_minutes);
    void finalizeCandle(int timeframe_minutes);
    time_t getTimeframeBoundary(time_t timestamp, int timeframe_minutes);

    CandleCallback on_candle_complete_;
    
    // Internal state for building candles
    struct PartialCandle {
        double open = 0.0;
        double high = 0.0;
        double low = 0.0;
        double close = 0.0;
        double volume = 0.0;
        time_t start_time = 0;
        int count = 0;
    };
    
    std::map<int, PartialCandle> current_candles_; // Key: timeframe_minutes
    std::map<int, time_t> current_boundaries_; // Key: timeframe_minutes
    
    // Supported timeframes
    std::vector<int> timeframes_ = {5, 15}; // M5, M15
};
