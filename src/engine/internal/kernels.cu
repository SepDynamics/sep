#include <cuda_runtime.h>

#include "kernels.h"
#include "quantum/bitspace/qfh.h"

// Forward declarations for kernels implemented in quantum_kernels.cu
__global__ void qbsa_kernel(const std::uint32_t* d_probe_indices,
                           const std::uint32_t* d_expectations,
                           std::uint32_t num_probes,
                           std::uint32_t* d_bitfield,
                           std::uint32_t* d_corrections,
                           std::uint32_t* d_correction_count);

__global__ void qsh_kernel(const std::uint64_t* d_chunks,
                          std::uint32_t num_chunks,
                          std::uint32_t* d_collapse_indices,
                          std::uint32_t* d_collapse_counts);

cudaError_t launchQBSAKernel(const std::uint32_t *d_probe_indices,
                           const std::uint32_t *d_expectations, std::uint32_t num_probes,
                           std::uint32_t *d_bitfield, std::uint32_t *d_corrections,
                           std::uint32_t *d_correction_count, cudaStream_t stream) {
    const std::uint32_t block_size = 256;
    const std::uint32_t grid_size = (num_probes + block_size - 1) / block_size;
    qbsa_kernel<<<grid_size, block_size, 0, stream>>>(d_probe_indices, d_expectations, num_probes, d_bitfield, d_corrections, d_correction_count);
    return cudaGetLastError();
}

cudaError_t launchQSHKernel(const std::uint64_t *d_chunks,
                            std::uint32_t num_chunks,
                            std::uint32_t *d_collapse_indices,
                            std::uint32_t *d_collapse_counts,
                            cudaStream_t stream) {
    const std::uint32_t block_size = 256;
    const std::uint32_t grid_size = (num_chunks + block_size - 1) / block_size;
    qsh_kernel<<<grid_size, block_size, 0, stream>>>(d_chunks, num_chunks, d_collapse_indices, d_collapse_counts);
    return cudaGetLastError();
}

__global__ void qfhKernel(const uint8_t* d_bit_packages, int num_packages, int package_size, sep::quantum::bitspace::ForwardWindowResult* d_results) {
    int idx = blockIdx.x;
    if (idx >= num_packages) {
        return;
    }

    const uint8_t* current_package = &d_bit_packages[idx * package_size];
    double accumulated_value = 0.0;
    double lambda = 0.1; // Decay constant

    for (int i = 1; i < package_size; ++i) {
        double future_bit = current_package[i];
        double current_bit = current_package[0];
        accumulated_value += (future_bit - current_bit) * exp(-lambda * i);
    }

    d_results[idx].damped_coherence = 1.0 - accumulated_value; // Example
    d_results[idx].damped_stability = accumulated_value; // Example
}

cudaError_t launchQFHBitTransitionsKernel(const uint8_t* d_bit_packages, int num_packages, int package_size, sep::quantum::bitspace::ForwardWindowResult* d_results, cudaStream_t stream) {
    const int block_size = 256;
    const int grid_size = (num_packages + block_size - 1) / block_size;
    qfhKernel<<<grid_size, block_size, 0, stream>>>(d_bit_packages, num_packages, package_size, d_results);
    return cudaGetLastError();
}
