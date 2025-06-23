#ifndef SEP_CORE_ERROR_H
#define SEP_CORE_ERROR_H

#include <string>
#include "api/types.h"

namespace sep {

struct Error {
    api::ErrorCode code{api::ErrorCode::Success};
    std::string message;
};

} // namespace sep

#endif // SEP_CORE_ERROR_H