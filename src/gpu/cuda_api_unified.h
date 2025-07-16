#pragma once

#include <cstddef>
#include <string>

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

class CudaError {
public:
    CudaError(int code, const std::string& msg) : code_(code), message_(msg) {}
    bool is_success() const { return code_ == 0; }
    const std::string& message() const { return message_; }
    int code() const { return code_; }
    static CudaError success() { return CudaError(0, "Success"); }
private:
    int code_;
    std::string message_;
};

class CudaAPI {
public:
    static CudaAPI& instance();

    CudaError malloc(void** ptr, std::size_t size);
    CudaError free(void* ptr);
    CudaError memcpy(void* dst, const void* src, std::size_t count, MemcpyKind kind);
    CudaError memcpy_async(void* dst, const void* src, std::size_t count, MemcpyKind kind, CudaStream* stream);
    CudaError memset(void* ptr, int value, std::size_t count);

    CudaError create_stream(CudaStream** stream, StreamFlags flags = StreamFlags::Default);
    CudaError destroy_stream(CudaStream* stream);
    CudaError stream_synchronize(CudaStream* stream);
    CudaError stream_wait_event(CudaStream* stream, CudaEvent* event);

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

class CudaMemory {
public:
    explicit CudaMemory(std::size_t size);
    ~CudaMemory();

    void* get() { return ptr_; }
    const void* get() const { return ptr_; }
    std::size_t size() const { return size_; }

    CudaMemory(CudaMemory&& other) noexcept;
    CudaMemory& operator=(CudaMemory&& other) noexcept;

    CudaMemory(const CudaMemory&) = delete;
    CudaMemory& operator=(const CudaMemory&) = delete;

private:
    void* ptr_ = nullptr;
    std::size_t size_ = 0;
};

} // namespace gpu
} // namespace sep

