#ifndef SEP_CUDA_STREAM_H
#define SEP_CUDA_STREAM_H

#include <cuda_runtime.h>
#include <memory>

namespace sep {
namespace cuda {

class Stream {
 public:
  Stream();
  ~Stream();

  Stream(const Stream&) = delete;
  Stream& operator=(const Stream&) = delete;
  Stream(Stream&&) noexcept;
  Stream& operator=(Stream&&) noexcept;

  void synchronize();
  void wait(cudaEvent_t event);
  void record(cudaEvent_t event);
  cudaStream_t handle() const;
  bool isValid() const;

  static std::shared_ptr<Stream> create(unsigned int flags = cudaStreamDefault);

 private:
  cudaStream_t stream_ = nullptr;
};

}  // namespace cuda
}  // namespace sep

#endif  // SEP_CUDA_STREAM_H
