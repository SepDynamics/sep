#pragma once

#include <chrono>
#include <cstdint>
#include <string>

namespace sep::common {

struct CandleData {
    std::chrono::time_point<std::chrono::system_clock> timestamp;
    double open;
    double high;
    double low;
    double close;
    double volume;
};

struct SEPSignalData {
    // Assuming some fields based on the name
    std::string signal_id;
    std::chrono::time_point<std::chrono::system_clock> timestamp;
    double signal_value;
};

struct CorrelationMetrics {
    // Assuming some fields based on the name
    std::string metric_id;
    double correlation_coefficient;
    double p_value;
};


enum class OrderStatus { PENDING, FILLED, CANCELED };

struct OrderInfo {
    std::string id;
    std::string instrument;
    double units{0};
    double price{0};
    OrderStatus status{OrderStatus::PENDING};
};

} // namespace sep::common
