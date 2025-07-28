#include "rolling_window_manager.h"

RollingWindowManager::RollingWindowManager(std::size_t max_size)
    : max_size_(max_size) {}

void RollingWindowManager::add_data(const sep::common::CandleData& data) {
    window_.push_back(data);
    if (window_.size() > max_size_) {
        window_.pop_front();
    }
}

const std::deque<sep::common::CandleData>& RollingWindowManager::get_window() const {
    return window_;
}
