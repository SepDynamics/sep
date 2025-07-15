#include <cuda_runtime.h>
#include "qbsa.cuh"

namespace sep::quantum {

__global__ void qbsa_kernel(QBSAParams params) {
    const uint32_t tid = blockIdx.x * blockDim.x + threadIdx.x;
    if (tid < params.num_probes) {
        if (params.probe_indices[tid] != params.expectations[tid]) {
            atomicAdd(&params.corrections[0], 1);
        }
    }
}

bool launch_qbsa_kernel(const QBSAParams& params) {
    const dim3 block(256);
    const dim3 grid((params.num_probes + block.x - 1) / block.x);
    
    qbsa_kernel<<<grid, block>>>(params);
    
    return cudaGetLastError() == cudaSuccess &&
           cudaDeviceSynchronize() == cudaSuccess;
}

                           const uint32_t* expectations,
                           uint32_t* corrections,
                           const uint32_t num_probes) {
    const uint32_t tid = blockIdx.x * blockDim.x + threadIdx.x;
    if (tid >= num_probes) return;

    if (probe_indices[tid] != expectations[tid]) {
        // Use atomic operation to add correction index
        const uint32_t idx = atomicAdd(&corrections[0], 1);  // Increment count and get position
        corrections[idx + 1] = tid;  // Store correction index
    }
}

// Helper for CUDA error checking using toolkit's error system
inline void checkCuda(cudaError_t result) {
    if (result != cudaSuccess) {
        ::sep::cuda::logCudaError("QBSA CUDA kernel", result);
    }
}

} // namespace sep::quantum