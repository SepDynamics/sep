// *** CRITICAL: COMPATIBILITY SETTINGS FOR GCC 14 AND CUDA 12.9 ***
// These must be defined before any CUDA headers are included
#ifndef SEP_CUDACC_DISABLE_EXCEPTION_SPEC_CHECKS
#define SEP_CUDACC_DISABLE_EXCEPTION_SPEC_CHECKS 1
#endif

// Disable problematic fp8 and half precision conversions
#ifndef __CUDA_NO_FP8_CONVERSIONS__
#define __CUDA_NO_FP8_CONVERSIONS__ 1
#endif

#ifndef __CUDA_NO_HALF_OPERATORS__
#define __CUDA_NO_HALF_OPERATORS__ 1
#endif

#ifndef __CUDA_NO_HALF2_OPERATORS__
#define __CUDA_NO_HALF2_OPERATORS__ 1
#endif

#ifndef __CUDA_NO_BFLOAT16_CONVERSIONS__
#define __CUDA_NO_BFLOAT16_CONVERSIONS__ 1 
#endif

#pragma once

#ifndef CUDA_UNIFIED_FIX_H
#define CUDA_UNIFIED_FIX_H

// Feature toggles must be defined early so later sections can use them.
#ifndef CUDA_UNIFIED_FIX_ENABLE_MATH_STUBS
#define CUDA_UNIFIED_FIX_ENABLE_MATH_STUBS 1
#endif

#ifndef CUDA_UNIFIED_FIX_ENABLE_FUNCTION_RENAMING
#define CUDA_UNIFIED_FIX_ENABLE_FUNCTION_RENAMING 1
#endif

// *** CRITICAL: DISABLE EXCEPTION SPECIFICATION CHECKS ***
// This must be defined before any CUDA headers are included
#ifndef SEP_CUDACC_DISABLE_EXCEPTION_SPEC_CHECKS
#define SEP_CUDACC_DISABLE_EXCEPTION_SPEC_CHECKS 1
#endif

// Macro scope guards
#define CUDA_UNIFIED_FIX_PUSH_MACROS()                                                                  \
    _Pragma("push_macro(\"sinpi\")") _Pragma("push_macro(\"cospi\")") _Pragma("push_macro(\"sinpif\")") \
        _Pragma("push_macro(\"cospif\")")

#define CUDA_UNIFIED_FIX_POP_MACROS()                                                                 \
    _Pragma("pop_macro(\"cospif\")") _Pragma("pop_macro(\"sinpif\")") _Pragma("pop_macro(\"cospi\")") \
        _Pragma("pop_macro(\"sinpi\")")

#define CUDA_UNIFIED_FIX_BEGIN_SCOPE() CUDA_UNIFIED_FIX_PUSH_MACROS()

#define CUDA_UNIFIED_FIX_END_SCOPE() CUDA_UNIFIED_FIX_POP_MACROS()

#if defined(CUDA_UNIFIED_FIX_ENABLE_FUNCTION_RENAMING) && defined(__CUDACC__)
#define sinpi __cuda_sinpi
#define cospi __cuda_cospi
#define sinpif __cuda_sinpif
#define cospif __cuda_cospif
#define sincospi __cuda_sincospi
#define sincospif __cuda_sincospif
#endif

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#include <cerrno>
#pragma GCC diagnostic pop

CUDA_UNIFIED_FIX_BEGIN_SCOPE()
#if !defined(__CUDACC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#include <cmath>
#pragma GCC diagnostic pop
#endif

// Map standard long double functions to SEP implementations
#define acosl sep_acosl
#define asinl sep_asinl
#define atanl sep_atanl
#define atan2l sep_atan2l
#define sqrtl sep_sqrtl
#define logl sep_logl
CUDA_UNIFIED_FIX_END_SCOPE()

// Indicate whether CUDA support is available for this compilation unit.
#if defined(__CUDACC__) || defined(SEP_USE_CUDA)
#define cuda_available 1
#else
#define cuda_available 0
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Define CUDA device/host function qualifiers
#if (__CUDACC__)
#define SEP_HOST __host__
#define SEP_DEVICE __device__
#define SEP_HD __host__ __device__
#else
#define SEP_HOST
#define SEP_DEVICE
#define SEP_HD
#endif

