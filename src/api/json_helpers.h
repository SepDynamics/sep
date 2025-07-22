#pragma once

#include <nlohmann/json.hpp>
#include <string>

namespace sep {
namespace api {

inline nlohmann::json parse_json(const std::string& str) {
    std::string std_str(str.c_str(), str.size());
    return nlohmann::json::parse(std_str);
}

template<typename StringType>
inline nlohmann::json parse_json_safe(const StringType& str,
                                     nlohmann::json default_value = nlohmann::json{}) {
    try {
        return parse_json(str);
    } catch (const nlohmann::json::parse_error&) {
        return default_value;
    }
}

inline std::string to_std_string(const std::string& str) {
    return std::string(str.c_str(), str.size());
}

inline std::string to_std_string(const char* str) { return str ? std::string(str) : std::string(); }

} // namespace api
} // namespace sep
