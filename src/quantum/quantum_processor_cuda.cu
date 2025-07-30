// Include compatibility header first to handle math function conflicts
#include <cuda_runtime.h>

// GLM CUDA compatibility
#define GLM_COMPILER 0
#define CUDA_VERSION 12090
#define __CUDA_VER_MAJOR__ 12
#define __CUDA_VER_MINOR__ 9

#include <cmath>
#include <glm/glm.hpp>

#include "engine/internal/cuda_math_compat.h"
#include "quantum/quantum_processor_cuda.h"

namespace sep::quantum {

// A simple CUDA kernel to calculate coherence (dot product)
__global__ void coherenceKernel(const float* a, const float* b, float* result) {
    int i = threadIdx.x;
    result[i] = a[i] * b[i];
}

QuantumProcessorCUDA::QuantumProcessorCUDA(const Config& config) : QuantumProcessor(config) {
    // Constructor implementation
}

QuantumProcessorCUDA::~QuantumProcessorCUDA() {
    // Destructor implementation
}

float QuantumProcessorCUDA::calculateCoherence(const glm::vec3& a, const glm::vec3& b) {
    // This is a simplified example. A real implementation would handle batches of vectors.
    float result = 0.0f;
    
    // Convert glm::vec3 to float arrays
    float h_a[3] = {a.x, a.y, a.z};
    float h_b[3] = {b.x, b.y, b.z};
    
    // Allocate memory on the device
    float* d_a;
    float* d_b;
    float* d_result;
    cudaMalloc((void**)&d_a, sizeof(float) * 3);
    cudaMalloc((void**)&d_b, sizeof(float) * 3);
    cudaMalloc((void**)&d_result, sizeof(float) * 3);

    // Copy data from host to device
    cudaMemcpy(d_a, h_a, sizeof(float) * 3, cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, h_b, sizeof(float) * 3, cudaMemcpyHostToDevice);

    // Launch the kernel
    coherenceKernel<<<1, 3>>>(d_a, d_b, d_result);

    // Copy result back from device to host
    float h_result[3];
    cudaMemcpy(h_result, d_result, sizeof(float) * 3, cudaMemcpyDeviceToHost);

    // Sum the partial results and normalize
    float dot_product = 0.0f;
    for (int i = 0; i < 3; ++i) {
        dot_product += h_result[i];
    }
    
    // Calculate magnitudes for normalization
    float mag_a = glm::length(a);
    float mag_b = glm::length(b);
    
    // Avoid division by zero
    if (mag_a < 1e-6f || mag_b < 1e-6f) {
        result = 0.0f;
    } else {
        result = glm::clamp(dot_product / (mag_a * mag_b), 0.0f, 1.0f);
    }

    // Free device memory
    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_result);

    return result;
}

} // namespace sep::quantum