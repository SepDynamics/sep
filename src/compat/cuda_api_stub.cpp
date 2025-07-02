#ifndef SEP_USE_CUDA
#include "compat/cuda_api.hpp"
#include "core/common.h"

extern "C" {

sep::SEPResult sep_cuda_init(int /*device_id*/) { return sep::SEPResult::FEATURE_UNAVAILABLE; }
sep::SEPResult sep_cuda_cleanup(void) { return sep::SEPResult::SUCCESS; }

sep::SEPResult sep_cuda_process_batch(const std::uint32_t*, const std::uint32_t*,
                                      std::uint32_t, std::uint32_t*, std::uint32_t*,
                                      std::uint32_t*) {
  return sep::SEPResult::FEATURE_UNAVAILABLE;
}

sep::SEPResult sep_cuda_process_symmetry(const std::uint64_t*, std::uint32_t,
                                         std::uint32_t*, std::uint32_t*) {
  return sep::SEPResult::FEATURE_UNAVAILABLE;
}

}  // extern "C"
#endif  // SEP_USE_CUDA
