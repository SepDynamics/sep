#include <cuda_runtime.h>
extern "C" __global__ void increment_kernel(int* data, unsigned int n) {
    unsigned int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx < n) {
        data[idx] += 1;
    }
}

extern "C" cudaError_t launch_increment_kernel(int* d_data, unsigned int n, cudaStream_t stream) {
    if (!d_data) return cudaErrorInvalidValue;
    const unsigned int block = 64;
    const unsigned int grid = (n + block - 1) / block;
    increment_kernel<<<grid, block, 0, stream>>>(d_data, n);
    return cudaGetLastError();
}
