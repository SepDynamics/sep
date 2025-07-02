/**
 * @file mock_process.cpp
 * @brief Mock implementation of pattern processing functions for testing
 */

#include "compat/cuda_common.h"


#include <iostream>

// Define the actual implementation of the function that will be used
// This completely replaces the implementation in src/pattern/process.cpp
namespace sep {
namespace cuda {

// Host-side implementations
cudaError_t launch_pattern_processing(PatternData* patterns,
                                      PatternData* results,
                                      const PatternConfig& config,
                                      size_t pattern_count,
                                      const PatternData* previous_patterns,
                                      cudaStream_t stream) {
  std::cout << "Mock launch_pattern_processing called without TraceSpan" << std::endl;
  
  // Mock implementation - doesn't use TraceSpan
  (void)patterns;
  (void)results;
  (void)config;
  (void)pattern_count;
  (void)previous_patterns;
  (void)stream;
  return cudaSuccess;
}

}  // namespace cuda
}  // namespace sep

// Also provide the mangled name version for extra safety
extern "C" {
    // This is the mangled name for the launch_pattern_processing function
    cudaError_t _ZN3sep4cuda25launch_pattern_processingEPNS_7pattern11PatternDataES3_RKNS1_13PatternConfigEmPKS2_P11CUstream_st(
        sep::pattern::PatternData* patterns,
        sep::pattern::PatternData* results,
        const sep::pattern::PatternConfig& config,
        size_t pattern_count,
        const sep::pattern::PatternData* previous_patterns,
        cudaStream_t stream) {

        std::cout << "Mock launch_pattern_processing called via mangled name" << std::endl;

        // Just call our implementation above
        return sep::cuda::launch_pattern_processing(
            patterns, results, config, pattern_count, previous_patterns, stream);
    }
}