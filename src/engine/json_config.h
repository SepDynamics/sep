#pragma once

// Disable nlohmann::json's internal assertions to avoid conflicts.
#define JSON_DISABLE_ASSERT 1

// GLM compiler compatibility for CUDA
#define GLM_COMPILER 0

// Force GLM to accept CUDA version
#ifdef __CUDACC__
#define CUDA_VERSION 11080
#define __CUDA_VER_MAJOR__ 11
#define __CUDA_VER_MINOR__ 8
#endif