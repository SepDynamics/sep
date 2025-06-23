// GLM isolation layer
// Include CUDA compatibility layer for C++ standard library functions

// Include CUDA headers
#if defined(__CUDACC__) || defined(SEP_USE_CUDA)
#include "compat/cuda_runtime.h"
#else
#include "compat/cuda_impl.h"
#endif


// After CUDA headers, include the rest

#include "compat/raii.h"
#include "memory/types.h"


namespace sep {

// NOTE: UnifiedMemory template class implementation is now provided inline in unified_memory.h
// This file only provides explicit template instantiations for common types

// Explicit instantiations for common types
template class UnifiedMemory<char>;
template class UnifiedMemory<unsigned char>;
template class UnifiedMemory<short>;
template class UnifiedMemory<unsigned short>;
template class UnifiedMemory<int>;
template class UnifiedMemory<unsigned int>;
template class UnifiedMemory<long>;
template class UnifiedMemory<unsigned long>;
template class UnifiedMemory<long long>;
template class UnifiedMemory<unsigned long long>;
template class UnifiedMemory<float>;
template class UnifiedMemory<double>;

} // namespace sep
