#pragma once

#include <nlohmann/json.hpp>
#include <string>

#include "engine/shim.h"

namespace sep {
namespace api {

inline nlohmann::json parse_json(const sep::shim::string& str) {
    shim::string std_str(str.c_str(), str.size());
    return nlohmann::json::parse(std_str);
}

inline nlohmann::json parse_json(const shim::string& str) { return nlohmann::json::parse(str); }

inline nlohmann::json parse_json(const char* str) {
    return nlohmann::json::parse(str);
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

inline shim::string to_std_string(const sep::shim::string& str)
{
    return shim::string(str.c_str(), str.size());
}

inline shim::string to_std_string(const shim::string& str) { return str; }

inline shim::string to_std_string(const char* str)
{
    return str ? shim::string(str) : shim::string();
}

} // namespace api
} // namespace sep
