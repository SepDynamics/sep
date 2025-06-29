#pragma once

#include <string>
#include <cstdint>

namespace sep {

// Common result type for SEP operations
enum class SEPResult : int32_t {
    SUCCESS = 0,
    UNKNOWN_ERROR = -1,
    INVALID_ARGUMENT = -2,
    OUT_OF_MEMORY = -3,
    FILE_NOT_FOUND = -4,
    PERMISSION_DENIED = -5,
    NETWORK_ERROR = -6,
    TIMEOUT = -7,
    NOT_IMPLEMENTED = -8,
    ALREADY_EXISTS = -9,
    NOT_FOUND = -10,
    INVALID_STATE = -11,
    BUFFER_TOO_SMALL = -12,
    OPERATION_CANCELLED = -13,
    RESOURCE_BUSY = -14,
    INITIALIZATION_FAILED = -15,
    COMPILATION_FAILED = -16,
    PATTERN_MATCH_FAILED = -17,
    GPU_ERROR = -18,
    CUDA_ERROR = -19,
    MEMORY_ERROR = -20,
    PROCESSING_ERROR = -21,
    ALLOCATION_FAILED = -22,
    FEATURE_UNAVAILABLE = -23,
    NOT_INITIALIZED = -24
};

// Basic status enumeration for engine components
enum class Status { Success = 0, Error = 1 };

// Simple pin state structure used by the engine algorithms
#ifndef SEP_PINSTATE_DEFINED
#define SEP_PINSTATE_DEFINED
struct PinState {
    std::uint64_t state{0};
    std::uint32_t flags{0};
    bool operator==(const PinState& other) const noexcept {
        return state == other.state && flags == other.flags;
    }
};
#endif // SEP_PINSTATE_DEFINED

// Convert SEPResult to string
inline const char* to_string(SEPResult result) {
    switch (result) {
        case SEPResult::SUCCESS: return "SUCCESS";
        case SEPResult::UNKNOWN_ERROR: return "UNKNOWN_ERROR";
        case SEPResult::INVALID_ARGUMENT: return "INVALID_ARGUMENT";
        case SEPResult::OUT_OF_MEMORY: return "OUT_OF_MEMORY";
        case SEPResult::FILE_NOT_FOUND: return "FILE_NOT_FOUND";
        case SEPResult::PERMISSION_DENIED: return "PERMISSION_DENIED";
        case SEPResult::NETWORK_ERROR: return "NETWORK_ERROR";
        case SEPResult::PROCESSING_ERROR: return "PROCESSING_ERROR";
        case SEPResult::TIMEOUT: return "TIMEOUT";
        case SEPResult::NOT_IMPLEMENTED: return "NOT_IMPLEMENTED";
        case SEPResult::ALREADY_EXISTS: return "ALREADY_EXISTS";
        case SEPResult::NOT_FOUND: return "NOT_FOUND";
        case SEPResult::INVALID_STATE: return "INVALID_STATE";
        case SEPResult::BUFFER_TOO_SMALL: return "BUFFER_TOO_SMALL";
        case SEPResult::OPERATION_CANCELLED: return "OPERATION_CANCELLED";
        case SEPResult::RESOURCE_BUSY: return "RESOURCE_BUSY";
        case SEPResult::INITIALIZATION_FAILED: return "INITIALIZATION_FAILED";
        case SEPResult::COMPILATION_FAILED: return "COMPILATION_FAILED";
        case SEPResult::PATTERN_MATCH_FAILED: return "PATTERN_MATCH_FAILED";
        case SEPResult::GPU_ERROR: return "GPU_ERROR";
        case SEPResult::CUDA_ERROR: return "CUDA_ERROR";
        case SEPResult::MEMORY_ERROR: return "MEMORY_ERROR";
        case SEPResult::ALLOCATION_FAILED: return "ALLOCATION_FAILED";
        default: return "UNKNOWN";
    }
}

// Check if result indicates success
inline bool is_success(SEPResult result) {
    return result == SEPResult::SUCCESS;
}

// Check if result indicates error
inline bool is_error(SEPResult result) {
    return result != SEPResult::SUCCESS;
}

// Common macros for error handling
#define SEP_RETURN_IF_ERROR(expr) \
    do { \
        SEPResult _result = (expr); \
        if (is_error(_result)) return _result; \
    } while(0)

#define SEP_CHECK_SUCCESS(expr) \
    do { \
        SEPResult _result = (expr); \
        if (is_error(_result)) { \
            throw std::runtime_error("SEP operation failed: " + std::string(to_string(_result))); \
        } \
    } while(0)

} // namespace sep