#ifndef SEP_CUDA_KERNELS_H
#define SEP_CUDA_KERNELS_H

#include "constants.h"
#include "types.h"
#include <cstdint>

#ifdef __CUDACC__
#include <compat/cuda_runtime.h>
#endif

namespace sep {
namespace cuda {

// Forward declarations of kernel launch functions
cudaError_t
launchQBSAKernel(const std::uint32_t *d_probe_indices,
                 const std::uint32_t *d_expectations, std::uint32_t num_probes,
                 std::uint32_t *d_bitfield, std::uint32_t *d_corrections,
                 std::uint32_t *d_correction_count, cudaStream_t stream);

cudaError_t launchQSHKernel(const std::uint64_t *d_chunks,
                            std::uint32_t num_chunks,
                            std::uint32_t *d_collapse_indices,
                            std::uint32_t *d_collapse_counts,
                            cudaStream_t stream);

cudaError_t launchSimilarityKernel(float *d_similarity, const float *d_emb_a,
                                   const float *d_emb_b,
                                   std::uint32_t embedding_size,
                                   cudaStream_t stream);

cudaError_t launchBlendKernel(float *d_output, const float *d_embeddings,
                              const float *d_weights,
                              std::uint32_t num_contexts,
                              std::uint32_t embedding_size,
                              cudaStream_t stream);

} // namespace cuda
} // namespace sep

#endif // SEP_CUDA_KERNELS_H
