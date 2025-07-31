#ifndef SEP_CUDA_KERNELS_H
#define SEP_CUDA_KERNELS_H

#include <cuda_runtime.h>

#include <cstdint>

#include "quantum/bitspace/qfh.h"
#include "quantum/bitspace/forward_window_result.h"

// Forward declarations of kernel launch functions
cudaError_t launchQBSAKernel(const std::uint32_t *d_probe_indices,
                           const std::uint32_t *d_expectations, std::uint32_t num_probes,
                           std::uint32_t *d_bitfield, std::uint32_t *d_corrections,
                           std::uint32_t *d_correction_count, cudaStream_t stream);

cudaError_t launchQSHKernel(const std::uint64_t *d_chunks,
                            std::uint32_t num_chunks,
                            std::uint32_t *d_collapse_indices,
                            std::uint32_t *d_collapse_counts,
                            cudaStream_t stream);

cudaError_t launchQFHBitTransitionsKernel(const uint8_t *d_bit_packages, int num_packages,
                                          int package_size,
                                          sep::quantum::bitspace::ForwardWindowResult *d_results,
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

#endif // SEP_CUDA_KERNELS_H
