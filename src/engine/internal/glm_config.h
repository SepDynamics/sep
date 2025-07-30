#ifndef SEP_GLM_CONFIG_H
#define SEP_GLM_CONFIG_H
#pragma once

// GLM configuration - must come before any GLM includes
#ifndef GLM_FORCE_CUDA
#define GLM_FORCE_CUDA
#endif

// Define CUDA version before GLM checks it
#ifdef __CUDACC__
#ifndef CUDA_VERSION
#define CUDA_VERSION 12090  // CUDA 12.9
#endif
// Define GLM compiler for CUDA compatibility
#ifndef GLM_COMPILER
#define GLM_COMPILER 0  // Ignore compiler checks as suggested by GLM error
#endif
#endif

#ifndef GLM_CUDA_VERSION_CHECK
#define GLM_CUDA_VERSION_CHECK CUDA_VERSION
#endif


#ifndef GLM_FORCE_CXX17
#define GLM_FORCE_CXX17
#endif

#ifndef GLM_FORCE_SILENT_WARNINGS
#define GLM_FORCE_SILENT_WARNINGS
#endif

#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif

#endif // SEP_GLM_CONFIG_H