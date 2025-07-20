#pragma once

#include <cuda_runtime.h>
#include <cstdint>

namespace sep::quantum {

struct QBSAParams {
    const uint32_t* probe_indices;
    const uint32_t* expectations;
    uint32_t* corrections;
    uint32_t num_probes;
    uint32_t max_corrections;
    float* correction_ratio;
};

// CUDA kernel declarations
__global__ void qbsa_kernel(QBSAParams params);

} // namespace sep::quantum
