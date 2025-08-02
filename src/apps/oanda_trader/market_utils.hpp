#pragma once

#include <chrono>
#include <ctime>

namespace SEP {

/**
 * Market Hours Utility
 * Determines if forex markets are currently open for trading.
 * Forex markets operate 24/5: Sunday 21:00 UTC to Friday 21:00 UTC
 */
class MarketUtils {
public:
    /**
     * Check if forex markets are currently open
     * @return true if markets are open, false during weekend closure
     */
    static bool isMarketOpen() {
        auto now = std::chrono::system_clock::now();
        auto now_time_t = std::chrono::system_clock::to_time_t(now);
        std::tm* gmt = std::gmtime(&now_time_t);
        
        int day_of_week = gmt->tm_wday;  // 0=Sunday, 1=Monday, ..., 6=Saturday
        int hour = gmt->tm_hour;         // 0-23 UTC
        
        // Market closed conditions:
        // Friday after 21:00 UTC
        if (day_of_week == 5 && hour >= 21) return false;
        
        // All of Saturday
        if (day_of_week == 6) return false;
        
        // Sunday before 21:00 UTC
        if (day_of_week == 0 && hour < 21) return false;
        
        return true;
    }
    
    /**
     * Get current market session name for logging
     * @return string describing current market session
     */
    static std::string getCurrentSession() {
        if (!isMarketOpen()) {
            return "MARKET_CLOSED";
        }
        
        auto now = std::chrono::system_clock::now();
        auto now_time_t = std::chrono::system_clock::to_time_t(now);
        std::tm* gmt = std::gmtime(&now_time_t);
        int hour = gmt->tm_hour;
        
        // Approximate session times (overlaps exist)
        if (hour >= 21 || hour < 8) return "SYDNEY_TOKYO";
        if (hour >= 8 && hour < 16) return "LONDON";
        if (hour >= 13 && hour < 22) return "NEW_YORK";
        
        return "MARKET_OPEN";
    }
};

} // namespace SEP
