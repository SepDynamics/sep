#pragma once

#include "compat/cuda_wrapper.h"

namespace sep {
namespace cuda {
    // Unified wrapper namespace aliasing existing SEP CUDA wrappers.
    using namespace ::sep::cuda; // NOLINT(manual-namespace-usage)
}
} // namespace sep

