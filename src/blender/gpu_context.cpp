#include "blender/gpu_context.h"
#include <cstdlib>
#include <cstring>

namespace sep {

GPUContext::GPUContext() = default;
GPUContext::~GPUContext() = default;

SEPResult GPUContext::init(int device_index) {
    device_index_ = device_index;
    return SEPResult::SUCCESS;
}

SEPResult GPUContext::selectDevice(int device_index) {
    device_index_ = device_index;
    return SEPResult::SUCCESS;
}

GpuBufferPtr GPUContext::createBuffer(size_t size, const void* data) {
    GPUBuffer* buf = new GPUBuffer();
    buf->size = size;
    buf->ptr = std::malloc(size);
    if (!buf->ptr) {
        delete buf;
        return GpuBufferPtr{};
    }
    if (data) {
        std::memcpy(buf->ptr, data, size);
    }
    return GpuBufferPtr(this, buf);
}

void GPUContext::deleteBuffer(GPUBuffer* buffer) {
    if (!buffer) {
        return;
    }
    std::free(buffer->ptr);
    delete buffer;
}

void* GPUContext::mapBuffer(GPUBuffer* buffer) {
    if (!buffer || buffer->mapped) {
        return nullptr;
    }
    buffer->mapped = true;
    return buffer->ptr;
}

void GPUContext::unmapBuffer(GPUBuffer* buffer) {
    if (!buffer) {
        return;
    }
    buffer->mapped = false;
}

} // namespace sep