// *** CRITICAL: DISABLE EXCEPTION SPECIFICATION CHECKS ***
// This must be defined before any CUDA headers are included
#ifndef SEP_CUDACC_DISABLE_EXCEPTION_SPEC_CHECKS
#define SEP_CUDACC_DISABLE_EXCEPTION_SPEC_CHECKS 1
#endif

// Handle CUDA keyword conflicts first
#ifdef __CUDACC__
#define _GLIBCXX_INCLUDE_NEXT_C_HEADERS
#endif

// Include math compatibility headers
#ifdef __has_include
#if __has_include("compat/math_common.h")
#include "compat/math_common.h"
#define SEP_MATH_COMMON_AVAILABLE 1
#else
// Fallback for older compilers

#define SEP_MATH_COMMON_AVAILABLE 0
#endif
#else
// Very old compiler fallback

#define SEP_MATH_COMMON_AVAILABLE 0
#endif

#ifdef __CUDACC__
#endif

// Ensure c++20 ABI compatibility
#ifndef SEP_LIBCXX_USE_CXX20_ABI
#define SEP_LIBCXX_USE_CXX20_ABI 1
#endif

// Floating point classification constants (IEEE 754 standard)
namespace cuda_fix_detail {
constexpr int FP_CLASS_ZERO = 0;       // ±0
constexpr int FP_CLASS_NORMAL = 1;     // Normal finite numbers
constexpr int FP_CLASS_SUBNORMAL = 2;  // Subnormal finite numbers
constexpr int FP_CLASS_INFINITE = 3;   // ±∞
constexpr int FP_CLASS_NAN = 4;        // NaN values
}  // namespace cuda_fix_detail

#ifndef FP_ZERO
#define FP_ZERO cuda_fix_detail::FP_CLASS_ZERO
#endif

#ifndef FP_NORMAL
#define FP_NORMAL 1
#endif

#ifndef FP_SUBNORMAL
#define FP_SUBNORMAL 2
#endif

#ifndef FP_INFINITE
#define FP_INFINITE 3
#endif

#ifndef FP_NAN
#define FP_NAN 4
#endif

// *** CUDA COMPATIBILITY SETTINGS ***
// Disable CUDA version check
#ifndef SEP_NV_NO_HOST_COMPILER_CHECK
#define SEP_NV_NO_HOST_COMPILER_CHECK 1
#endif

// *** CUDA FUNCTION DECLARATIONS ***
// These declarations provide the CUDA-compatible versions of math functions
// that would otherwise conflict with system headers

