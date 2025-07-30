#include <cuda_runtime.h>

#include <cstdio>

#include "quantum/qbsa.cuh"

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

// Helper for CUDA error checking
inline void checkCuda(cudaError_t result) {
    if (result != cudaSuccess) {
        // Simple error logging - could be enhanced later
        printf("CUDA error in QBSA kernel: %s\n", cudaGetErrorString(result));
    }
}

} // namespace sep::quantum