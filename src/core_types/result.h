#pragma once

// The One True Normal Reference Frame for Result Types
// This is the canonical source of truth for all result and error handling

namespace sep::core {

/// Standard result type for all SEP operations
enum class Result {
    SUCCESS = 0,
    FAILURE = 1,
    INVALID_ARGUMENT = 2,
    OUT_OF_MEMORY = 3,
    CUDA_ERROR = 4,
    FILE_NOT_FOUND = 5,
    NETWORK_ERROR = 6,
    TIMEOUT = 7,
    NOT_INITIALIZED = 8,
    ALREADY_EXISTS = 9,
    NOT_FOUND = 10,
    NOT_IMPLEMENTED = 11,
    PROCESSING_ERROR = 12,
    RUNTIME_ERROR = 13,
    UNKNOWN_ERROR = 14
};

/// Convert result to human-readable string
const char* resultToString(Result result);

/// Check if result indicates success
inline bool isSuccess(Result result) {
    return result == Result::SUCCESS;
}

/// Check if result indicates failure
inline bool isFailure(Result result) {
    return result != Result::SUCCESS;
}

} // namespace sep::core

// Backward compatibility alias
namespace sep {
    using SEPResult = core::Result;
}
