#ifndef SEP_COMPAT_CUFFT_H
#define SEP_COMPAT_CUFFT_H

// Include standard headers needed for types
#include <stddef.h>  // For size_t

// Include cuda_runtime.h first to ensure CUDA types are defined
#include "compat/cuda_runtime.h"

#if SEP_ENGINE_HAS_CUDA
#  if defined(__has_include) && __has_include(<cufft.h>)
#    include <cufft.h>
#    define SEP_HAS_CUFFT 1
#  else
#    define SEP_HAS_CUFFT 0
#  endif
#else
#  define SEP_HAS_CUFFT 0
#endif

#if SEP_HAS_CUFFT
// When real CUDA/cuFFT is available, use those types
namespace sep {
namespace cuda {
using ::cufftResult;
using ::cufftHandle;
using ::cufftType;
using ::cufftReal;
using ::cufftDoubleReal;
using ::cufftComplex;
using ::cufftDoubleComplex;
} // namespace cuda
} // namespace sep

#else
namespace sep {
namespace cuda {

// CUFFT Types
typedef int cufftResult;
struct cufftHandle_t;
typedef cufftHandle_t* cufftHandle;
typedef int cufftType;
typedef float cufftReal;
typedef double cufftDoubleReal;
struct cufftComplex { float x, y; };
struct cufftDoubleComplex { double x, y; };

// CUFFT Transform types
constexpr cufftType CUFFT_R2C = 0x2a;
constexpr cufftType CUFFT_C2R = 0x2c;
constexpr cufftType CUFFT_C2C = 0x29;

// CUFFT Result codes
constexpr cufftResult CUFFT_SUCCESS = 0;
constexpr cufftResult CUFFT_INVALID_PLAN = 1;
constexpr cufftResult CUFFT_ALLOC_FAILED = 2;
constexpr cufftResult CUFFT_INVALID_TYPE = 3;
constexpr cufftResult CUFFT_INVALID_VALUE = 4;
constexpr cufftResult CUFFT_INTERNAL_ERROR = 5;
constexpr cufftResult CUFFT_EXEC_FAILED = 6;
constexpr cufftResult CUFFT_SETUP_FAILED = 7;
constexpr cufftResult CUFFT_INVALID_SIZE = 8;
constexpr cufftResult CUFFT_UNALIGNED_DATA = 9;

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

#ifdef __cplusplus
// Make types available in global scope
using sep::cuda::cufftResult;
using sep::cuda::cufftHandle;
using sep::cuda::cufftType;
using sep::cuda::cufftReal;
using sep::cuda::cufftDoubleReal;
using sep::cuda::cufftComplex;
using sep::cuda::cufftDoubleComplex;

// Make constants available in global scope
using sep::cuda::CUFFT_R2C;
using sep::cuda::CUFFT_C2R;
using sep::cuda::CUFFT_C2C;
using sep::cuda::CUFFT_SUCCESS;
using sep::cuda::CUFFT_INVALID_PLAN;
using sep::cuda::CUFFT_ALLOC_FAILED;
using sep::cuda::CUFFT_INVALID_TYPE;
using sep::cuda::CUFFT_INVALID_VALUE;
using sep::cuda::CUFFT_INTERNAL_ERROR;
using sep::cuda::CUFFT_EXEC_FAILED;
using sep::cuda::CUFFT_SETUP_FAILED;
using sep::cuda::CUFFT_INVALID_SIZE;
using sep::cuda::CUFFT_UNALIGNED_DATA;

// Define function implementations in global scope
inline cufftResult cufftPlan1d(cufftHandle* plan, int nx, int type, int batch) {
    return sep::cuda::cufftPlan1d(plan, nx, type, batch);
}

inline cufftResult cufftPlan2d(cufftHandle* plan, int nx, int ny, int type) {
    return sep::cuda::cufftPlan2d(plan, nx, ny, type);
}

inline cufftResult cufftPlan3d(cufftHandle* plan, int nx, int ny, int nz, int type) {
    return sep::cuda::cufftPlan3d(plan, nx, ny, nz, type);
}

inline cufftResult cufftDestroy(cufftHandle plan) {
    return sep::cuda::cufftDestroy(plan);
}

inline cufftResult cufftExecC2C(cufftHandle plan, void* idata, void* odata, int direction) {
    return sep::cuda::cufftExecC2C(plan, idata, odata, direction);
}

inline cufftResult cufftExecR2C(cufftHandle plan, void* idata, void* odata) {
    return sep::cuda::cufftExecR2C(plan, idata, odata);
}

inline cufftResult cufftExecC2R(cufftHandle plan, void* idata, void* odata) {
    return sep::cuda::cufftExecC2R(plan, idata, odata);
}
#endif // __cplusplus

#endif // SEP_HAS_CUFFT

#endif // SEP_COMPAT_CUFFT_H