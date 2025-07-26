#pragma once

#include <string>
#include <cstdint>

namespace sep {

    // Raw candle data structure matching the JSON format
    struct CandleData
    {
        std::string time;
        uint64_t volume;
        float open;
        float high;
        float low;
        float close;
    };

} // namespace sep