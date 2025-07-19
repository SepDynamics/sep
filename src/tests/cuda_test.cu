#include <cuda_runtime.h>
#include <iostream>
#include <cstdio>

__global__ void testKernel(int* d_data) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    d_data[idx] = idx * idx;
}

int main() {
    // Check CUDA device
    int deviceCount = 0;
    cudaError_t error = cudaGetDeviceCount(&deviceCount);
    
    if (error != cudaSuccess) {
        std::cerr << "CUDA Error: " << cudaGetErrorString(error) << std::endl;
        return 1;
    }
    
    if (deviceCount == 0) {
        std::cerr << "No CUDA devices found!" << std::endl;
        return 1;
    }
    
    std::cout << "Found " << deviceCount << " CUDA device(s)" << std::endl;
    
    // Get device properties
    cudaDeviceProp prop;
    cudaGetDeviceProperties(&prop, 0);
    std::cout << "Device 0: " << prop.name << std::endl;
    std::cout << "Compute capability: " << prop.major << "." << prop.minor << std::endl;
    std::cout << "Total global memory: " << prop.totalGlobalMem / (1024*1024) << " MB" << std::endl;
    
    // Test kernel execution
    const int N = 256;
    int* h_data = new int[N];
    int* d_data;
    
    cudaMalloc(&d_data, N * sizeof(int));
    
    // Launch kernel
    testKernel<<<1, N>>>(d_data);
    
    // Copy back and verify
    cudaMemcpy(h_data, d_data, N * sizeof(int), cudaMemcpyDeviceToHost);
    
    bool success = true;
    for (int i = 0; i < N; i++) {
        if (h_data[i] != i * i) {
            success = false;
            break;
        }
    }
    
    if (success) {
        std::cout << "CUDA kernel test PASSED!" << std::endl;
    } else {
        std::cout << "CUDA kernel test FAILED!" << std::endl;
    }
    
    // Cleanup
    cudaFree(d_data);
    delete[] h_data;
    
    return success ? 0 : 1;
}
