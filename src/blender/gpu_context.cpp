#include "blender/gpu_context.h"
#include <cstdlib>
#include <cstring>

#include <cstring>
#include <new>

namespace sep {

GPUContext::GPUContext() = default;

GPUContext::~GPUContext() = default;

SEPResult GPUContext::init(int device_index) {
    device_index_ = device_index;
    initialized_ = true;
    clearError();
    return SEPResult::SUCCESS;
}

SEPResult GPUContext::getDeviceCount(int& count) const {
    count = 1;
    return SEPResult::SUCCESS;
}

SEPResult GPUContext::selectDevice(int device_index) {
    int count = 0;
    getDeviceCount(count);
    if (device_index < 0 || device_index >= count) {
        has_error_ = true;
        last_error_ = "Invalid device index";
        return SEPResult::INVALID_DEVICE;
    }
    device_index_ = device_index;
    return SEPResult::SUCCESS;
}

GpuBufferPtr GPUContext::createBuffer(size_t size, const void* data) {
    if (size == 0) {
        has_error_ = true;
        last_error_ = "size must be greater than zero";
        return {};
    }
    auto* buf = new (std::nothrow) GPUBuffer();
    if (!buf) {
        has_error_ = true;
        last_error_ = "buffer allocation failed";
        return {};
    }
    buf->ptr = ::operator new(size, std::nothrow);
    if (!buf->ptr) {
        delete buf;
        has_error_ = true;
        last_error_ = "memory allocation failed";
        return {};
    }
    buf->size = size;
    if (data) {
        std::memcpy(buf->ptr, data, size);
    }
    buf->mapped = false;
    return GpuBufferPtr(this, buf);
}

void GPUContext::deleteBuffer(GPUBuffer* buffer) {
    if (!buffer) {
        return;
    }
    ::operator delete(buffer->ptr);
    delete buffer;
}

void* GPUContext::mapBuffer(GPUBuffer* buffer) {
    if (!buffer || buffer->mapped) {
        has_error_ = true;
        last_error_ = "invalid map";
        return nullptr;
    }
    buffer->mapped = true;
    return buffer->ptr;
}

void GPUContext::unmapBuffer(GPUBuffer* buffer) {
    if (buffer) {
        buffer->mapped = false;
    }
}

} // namespace sep
