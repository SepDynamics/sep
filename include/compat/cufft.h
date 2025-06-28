#ifndef SEP_COMPAT_CUFFT_H
#define SEP_COMPAT_CUFFT_H

// Include standard headers needed for types
#include <stddef.h>  // For size_t

// Define SEP_CUDA_AVAILABLE if not already defined
#ifndef SEP_CUDA_AVAILABLE
#define SEP_CUDA_AVAILABLE 0
#endif

#if SEP_CUDA_AVAILABLE
// When CUDA is available, include the real CUDA FFT headers
#include <cufft.h>
#else
// When CUDA is not available, define stub types and functions

#ifdef __cplusplus
namespace sep {
namespace cuda {

// CUFFT Types
typedef int cufftResult;
typedef int cufftHandle;
typedef int cufftType;

// CUFFT Transform types
#define CUFFT_R2C 0x2a
#define CUFFT_C2R 0x2c
#define CUFFT_C2C 0x29

// CUFFT Result codes
static const cufftResult CUFFT_SUCCESS = 0;
static const cufftResult CUFFT_INVALID_PLAN = 1;
static const cufftResult CUFFT_ALLOC_FAILED = 2;
static const cufftResult CUFFT_INVALID_TYPE = 3;
static const cufftResult CUFFT_INVALID_VALUE = 4;
static const cufftResult CUFFT_INTERNAL_ERROR = 5;
static const cufftResult CUFFT_EXEC_FAILED = 6;
static const cufftResult CUFFT_SETUP_FAILED = 7;
static const cufftResult CUFFT_INVALID_SIZE = 8;
static const cufftResult CUFFT_UNALIGNED_DATA = 9;

// Function declarations
cufftResult cufftPlan1d(cufftHandle* plan, int nx, int type, int batch);
cufftResult cufftPlan2d(cufftHandle* plan, int nx, int ny, int type);
cufftResult cufftPlan3d(cufftHandle* plan, int nx, int ny, int nz, int type);
cufftResult cufftDestroy(cufftHandle plan);
cufftResult cufftExecC2C(cufftHandle plan, void* idata, void* odata, int direction);
cufftResult cufftExecR2C(cufftHandle plan, void* idata, void* odata);
cufftResult cufftExecC2R(cufftHandle plan, void* idata, void* odata);

} // namespace cuda
} // namespace sep

// Make the sep::cuda types available in global namespace for compatibility
using sep::cuda::cufftResult;
using sep::cuda::cufftHandle;
using sep::cuda::cufftType;

// Define the function prototypes in global namespace
inline cufftResult cufftPlan1d(cufftHandle* plan, int nx, int type, int batch) {
    return sep::cuda::CUFFT_SUCCESS;
}

inline cufftResult cufftPlan2d(cufftHandle* plan, int nx, int ny, int type) {
    return sep::cuda::CUFFT_SUCCESS;
}

inline cufftResult cufftPlan3d(cufftHandle* plan, int nx, int ny, int nz, int type) {
    return sep::cuda::CUFFT_SUCCESS;
}

inline cufftResult cufftDestroy(cufftHandle plan) {
    return sep::cuda::CUFFT_SUCCESS;
}

inline cufftResult cufftExecC2C(cufftHandle plan, void* idata, void* odata, int direction) {
    return sep::cuda::CUFFT_SUCCESS;
}

inline cufftResult cufftExecR2C(cufftHandle plan, void* idata, void* odata) {
    return sep::cuda::CUFFT_SUCCESS;
}

inline cufftResult cufftExecC2R(cufftHandle plan, void* idata, void* odata) {
    return sep::cuda::CUFFT_SUCCESS;
}

#endif // __cplusplus
#endif // !SEP_CUDA_AVAILABLE

#endif // SEP_COMPAT_CUFFT_H