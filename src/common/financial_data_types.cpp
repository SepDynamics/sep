#include "financial_data_types.h"

#include <iomanip>
#include <sstream>

namespace sep {
namespace common {

std::chrono::time_point<std::chrono::system_clock> parseTimestamp(const std::string& timestamp_str) {
    std::tm tm = {};
    std::stringstream ss(timestamp_str);
    ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S");
    return std::chrono::system_clock::from_time_t(std::mktime(&tm));
}

} // namespace common
} // namespace sep
