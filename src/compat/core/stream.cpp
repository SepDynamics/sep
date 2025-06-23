#include <memory>

#include "compat/cuda_common.h"
#include "compat/stream.h"
#include "compat/stream_impl.h"

namespace sep::cuda {

// Define Stream::Impl as StreamImpl
struct Stream::Impl : public impl::StreamImpl {};

Stream::~Stream() = default;

Stream::Stream(Stream&& other) noexcept = default;
Stream& Stream::operator=(Stream&& other) noexcept = default;

// Host-side implementation
Stream::Stream() : impl_(std::make_unique<Impl>()) {}

void Stream::synchronize() { impl_->synchronize(); }

void Stream::wait(void* event) { impl_->wait(event); }

void Stream::record(void* event) { impl_->record(event); }

void* Stream::handle() const { return impl_->handle(); }

bool Stream::isValid() const { return impl_->isValid(); }

std::shared_ptr<Stream> Stream::create(sep::StreamFlags flags) {
  auto stream = std::make_shared<Stream>();

  cudaStream_t cuda_stream;
  unsigned int cuda_flags =
      (flags == sep::StreamFlags::NonBlocking) ? cudaStreamNonBlocking : cudaStreamDefault;

  cudaError_t err = cudaStreamCreateWithFlags(&cuda_stream, cuda_flags);
  if (err != cudaSuccess) {
    ::sep::cuda::logCudaError("cudaStreamCreateWithFlags", err);
    return nullptr;
  }
  stream->impl_->setHandle(cuda_stream);
  return stream;
}

void synchronizeStream(Stream& stream) { stream.synchronize(); }

void waitEvent(Stream& stream, void* event) { stream.wait(event); }

void recordEvent(Stream& stream, void* event) { stream.record(event); }

}  // namespace sep::cuda
