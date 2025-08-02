#include "realtime_aggregator.hpp"
#include "candle_types.h"
#include <iostream>
#include <algorithm>

RealTimeAggregator::RealTimeAggregator(CandleCallback cb) 
    : on_candle_complete_(cb) {
    // Initialize boundaries for all timeframes
    for (int tf : timeframes_) {
        current_boundaries_[tf] = 0;
    }
}

void RealTimeAggregator::addM1Candle(const Candle& m1_candle) {
    for (int timeframe : timeframes_) {
        processTimeframe(m1_candle, timeframe);
    }
}

void RealTimeAggregator::processTimeframe(const Candle& m1_candle, int timeframe_minutes) {
    time_t candle_boundary = getTimeframeBoundary(m1_candle.timestamp, timeframe_minutes);
    
    // Check if we've crossed into a new timeframe period
    if (current_boundaries_[timeframe_minutes] != 0 && 
        candle_boundary != current_boundaries_[timeframe_minutes]) {
        // Finalize the previous candle
        finalizeCandle(timeframe_minutes);
    }
    
    // Update or initialize the current candle
    PartialCandle& partial = current_candles_[timeframe_minutes];
    
    if (partial.count == 0) {
        // Start new candle
        partial.open = m1_candle.open;
        partial.high = m1_candle.high;
        partial.low = m1_candle.low;
        partial.close = m1_candle.close;
        partial.volume = m1_candle.volume;
        partial.start_time = candle_boundary;
        partial.count = 1;
    } else {
        // Update existing candle
        partial.high = std::max(partial.high, m1_candle.high);
        partial.low = std::min(partial.low, m1_candle.low);
        partial.close = m1_candle.close;
        partial.volume += m1_candle.volume;
        partial.count++;
    }
    
    current_boundaries_[timeframe_minutes] = candle_boundary;
}

void RealTimeAggregator::finalizeCandle(int timeframe_minutes) {
    if (current_candles_.find(timeframe_minutes) == current_candles_.end() ||
        current_candles_[timeframe_minutes].count == 0) {
        return;
    }
    
    const PartialCandle& partial = current_candles_[timeframe_minutes];
    
    // Create completed candle
    Candle completed_candle;
    completed_candle.timestamp = partial.start_time;
    completed_candle.open = partial.open;
    completed_candle.high = partial.high;
    completed_candle.low = partial.low;
    completed_candle.close = partial.close;
    completed_candle.volume = partial.volume;
    
    // Notify callback
    if (on_candle_complete_) {
        on_candle_complete_(completed_candle, timeframe_minutes);
    }
    
    // Reset the partial candle
    current_candles_[timeframe_minutes] = PartialCandle();
}

time_t RealTimeAggregator::getTimeframeBoundary(time_t timestamp, int timeframe_minutes) {
    // Convert to minutes since epoch
    time_t minutes_since_epoch = timestamp / 60;
    
    // Find the start of the timeframe period
    time_t period_start = (minutes_since_epoch / timeframe_minutes) * timeframe_minutes;
    
    // Convert back to seconds
    return period_start * 60;
}
