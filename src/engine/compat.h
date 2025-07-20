#ifndef SEP_CUDA_COMPAT_H
#define SEP_CUDA_COMPAT_H

// Include CUDA macro definitions
#include "macros.h"

// CUDA compatibility layer for SEP engine
#if SEP_CUDA_AVAILABLE
#include <cuda_runtime.h>

#include "cuda_helpers.h"
#include "math_common.h"

// We don't need to define uint64_t as it's already provided by system headers
// in <stdint.h> or <cstdint> which is included by other headers

namespace sep {
namespace cuda {

// Forward declaration compatibility for CUDA
template <typename T>
SEP_HOST SEP_DEVICE T&& forward(typename std::remove_reference<T>::type& t) noexcept {
    return static_cast<T&&>(t);
}

template <typename T>
SEP_HOST SEP_DEVICE T&& forward(typename std::remove_reference<T>::type&& t) noexcept {
    static_assert(!std::is_lvalue_reference<T>::value, "Can't forward rvalue as lvalue");
    return static_cast<T&&>(t);
}

// Promote compatibility for CUDA contexts
template <typename T1, typename T2>
struct promote {
    using type = decltype(T1() + T2());
};

}  // namespace cuda

}  // namespace sep

#endif  // SEP_CUDA_AVAILABLE

// Standard C++ compatibility
#if !SEP_CUDA_AVAILABLE
#include <cmath>  // For std::sqrt
#include <cstdint>
#include <utility>

namespace sep {
namespace cuda {

// Non-CUDA forward declarations
template <typename T>
T&& forward(T&& t) noexcept {
    return std::forward<T>(t);
}

// Non-CUDA promote
template <typename T1, typename T2>
struct promote {
    using type = decltype(T1() + T2());
};

}  // namespace cuda

}  // namespace sep
#endif  // !SEP_CUDA_AVAILABLE

#endif  // SEP_CUDA_COMPAT_H
