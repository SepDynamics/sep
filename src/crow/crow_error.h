#pragma once

#include "engine/shim/string.h"

namespace sep {
namespace crow {
namespace error {

enum class Code {
    None = 0,
    InvalidRequest = 1,
    InvalidResponse = 2,
    NetworkError = 3,
    ServerError = 4
};

void log(Code code, const sep::string& message);

} // namespace error
} // namespace crow
} // namespace sep

#if !defined(__cpp_exceptions) && !defined(__EXCEPTIONS) && !defined(_CPPUNWIND)
namespace crow {
extern const char* last_error;
}
#endif