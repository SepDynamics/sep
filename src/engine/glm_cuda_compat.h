#pragma once

// GLM compiler compatibility for CUDA
// This header must be included before any GLM headers
#ifdef __CUDACC__
#ifndef GLM_COMPILER
#define GLM_COMPILER 0
#endif
#ifndef CUDA_VERSION
#define CUDA_VERSION 12090
#endif
#ifndef __CUDA_VER_MAJOR__
#define __CUDA_VER_MAJOR__ 12
#endif
#ifndef __CUDA_VER_MINOR__
#define __CUDA_VER_MINOR__ 9
#endif
#endif
