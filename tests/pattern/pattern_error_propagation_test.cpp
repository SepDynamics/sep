
#include "compat/cuda_runtime.h"
#include <gtest/gtest.h>

// Override the kernel launcher to simulate failure
namespace sep {
namespace cuda {
cudaError_t launch_pattern_processing(sep::pattern::GPUPatternData*,
                                      sep::pattern::GPUPatternData*,
                                      const sep::pattern::PatternConfig&,
                                      size_t,
                                      const sep::pattern::GPUPatternData*,
                                      cudaStream_t) {
    return cudaErrorUnknown;
}
} // namespace cuda
} // namespace sep

using namespace sep::pattern;

TEST(PatternEvolutionError, KernelFailurePropagates) {
    PatternData p{};
    std::vector<PatternData> input{p};
    PatternConfig cfg{0.1f, false, 1, 1};
    std::vector<PatternData> output;
    EXPECT_EQ(PatternEvolution::processPatterns(input, cfg, output),
              SEPResult::PROCESSING_ERROR);
}
