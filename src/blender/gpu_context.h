#pragma once

#include "core/common.h"
#include "compat/shim.h"
#include <cstdint>

namespace sep {

// Forward declaration for GPU buffer
struct GPUBuffer {
    void* ptr{nullptr};
    size_t size{0};
    bool mapped{false};
};

struct GpuBufferPtr;

// Minimal implementation of GPU context for pattern_processor.cpp
class GPUContext {
public:
    GPUContext();
    virtual ~GPUContext();

    // Initialize the context on the given device index. Passing -1 selects the
    // default device.
    virtual SEPResult init(int device_index = -1);

    // Query and select devices
    virtual SEPResult getDeviceCount(int& count) const;
    virtual SEPResult selectDevice(int device_index);

    // Error state helpers
    bool hasError() const { return has_error_; }
    const ::sep::shim::string& getLastError() const { return last_error_; }
    void clearError() { has_error_ = false; last_error_ = {}; }

    // Buffer helpers
    virtual GpuBufferPtr createBuffer(size_t size, const void* data = nullptr);
    virtual void deleteBuffer(GPUBuffer* buffer);
    virtual void* mapBuffer(GPUBuffer* buffer);
    virtual void unmapBuffer(GPUBuffer* buffer);

    // Simplified shader handling for tests
    SEPResult loadComputeShader(const ::sep::shim::string& path);
    SEPResult reloadComputeShaderIfNeeded();
    uint32_t getShaderRevision() const { return shader_revision_; }

private:
    int device_index_{-1};
    bool initialized_{false};
    bool has_error_{false};
    ::sep::shim::string last_error_{};
    uint32_t shader_revision_{0};
    ::sep::shim::string shader_path_{};
    long long shader_timestamp_{0};
};

// RAII wrapper for GPUBuffer that works with the simplified context
struct GpuBufferPtr {
    GPUContext* ctx{nullptr};
    GPUBuffer* buffer{nullptr};

    GpuBufferPtr() = default;
    GpuBufferPtr(GPUContext* c, GPUBuffer* b) : ctx(c), buffer(b) {}
    GpuBufferPtr(const GpuBufferPtr&) = delete;
    GpuBufferPtr& operator=(const GpuBufferPtr&) = delete;
    
    GpuBufferPtr(GpuBufferPtr&& other) noexcept 
        : ctx(other.ctx), buffer(other.buffer) {
        other.ctx = nullptr;
        other.buffer = nullptr;
    }
    
    GpuBufferPtr& operator=(GpuBufferPtr&& other) noexcept {
        if (this != &other) {
            reset();
            ctx = other.ctx;
            buffer = other.buffer;
            other.ctx = nullptr;
            other.buffer = nullptr;
        }
        return *this;
    }
    
    ~GpuBufferPtr() {
        reset();
    }

    GPUBuffer* get() const { return buffer; }
    GPUBuffer* operator->() const { return buffer; }
    explicit operator bool() const { return buffer != nullptr; }
    
    GPUBuffer* release() {
        GPUBuffer* tmp = buffer;
        ctx = nullptr;
        buffer = nullptr;
        return tmp;
    }
    
    void reset(GPUBuffer* b = nullptr) {
        if (buffer && ctx) {
            ctx->deleteBuffer(buffer);
        }
        buffer = b;
    }
};

// Deleter for use with unique_ptr
struct GPUBufferDeleter {
    GPUContext* ctx{nullptr};
    
    void operator()(GPUBuffer* buffer) const noexcept {
        if (ctx && buffer) {
            ctx->deleteBuffer(buffer);
        }
    }
};

}  // namespace sep
