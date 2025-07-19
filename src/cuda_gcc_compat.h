#ifndef CUDA_GCC_COMPAT_H
#define CUDA_GCC_COMPAT_H

// Workaround for GCC 14+ and CUDA 12.x compatibility issues
// Specifically addresses errors related to __is_pointer, __is_volatile, etc.
// and _Float32 in mathcalls.h

#if defined(__CUDACC__) && defined(__GNUC__) && (__GNUC__ >= 14)

// Disable host compiler version check in host_config.h
#define __NV_NO_HOST_COMPILER_CHECK 1

// Workaround for __is_pointer, __is_volatile, etc.
// These are GCC built-ins that nvcc's cudafe++ might not understand
// when parsing standard library headers.
// We define them as simple type traits if they are not already defined.
#ifndef __is_pointer
#define __is_pointer(T) __is_pointer_impl(T)
template <typename T> struct __is_pointer_impl { static const bool value = false; };
template <typename T> struct __is_pointer_impl<T*> { static const bool value = true; };
#endif

#ifndef __is_volatile
#define __is_volatile(T) __is_volatile_impl(T)
template <typename T> struct __is_volatile_impl { static const bool value = false; };
template <typename T> struct __is_volatile_impl<volatile T> { static const bool value = true; };
#endif

#ifndef __array_rank
#define __array_rank(T) __array_rank_impl(T)
template <typename T> struct __array_rank_impl { static const size_t value = 0; };
template <typename T, size_t N> struct __array_rank_impl<T[N]> { static const size_t value = 1 + __array_rank_impl<T>::value; };
template <typename T> struct __array_rank_impl<T[]> { static const size_t value = 1 + __array_rank_impl<T>::value; };
#endif

// For __is_invocable, __is_nothrow_invocable, __is_convertible, etc.
// These are more complex. For now, we'll rely on the --gnu_version flag
// or hope that the pre-include helps. If not, more specific workarounds
// might be needed, potentially involving disabling certain C++ features
// or providing simplified versions.

// Workaround for _Float32 and similar types in mathcalls.h
// These types are part of newer C standards (C23) and GCC extensions.
// CUDA's headers might not be ready for them.
// We can try to undefine the macro that enables them if it's causing issues.
#ifdef __GLIBC_USE_IEC_60559_TYPES_EXT__
#undef __GLIBC_USE_IEC_60559_TYPES_EXT__
#endif

// Workaround for noexcept in mathcalls.h
// CUDA's math functions might not have noexcept, causing conflicts.
// We can try to redefine noexcept to nothing for these specific headers.
// More aggressive undefines for glibc's noexcept macros
#if defined(__GNUC__) && !defined(__clang__)
#undef __THROW
#define __THROW
#undef __NTH
#define __NTH(fct) fct
// Also try to undefine noexcept itself
#undef noexcept
#define noexcept
#endif

#endif // __CUDACC__ && __GNUC__ && (__GNUC__ >= 14)

#endif // CUDA_GCC_COMPAT_H