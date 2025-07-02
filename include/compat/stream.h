#ifndef SEP_CUDA_STREAM_H
#define SEP_CUDA_STREAM_H

// First include CUDA macros to set up SEP_CUDA_AVAILABLE
#include "compat/macros.h"

// Then include CUDA runtime compatibility header for GCC 14 fixes
#include <cuda_runtime.h>

// Standard includes
#include <memory>

// Project includes
#include "core/types.h"

// CUDA implementation headers last
#ifdef __CUDACC__
#include "compat/cuda_common.h"
#endif

namespace sep {
namespace cuda {

// Forward declare Stream for StreamPtr
class Stream;

// Type alias for shared Stream pointer
using StreamPtr = std::shared_ptr<Stream>;

class Stream {
 public:
  Stream();
  ~Stream();

  Stream(const Stream&) = delete;
  Stream& operator=(const Stream&) = delete;
  Stream(Stream&&) noexcept;
  Stream& operator=(Stream&&) noexcept;

  void synchronize();
  void wait(void* event);
  void record(void* event);
  void* handle() const;
  bool isValid() const;

  static std::shared_ptr<Stream> create(::sep::StreamFlags flags = ::sep::StreamFlags::Default);

 private:
  struct Impl;
  std::unique_ptr<Impl> impl_;
};

// Utility functions
void synchronizeStream(Stream& stream);
void waitEvent(Stream& stream, void* event);
void recordEvent(Stream& stream, void* event);

}  // namespace cuda
}  // namespace sep

#endif  // SEP_CUDA_STREAM_H