#ifdef __cplusplus
extern "C" {

// Define stubs for long double math functions that GCC-14 expects but are missing in CUDA
#if defined(__CUDACC_VER_MAJOR__) && (__CUDACC_VER_MAJOR__ >= 12)
#  define SEP_HAS_CUDA_LDOUBLE_FUNCS 1
#else
#  define SEP_HAS_CUDA_LDOUBLE_FUNCS 0
#endif

inline long double acosl(long double x) {
    if (x < -1.0L || x > 1.0L) {
        errno = EDOM;
        return NAN;
    }
#if defined(__CUDA_ARCH__)
    long double r = sqrtl(1.0L - x * x);
    return atan2l(r, x);
#else
    return std::acosl(x);
#endif
}
inline long double asinl(long double x) {
    if (x < -1.0L || x > 1.0L) {
        errno = EDOM;
        return NAN;
    }
#if defined(__CUDA_ARCH__)
    long double r = sqrtl(1.0L - x * x);
    return atan2l(x, r);
#else
    return std::asinl(x);
#endif
}
inline long double atanl(long double x) {
#if defined(__CUDA_ARCH__)
    return atan((double)x);
#else
    return std::atanl(x);
#endif
}
inline long double atan2l(long double y, long double x) {
#if defined(__CUDA_ARCH__)
    return atan2((double)y, (double)x);
#else
    return std::atan2l(y, x);
#endif
}
inline long double ceill(long double x) {
    return ceil((double)x);
}
inline long double cosl(long double x) {
    return cos((double)x);
}
inline long double coshl(long double x) {
    return cosh((double)x);
}
inline long double expl(long double x) {
    return exp((double)x);
}
inline long double fabsl(long double x) {
    return fabs((double)x);
}
inline long double floorl(long double x) {
    return floor((double)x);
}
inline long double fmodl(long double x, long double y) {
    return fmod((double)x, (double)y);
}
inline long double frexpl(long double x, int* exp) {
    return frexp((double)x, exp);
}
inline long double ldexpl(long double x, int exp) {
    return ldexp((double)x, exp);
}
SEP_HD inline long double logl(long double x) {
    if (x <= 0.0L) {
        errno = EDOM;
        return NAN;
    }
#if defined(__CUDACC__) && SEP_HAS_CUDA_LDOUBLE_FUNCS
    return ::logl(x);
#else
    long double r = static_cast<long double>(log(static_cast<double>(x)));
    long double exp_r = exp(r);
    r += (x - exp_r) / exp_r;
    return r;
#endif
}
inline long double log10l(long double x) {
    return log10((double)x);
}
inline long double modfl(long double x, long double* iptr) {
    double int_part;
    double result = modf((double)x, &int_part);
    *iptr = int_part;
    return result;
}
inline long double powl(long double x, long double y) {
    return pow((double)x, (double)y);
}
inline long double sinl(long double x) {
    return sin((double)x);
}
inline long double sinhl(long double x) {
    return sinh((double)x);
}
inline long double sqrtl(long double x) {
#if defined(__CUDACC__) && SEP_HAS_CUDA_LDOUBLE_FUNCS
    return ::sqrtl(x);
#else
    if (x < 0.0L) {
        errno = EDOM;
        return NAN;
    }
    long double r = static_cast<long double>(sqrt(static_cast<double>(x)));
    if (x != 0.0L) {
        r = 0.5L * (r + x / r);
    }
    return r;
#endif
}
inline long double tanl(long double x) {
    return tan((double)x);
}
inline long double tanhl(long double x) {
    return tanh((double)x);
}

// C99 functions
inline long double acoshl(long double x) {
    return acosh((double)x);
}
inline long double asinhl(long double x) {
    return asinh((double)x);
}
inline long double atanhl(long double x) {
    return atanh((double)x);
}
inline long double cbrtl(long double x) {
    return cbrt((double)x);
}
inline long double copysignl(long double x, long double y) {
    return copysign((double)x, (double)y);
}
inline long double erfl(long double x) {
    return erf((double)x);
}
inline long double erfcl(long double x) {
    return erfc((double)x);
}
inline long double exp2l(long double x) {
    return exp2((double)x);
}
inline long double expm1l(long double x) {
    return expm1((double)x);
}
inline long double fdiml(long double x, long double y) {
    return fdim((double)x, (double)y);
}
inline long double fmal(long double x, long double y, long double z) {
    return fma((double)x, (double)y, (double)z);
}
inline long double fmaxl(long double x, long double y) {
    return fmax((double)x, (double)y);
}
inline long double fminl(long double x, long double y) {
    return fmin((double)x, (double)y);
}
inline long double hypotl(long double x, long double y) {
    return hypot((double)x, (double)y);
}
inline int ilogbl(long double x) {
    return ilogb((double)x);
}
inline long double lgammal(long double x) {
    return lgamma((double)x);
}
inline long long llrintl(long double x) {
    return llrint((double)x);
}
inline long long llroundl(long double x) {
    return llround((double)x);
}
inline long double log1pl(long double x) {
    return log1p((double)x);
}
inline long double log2l(long double x) {
    return log2((double)x);
}
inline long double logbl(long double x) {
    return logb((double)x);
}
inline long lrintl(long double x) {
    return lrint((double)x);
}
inline long lroundl(long double x) {
    return lround((double)x);
}
inline long double nanl(const char* tagp) {
    return nan(tagp);
}
inline long double nearbyintl(long double x) {
    return nearbyint((double)x);
}
inline long double nextafterl(long double x, long double y) {
    return nextafter((double)x, (double)y);
}
inline double nexttoward(double x, long double y) {
    return nextafter(x, (double)y);
}
inline float nexttowardf(float x, long double y) {
    return nextafterf(x, (double)y);
}
inline long double nexttowardl(long double x, long double y) {
    return nextafter((double)x, (double)y);
}
inline long double remainderl(long double x, long double y) {
    return remainder((double)x, (double)y);
}
inline long double remquol(long double x, long double y, int* quo) {
    return remquo((double)x, (double)y, quo);
}
#ifndef SEP_CUDA_RINT_DEFINED
#define SEP_CUDA_RINT_DEFINED
inline double rint(double x) {
    return nearbyint(x);
}
#endif
inline long double rintl(long double x) {
    return nearbyint((double)x);
}
inline long double roundl(long double x) {
    return round((double)x);
}
inline long double scalblnl(long double x, long n) {
    return scalbln((double)x, n);
}
inline long double scalbnl(long double x, int n) {
    return scalbn((double)x, n);
}
inline long double tgammal(long double x) {
    return tgamma((double)x);
}
inline long double truncl(long double x) {
    return trunc((double)x);
}

#ifdef __CUDACC__
// Double precision implementations
SEP_HOST SEP_DEVICE inline double __cuda_sinpi(double x) {
    return sin(M_PI * x);
}
SEP_HOST SEP_DEVICE inline double __cuda_cospi(double x) {
    return cos(M_PI * x);
}
SEP_HOST SEP_DEVICE inline void __cuda_sincospi(double x, double* s, double* c) {
    *s = sin(M_PI * x);
    *c = cos(M_PI * x);
}
SEP_HOST SEP_DEVICE inline double __cuda_tanpi(double x) {
    return tan(M_PI * x);
}

// Single precision implementations
SEP_HOST SEP_DEVICE inline float __cuda_sinpif(float x) {
    return sinf(M_PI * x);
}
SEP_HOST SEP_DEVICE inline float __cuda_cospif(float x) {
    return cosf(M_PI * x);
}
SEP_HOST SEP_DEVICE inline void __cuda_sincospif(float x, float* s, float* c) {
    *s = sinf(M_PI * x);
    *c = cosf(M_PI * x);
}
SEP_HOST SEP_DEVICE inline float __cuda_tanpif(float x) {
    return tanf(M_PI * x);
}
#endif  // __CUDACC__

};  // extern "C"
#endif

