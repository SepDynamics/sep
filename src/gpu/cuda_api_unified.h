#pragma once

#include <memory>

namespace sep {
namespace gpu {

struct CudaStream;
struct CudaEvent;

enum class MemcpyKind {
    HostToHost,
    HostToDevice,
    DeviceToHost,
    DeviceToDevice,
    Default
};

enum class StreamFlags {
    Default = 0x00,
    NonBlocking = 0x01
};

// Error handling
class CudaError {
public:
    CudaError(int code, const std::string& message);
    bool is_success() const { return code_ == 0; }
    const std::string& message() const { return message_; }
    int code() const { return code_; }

    static CudaError success() { return CudaError(0, "Success"); }

private:
    int code_;
    std::string message_;
};

// Main API class - Singleton pattern for global state management
class CudaAPI {
public:
    static CudaAPI& instance();

    // Memory operations
    CudaError malloc(void** ptr, size_t size);
    CudaError free(void* ptr);
    CudaError memcpy(void* dst, const void* src, size_t count, MemcpyKind kind);
    CudaError memcpy_async(void* dst, const void* src, size_t count,
                          MemcpyKind kind, CudaStream* stream);
    CudaError memset(void* ptr, int value, size_t count);

    // Stream operations
    CudaError create_stream(CudaStream** stream, StreamFlags flags = StreamFlags::Default);
    CudaError destroy_stream(CudaStream* stream);
    CudaError stream_synchronize(CudaStream* stream);
    CudaError stream_wait_event(CudaStream* stream, CudaEvent* event);

    // Device management
    CudaError get_device_count(int* count);
    CudaError set_device(int device);
    CudaError get_device(int* device);

    CudaError get_last_error();
    std::string error_string(const CudaError& error);

private:
    CudaAPI() = default;
    ~CudaAPI() = default;
    CudaAPI(const CudaAPI&) = delete;
    CudaAPI& operator=(const CudaAPI&) = delete;
};

// RAII Wrappers
class CudaMemory {
public:
    explicit CudaMemory(size_t size);
    ~CudaMemory();

    void* get() { return ptr_; }
    const void* get() const { return ptr_; }
    size_t size() const { return size_; }

    // Move semantics only
    CudaMemory(CudaMemory&& other) noexcept;
    CudaMemory& operator=(CudaMemory&& other) noexcept;

    // No copy
    CudaMemory(const CudaMemory&) = delete;
    CudaMemory& operator=(const CudaMemory&) = delete;

private:
    void* ptr_ = nullptr;
    size_t size_ = 0;
};

} // namespace gpu
} // namespace sep

