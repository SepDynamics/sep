#pragma once
#include "compat/shim.h"

namespace sep { namespace crow { namespace error {

enum class Code {
    None = 0,
    InvalidMethod,
    InvalidJson,
    InvalidParameter,
    OutOfRange,
    RuntimeError,
    InvalidContainer,
    InvalidList,
    IndexOutOfBounds,
    NotObject,
    KeyNotFound,
    InvalidNumberState
};

void log(Code code, const sep::shim::string& message);

} } } // namespace sep::crow::error
