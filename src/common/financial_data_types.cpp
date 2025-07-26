#include "financial_data_types.h"
#include <iomanip>
#include <sstream>

namespace sep::common {

int64_t time_point_to_nanoseconds(const std::chrono::time_point<std::chrono::system_clock>& tp) {
    auto duration = tp.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
}

std::chrono::time_point<std::chrono::system_clock> parseTimestamp(const std::string& timestamp_str) {
    std::tm tm = {};
    std::stringstream ss(timestamp_str);
    ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S.%fZ");
    return std::chrono::system_clock::from_time_t(std::mktime(&tm));
}

} // namespace sep::common
