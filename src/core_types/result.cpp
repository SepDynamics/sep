#include "result.h"

namespace sep::core {

const char* resultToString(Result result) {
    switch (result) {
        case Result::SUCCESS:
            return "SUCCESS";
        case Result::FAILURE:
            return "FAILURE";
        case Result::INVALID_ARGUMENT:
            return "INVALID_ARGUMENT";
        case Result::OUT_OF_MEMORY:
            return "OUT_OF_MEMORY";
        case Result::CUDA_ERROR:
            return "CUDA_ERROR";
        case Result::FILE_NOT_FOUND:
            return "FILE_NOT_FOUND";
        case Result::NETWORK_ERROR:
            return "NETWORK_ERROR";
        case Result::TIMEOUT:
            return "TIMEOUT";
        case Result::NOT_INITIALIZED:
            return "NOT_INITIALIZED";
        case Result::ALREADY_EXISTS:
            return "ALREADY_EXISTS";
        case Result::NOT_FOUND:
            return "NOT_FOUND";
        case Result::NOT_IMPLEMENTED:
            return "NOT_IMPLEMENTED";
        case Result::PROCESSING_ERROR:
            return "PROCESSING_ERROR";
        case Result::RUNTIME_ERROR:
            return "RUNTIME_ERROR";
        case Result::UNKNOWN_ERROR:
            return "UNKNOWN_ERROR";
        default:
            return "UNKNOWN";
    }
}

} // namespace sep::core
