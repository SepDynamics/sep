#include <cuda_runtime.h>

#include <stdexcept>

#include "stream.h"

namespace sep {
namespace cuda {

Stream::Stream() {
    if (cudaStreamCreate(&stream_) != cudaSuccess) {
        throw std::runtime_error("Failed to create CUDA stream");
    }
}

Stream::~Stream() {
    if (stream_) {
        cudaStreamDestroy(stream_);
    }
}

Stream::Stream(Stream&& other) noexcept : stream_(other.stream_) {
    other.stream_ = nullptr;
}

Stream& Stream::operator=(Stream&& other) noexcept {
    if (this != &other) {
        if (stream_) {
            cudaStreamDestroy(stream_);
        }
        stream_ = other.stream_;
        other.stream_ = nullptr;
    }
    return *this;
}

void Stream::synchronize() {
    if (stream_) {
        cudaStreamSynchronize(stream_);
    }
}

void Stream::wait(cudaEvent_t event) {
    if (stream_) {
        cudaStreamWaitEvent(stream_, event, 0);
    }
}

void Stream::record(cudaEvent_t event) {
    if (stream_) {
        cudaEventRecord(event, stream_);
    }
}

cudaStream_t Stream::handle() const {
    return stream_;
}

bool Stream::isValid() const {
    return stream_ != nullptr;
}

std::shared_ptr<Stream> Stream::create(unsigned int flags) {
    auto stream = std::make_shared<Stream>();
    if (cudaStreamCreateWithFlags(&stream->stream_, flags) != cudaSuccess) {
        throw std::runtime_error("Failed to create CUDA stream");
    }
    return stream;
}

}  // namespace cuda
}  // namespace sep
