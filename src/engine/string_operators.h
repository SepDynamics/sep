#pragma once

#include <string>

#include "engine/shim.h"

// Global operators for string comparison compatibility between shim::string and sep::shim::string
// These operators ensure proper string comparison in containers and algorithms

// Compare shim::string with sep::shim::string
inline bool operator==(const shim::string& lhs, const sep::shim::string& rhs)
{
    return lhs == rhs.c_str();
}

// Compare sep::shim::string with shim::string
inline bool operator==(const sep::shim::string& lhs, const shim::string& rhs)
{
    return shim::string(lhs.c_str()) == rhs;
}

// Additional comparison operators for completeness
inline bool operator!=(const shim::string& lhs, const sep::shim::string& rhs)
{
    return !(lhs == rhs);
}

inline bool operator!=(const sep::shim::string& lhs, const shim::string& rhs)
{
    return !(lhs == rhs);
}

// Less than operators for sorting and containers
inline bool operator<(const shim::string& lhs, const sep::shim::string& rhs)
{
    return lhs < shim::string(rhs.c_str());
}

inline bool operator<(const sep::shim::string& lhs, const shim::string& rhs)
{
    return shim::string(lhs.c_str()) < rhs;
}