#ifdef __cplusplus
template <typename Func>
inline auto make_long_double_wrapper_1arg(Func f) {
    return [f](long double x) -> long double { return static_cast<long double>(f(static_cast<double>(x))); };
}

template <typename Func>
inline long double cast_and_call_2arg(Func f, long double x, long double y) {
    return static_cast<long double>(f(static_cast<double>(x), static_cast<double>(y)));
}
#endif

// *** CUDA IMPLEMENTATION STUBS ***
// These provide minimal implementations to satisfy the linker
// Real implementations would be provided by CUDA libraries

// *** KEYWORD AND DEVICE FUNCTION CONFLICT PREVENTION ***
// Prevent keyword and device function conflicts
#ifdef __CUDACC__
#define SEP_VMAXS2 __vmaxs2_disabled
#define SEP_VMINS2 __vmins2_disabled
#define SEP_VMAXU2 __vmaxu2_disabled
#define SEP_VMINU2 __vminu2_disabled
#define SEP_VADD2 __vadd2_disabled
#define SEP_NV_BSWAP16_IMPL __nv_bswap16_impl_disabled
#define SEP_NV_BSWAP32_IMPL __nv_bswap32_impl_disabled
#define SEP_NV_BSWAP64_IMPL __nv_bswap64_impl_disabled
#define SEP_VIMAX_S16X2_RELU __vimax_s16x2_relu_disabled
#define SEP_VIMIN_S16X2_RELU __vimin_s16x2_relu_disabled
#define SEP_VIMAX3_U16X2 __vimax3_u16x2_disabled
#define SEP_VIMIN3_U16X2 __vimin3_u16x2_disabled
#define SEP_VIADDMAX_S16X2 __viaddmax_s16x2_disabled
#define SEP_VIADDMAX_U16X2 __viaddmax_u16x2_disabled
#define SEP_VIADDMIN_S16X2 __viaddmin_s16x2_disabled
#define SEP_VIADDMIN_U16X2 __viaddmin_u16x2_disabled
#define SEP_NV_BSWAP16 __nv_bswap16_disabled
#define SEP_NV_BSWAP32 __nv_bswap32_disabled
#define SEP_NV_BSWAP64 __nv_bswap64_disabled
#endif  // __CUDACC__

// Restore any pushed macros at the end (none currently)
#endif  // CUDA_UNIFIED_FIX_H
