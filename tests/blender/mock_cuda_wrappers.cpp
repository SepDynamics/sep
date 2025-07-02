#include <gmock/gmock.h>
#include "compat/cuda.h"
#include "compat/cuda_api.hpp"

namespace sep {
namespace compat {

class MockCudaWrapper : public CudaApi {
public:
    MOCK_METHOD(cudaError_t, cudaMemcpyAsync, (void* dst, const void* src, size_t size, cudaMemcpyKind kind, cudaStream_t stream), (override));
    MOCK_METHOD(cudaError_t, cudaMalloc, (void** ptr, size_t size), (override));
    MOCK_METHOD(cudaError_t, cudaFree, (void* ptr), (override));
    MOCK_METHOD(cudaError_t, cudaStreamCreate, (cudaStream_t* stream), (override));
    MOCK_METHOD(cudaError_t, cudaStreamDestroy, (cudaStream_t stream), (override));
};

} // namespace compat
} // namespace sep