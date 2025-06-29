#pragma once

#include "core/common.h"
#include "blender/config.h"
#include <memory>

// Forward declaration for MemoryTierEnum from sep::math_common.h
namespace sep {
  enum class MemoryTierEnum : int;
  namespace pattern {
    class BlenderBridge;
  }
}

// Using the SEPResult enum from sep namespace
// Convert SEPResult to string
inline const char* sep_result_to_string(sep::SEPResult result) {
  switch (result) {
    case sep::SEPResult::SUCCESS:
      return "Success";
    case sep::SEPResult::INVALID_ARGUMENT:
      return "Invalid argument";
    case sep::SEPResult::INVALID_STATE:
      return "Invalid state";
    case sep::SEPResult::INITIALIZATION_FAILED:
      return "Not initialized";
    case sep::SEPResult::ALREADY_EXISTS:
      return "Already initialized";
    case sep::SEPResult::UNKNOWN_ERROR:
      return "Unknown error";
    case sep::SEPResult::MEMORY_ERROR:
      return "Memory error";
    case sep::SEPResult::OUT_OF_MEMORY:
      return "Allocation failed";
    case sep::SEPResult::ALLOCATION_FAILED:
      return "Allocation failed";
    case sep::SEPResult::PROCESSING_ERROR:
      return "Processing error";
    case sep::SEPResult::NOT_FOUND:
      return "Object not found";
    case sep::SEPResult::PATTERN_MATCH_FAILED:
      return "Pattern error";
    case sep::SEPResult::CUDA_ERROR:
      return "CUDA initialization failed";
    case sep::SEPResult::FILE_NOT_FOUND:
      return "File not found";
    default:
      return "UNKNOWN_SEP_RESULT";
  }
}

// Bridge structure used by the C API
struct SEPBlenderBridge {
    std::shared_ptr<sep::pattern::BlenderBridge> impl;
    SEPAudioMetrics                              audio_metrics{};
    SEPPatternMetrics                            pattern_metrics{};
};

#ifdef __cplusplus
namespace sep {
using SEPBlenderBridge = ::SEPBlenderBridge;
}  // namespace sep
#endif
