#ifndef SEP_CUDA_STREAM_IMPL_H
#define SEP_CUDA_STREAM_IMPL_H

#include "compat/stream.h"
#include "compat/cuda_helpers.h"

#ifdef __CUDACC__
#include "compat/cuda_common.h"
#else
#include "compat/cuda_impl.h"
#endif

namespace sep {
namespace cuda {
namespace impl {

class StreamImpl {
 public:
  StreamImpl() : stream_(nullptr) {}

  // Non-copyable
  StreamImpl(const StreamImpl&) = delete;
  StreamImpl& operator=(const StreamImpl&) = delete;

  // Movable
  StreamImpl(StreamImpl&& other) noexcept : stream_(other.stream_) {
    other.stream_ = nullptr;
  }
  StreamImpl& operator=(StreamImpl&& other) noexcept {
    if (this != &other) {
      if (stream_) {
        cudaError_t err = cudaStreamDestroy(stream_);
        if (err != cudaSuccess) {
          ::sep::cuda::logCudaError("cudaStreamDestroy", err);
        }
      }
      stream_ = other.stream_;
      other.stream_ = nullptr;
    }
    return *this;
  }

  ~StreamImpl() {
    if (stream_) {
      cudaError_t err = cudaStreamDestroy(stream_);
      if (err != cudaSuccess) {
        ::sep::cuda::logCudaError("cudaStreamDestroy", err);
      }
      stream_ = nullptr;
    }
  }

  void synchronize() {
    if (stream_) {
      cudaError_t err = cudaStreamSynchronize(stream_);
      if (err != cudaSuccess) {
        ::sep::cuda::logCudaError("cudaStreamSynchronize", err);
      }
    }
  }

  void wait(void* event) {
    if (stream_ && event) {
      cudaError_t err =
          cudaStreamWaitEvent(stream_, static_cast<cudaEvent_t>(event), 0);
      if (err != cudaSuccess) {
        ::sep::cuda::logCudaError("cudaStreamWaitEvent", err);
      }
    }
  }

  void record(void* event) {
    if (stream_ && event) {
      cudaError_t err =
          cudaEventRecord(static_cast<cudaEvent_t>(event), stream_);
      if (err != cudaSuccess) {
        ::sep::cuda::logCudaError("cudaEventRecord", err);
      }
    }
  }

  void* handle() const { return stream_; }
  bool isValid() const { return stream_ != nullptr; }
  void setHandle(cudaStream_t stream) { stream_ = stream; }

 private:
  cudaStream_t stream_;
};

}  // namespace impl
}  // namespace cuda
}  // namespace sep

#endif  // SEP_CUDA_STREAM_IMPL_H
