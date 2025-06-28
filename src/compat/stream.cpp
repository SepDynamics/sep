#include <memory>

#include "compat/cuda_runtime.h" // Ensure cudaStream_t is defined
#include "compat/stream.h"

namespace sep::cuda {

// Implementation storing the underlying CUDA stream handle
struct Stream::Impl {
  cudaStream_t stream_handle_{nullptr};

  void setHandle(cudaStream_t handle) { stream_handle_ = handle; }
  void synchronize() {
#if SEP_CUDA_AVAILABLE
    if (stream_handle_) cudaStreamSynchronize(stream_handle_);
#else
    (void)stream_handle_;
#endif
  }
  void wait(void* event) {
#if SEP_CUDA_AVAILABLE
    if (stream_handle_ && event)
      cudaStreamWaitEvent(stream_handle_, static_cast<cudaEvent_t>(event), 0);
#else
    (void)stream_handle_;
    (void)event;
#endif
  }
  void record(void* event) {
#if SEP_CUDA_AVAILABLE
    if (stream_handle_ && event)
      cudaEventRecord(static_cast<cudaEvent_t>(event), stream_handle_);
#else
    (void)stream_handle_;
    (void)event;
#endif
  }
  void* handle() const { return stream_handle_; }
  bool isValid() const { return stream_handle_ != nullptr; }
};

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
      (flags == sep::StreamFlags::NonBlocking) ?
      cudaStreamNonBlocking :
      cudaStreamDefault;

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
