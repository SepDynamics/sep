#include "cuda_api_unified.h"

#ifdef SEP_HAS_CUDA
#include <cuda_runtime_api.h>

namespace sep::gpu {
struct CudaStream { cudaStream_t handle; };
struct CudaEvent { cudaEvent_t handle; };
}
#else
#include <cstdlib>
#include <cstring>

namespace sep::gpu {
struct CudaStream { int dummy; };
struct CudaEvent { int dummy; };
}
#endif

namespace sep::gpu {

#ifdef SEP_HAS_CUDA
static cudaMemcpyKind convert_memcpy_kind(MemcpyKind kind) {
    switch (kind) {
        case MemcpyKind::HostToHost: return cudaMemcpyHostToHost;
        case MemcpyKind::HostToDevice: return cudaMemcpyHostToDevice;
        case MemcpyKind::DeviceToHost: return cudaMemcpyDeviceToHost;
        case MemcpyKind::DeviceToDevice: return cudaMemcpyDeviceToDevice;
        default: return cudaMemcpyDefault;
    }
}
#endif

CudaError::CudaError(int code, const std::string& message)
    : code_(code), message_(message) {}

CudaAPI& CudaAPI::instance() {
    static CudaAPI api;
    return api;
}

CudaError CudaAPI::malloc(void** ptr, size_t size) {
#ifdef SEP_HAS_CUDA
    cudaError_t err = ::cudaMalloc(ptr, size);
    return CudaError(static_cast<int>(err), ::cudaGetErrorString(err));
#else
    *ptr = std::malloc(size);
    return CudaError::success();
#endif
}

CudaError CudaAPI::free(void* ptr) {
#ifdef SEP_HAS_CUDA
    cudaError_t err = ::cudaFree(ptr);
    return CudaError(static_cast<int>(err), ::cudaGetErrorString(err));
#else
    std::free(ptr);
    return CudaError::success();
#endif
}

CudaError CudaAPI::memcpy(void* dst, const void* src, size_t count, MemcpyKind kind) {
#ifdef SEP_HAS_CUDA
    cudaError_t err = ::cudaMemcpy(dst, src, count, convert_memcpy_kind(kind));
    return CudaError(static_cast<int>(err), ::cudaGetErrorString(err));
#else
    std::memcpy(dst, src, count);
    return CudaError::success();
#endif
}

CudaError CudaAPI::memcpy_async(void* dst, const void* src, size_t count,
                                MemcpyKind kind, CudaStream* stream) {
#ifdef SEP_HAS_CUDA
    cudaStream_t s = stream ? stream->handle : nullptr;
    cudaError_t err = ::cudaMemcpyAsync(dst, src, count, convert_memcpy_kind(kind), s);
    return CudaError(static_cast<int>(err), ::cudaGetErrorString(err));
#else
    (void)stream;
    std::memcpy(dst, src, count);
    return CudaError::success();
#endif
}

CudaError CudaAPI::memset(void* ptr, int value, size_t count) {
#ifdef SEP_HAS_CUDA
    cudaError_t err = ::cudaMemset(ptr, value, count);
    return CudaError(static_cast<int>(err), ::cudaGetErrorString(err));
#else
    std::memset(ptr, value, count);
    return CudaError::success();
#endif
}

CudaError CudaAPI::create_stream(CudaStream** stream, StreamFlags flags) {
#ifdef SEP_HAS_CUDA
    auto s = new CudaStream;
    cudaError_t err = ::cudaStreamCreateWithFlags(&s->handle, flags == StreamFlags::NonBlocking ? cudaStreamNonBlocking : cudaStreamDefault);
    if (err != cudaSuccess) {
        delete s;
        return CudaError(static_cast<int>(err), ::cudaGetErrorString(err));
    }
    *stream = s;
    return CudaError::success();
#else
    *stream = new CudaStream{0};
    (void)flags;
    return CudaError::success();
#endif
}

CudaError CudaAPI::destroy_stream(CudaStream* stream) {
    if (!stream) return CudaError::success();
#ifdef SEP_HAS_CUDA
    cudaError_t err = ::cudaStreamDestroy(stream->handle);
    delete stream;
    return CudaError(static_cast<int>(err), ::cudaGetErrorString(err));
#else
    delete stream;
    return CudaError::success();
#endif
}

CudaError CudaAPI::stream_synchronize(CudaStream* stream) {
#ifdef SEP_HAS_CUDA
    cudaError_t err = ::cudaStreamSynchronize(stream ? stream->handle : nullptr);
    return CudaError(static_cast<int>(err), ::cudaGetErrorString(err));
#else
    (void)stream;
    return CudaError::success();
#endif
}

CudaError CudaAPI::stream_wait_event(CudaStream* stream, CudaEvent* event) {
#ifdef SEP_HAS_CUDA
    cudaError_t err = ::cudaStreamWaitEvent(stream ? stream->handle : nullptr,
                                            event ? event->handle : nullptr, 0);
    return CudaError(static_cast<int>(err), ::cudaGetErrorString(err));
#else
    (void)stream;
    (void)event;
    return CudaError::success();
#endif
}

CudaError CudaAPI::get_device_count(int* count) {
#ifdef SEP_HAS_CUDA
    cudaError_t err = ::cudaGetDeviceCount(count);
    return CudaError(static_cast<int>(err), ::cudaGetErrorString(err));
#else
    *count = 0;
    return CudaError::success();
#endif
}

CudaError CudaAPI::set_device(int device) {
#ifdef SEP_HAS_CUDA
    cudaError_t err = ::cudaSetDevice(device);
    return CudaError(static_cast<int>(err), ::cudaGetErrorString(err));
#else
    (void)device;
    return CudaError::success();
#endif
}

CudaError CudaAPI::get_device(int* device) {
#ifdef SEP_HAS_CUDA
    cudaError_t err = ::cudaGetDevice(device);
    return CudaError(static_cast<int>(err), ::cudaGetErrorString(err));
#else
    *device = 0;
    return CudaError::success();
#endif
}

CudaError CudaAPI::get_last_error() {
#ifdef SEP_HAS_CUDA
    cudaError_t err = ::cudaGetLastError();
    return CudaError(static_cast<int>(err), ::cudaGetErrorString(err));
#else
    return CudaError::success();
#endif
}

std::string CudaAPI::error_string(const CudaError& error) {
#ifdef SEP_HAS_CUDA
    (void)error; // message already provided
    return error.message();
#else
    return error.message();
#endif
}

CudaMemory::CudaMemory(size_t size) {
    if (CudaAPI::instance().malloc(&ptr_, size).is_success()) {
        size_ = size;
    } else {
        ptr_ = nullptr;
        size_ = 0;
    }
}

CudaMemory::~CudaMemory() {
    if (ptr_) {
        CudaAPI::instance().free(ptr_);
    }
}

CudaMemory::CudaMemory(CudaMemory&& other) noexcept {
    ptr_ = other.ptr_;
    size_ = other.size_;
    other.ptr_ = nullptr;
    other.size_ = 0;
}

CudaMemory& CudaMemory::operator=(CudaMemory&& other) noexcept {
    if (this != &other) {
        if (ptr_) {
            CudaAPI::instance().free(ptr_);
        }
        ptr_ = other.ptr_;
        size_ = other.size_;
        other.ptr_ = nullptr;
        other.size_ = 0;
    }
    return *this;
}

} // namespace sep::gpu

