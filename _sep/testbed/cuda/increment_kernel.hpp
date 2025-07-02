#pragma once
#include <cuda_runtime.h>
extern "C" cudaError_t launch_increment_kernel(int* d_data, unsigned int n, cudaStream_t stream);
