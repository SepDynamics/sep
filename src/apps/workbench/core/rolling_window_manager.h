#pragma once

#include "common/candle_data.h"
#include <deque>
#include <cstddef>

class RollingWindowManager {
public:
    RollingWindowManager(std::size_t max_size);

    void add_data(const sep::common::CandleData& data);
    const std::deque<sep::common::CandleData>& get_window() const;

private:
    std::deque<sep::common::CandleData> window_;
    std::size_t max_size_;
};
