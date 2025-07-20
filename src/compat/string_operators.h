#pragma once

#include <string>

#include "shim.h"

// Global operators for string comparison compatibility between std::string and sep::shim::string
// These operators ensure proper string comparison in containers and algorithms

// Compare std::string with sep::shim::string
inline bool operator==(const std::string& lhs, const sep::shim::string& rhs) {
    return lhs == rhs.c_str();
}

// Compare sep::shim::string with std::string
inline bool operator==(const sep::shim::string& lhs, const std::string& rhs) {
    return std::string(lhs.c_str()) == rhs;
}

// Additional comparison operators for completeness
inline bool operator!=(const std::string& lhs, const sep::shim::string& rhs) {
    return !(lhs == rhs);
}

inline bool operator!=(const sep::shim::string& lhs, const std::string& rhs) {
    return !(lhs == rhs);
}

// Less than operators for sorting and containers
inline bool operator<(const std::string& lhs, const sep::shim::string& rhs) {
    return lhs < std::string(rhs.c_str());
}

inline bool operator<(const sep::shim::string& lhs, const std::string& rhs) {
    return std::string(lhs.c_str()) < rhs;
}