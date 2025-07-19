#pragma once
#pragma message("cuda_math_compat.h is being included!")

// Temporarily undefine problematic math functions
#ifdef cospi
#undef cospi
#endif

#ifdef sinpi
#undef sinpi
#endif

#ifdef cospif
#undef cospif
#endif

#ifdef sinpif
#undef sinpif
#endif

// Include CUDA headers first
#include <cuda_runtime.h>
#include <cuda.h>

// Then include system math
#include <math.h>

// Define our own versions that don't conflict
#ifndef CUDA_MATH_COMPAT_DEFINED
#define CUDA_MATH_COMPAT_DEFINED
inline double sep_cospi(double x) { return cospi(x); }
inline double sep_sinpi(double x) { return sinpi(x); }
inline float sep_cospif(float x) { return cospif(x); }
inline float sep_sinpif(float x) { return sinpif(x); }
#endif