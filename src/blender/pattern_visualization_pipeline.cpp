#include <cstring> // For memcpy, memset, memcmp, strlen, etc.
#include <ctime>   // For time-related functions
#include <string>  // For std::string
#include "blender/pattern_visualization_pipeline.h"
#include "compat/shim.h"
#include "core/common.h"  // defines sep::SEPResult
#include <vector>
#include <algorithm>
#include <numeric>

namespace sep {
namespace blender {

PatternVisualizationPipeline::PatternVisualizationPipeline(MeshHandler *handler,
                                                           GPUContext *gpu_ctx)
    : handler_(handler), gpu_ctx_(gpu_ctx), last_shader_revision_(0) {} 

std::array<float, 3> PatternVisualizationPipeline::projectNDim(
    const std::vector<float> &coords) const { 
  std::array<float, 3> out{0.0f, 0.0f, 0.0f}; 
  if (coords.empty())
    return out;

  out[0] = coords.size() > 0 ? coords[0] : 0.0f;
  out[1] = coords.size() > 1 ? coords[1] : 0.0f;
  out[2] = coords.size() > 2 ? coords[2] : 0.0f;

  if (coords.size() > 3) {
    float w = 1.0f / static_cast<float>(coords.size() - 3);
    for (size_t i = 3; i < coords.size(); ++i) {
      out[(i - 3) % 3] += coords[i] * w;
    }
  }
  return out;
}

sep::SEPResult PatternVisualizationPipeline::checkShaderReload() {
  if (!gpu_ctx_)
    return sep::SEPResult::SUCCESS;
  sep::SEPResult res = gpu_ctx_->reloadComputeShaderIfNeeded();
  if (res != sep::SEPResult::SUCCESS)
    return res;
  last_shader_revision_ = gpu_ctx_->getShaderRevision();
  return sep::SEPResult::SUCCESS;
}

sep::SEPResult PatternVisualizationPipeline::updatePattern(
    const pattern::PatternData &pattern) {
  if (!handler_) {
    return sep::SEPResult::INVALID_STATE;
  }
  checkShaderReload();
  // MeshHandler handles depsgraph updates internally.
  return handler_->update(pattern);
}

sep::SEPResult PatternVisualizationPipeline::deformMesh(
    const MeshHandler::DeformParams &params) {
  if (!handler_) {
    return sep::SEPResult::INVALID_STATE;
  }
  // Deformation already triggers a depsgraph notification via MeshHandler.
  return handler_->applyDeformation(params);
}

sep::SEPResult PatternVisualizationPipeline::generateMesh(
    const pattern::PatternData &pattern, int dimensionality) {
  if (!handler_ || dimensionality < 2) {
    return sep::SEPResult::INVALID_ARGUMENT;
  }
  return handler_->generateHyperMesh(pattern, dimensionality);
}

sep::SEPResult PatternVisualizationPipeline::renderManifold(
    const pattern::PatternData &pattern, int dimensionality) {
  sep::SEPResult r = generateMesh(pattern, dimensionality);
  if (r != sep::SEPResult::SUCCESS)
    return r;

  std::vector<float> coords;
  coords.reserve(static_cast<size_t>(dimensionality));
  coords.push_back(pattern.position.x);
  if (dimensionality > 1)
    coords.push_back(pattern.position.y);
  if (dimensionality > 2)
    coords.push_back(pattern.position.z);
  if (dimensionality > 3)
    coords.push_back(pattern.position.w);
  if (dimensionality > 4)
    coords.push_back(pattern.velocity.x);
  if (dimensionality > 5)
    coords.push_back(pattern.velocity.y);
  if (dimensionality > 6)
    coords.push_back(pattern.velocity.z);
  if (dimensionality > 7)
    coords.push_back(pattern.velocity.w);
  if (dimensionality > 8)
    coords.push_back(pattern.attributes.x);
  if (dimensionality > 9)
    coords.push_back(pattern.attributes.y);
  if (dimensionality > 10)
    coords.push_back(pattern.attributes.z);
  if (dimensionality > 11)
    coords.push_back(pattern.attributes.w);

  auto projected = projectNDim(coords);
  sep::pattern::PatternData projected_pattern = pattern;
  projected_pattern.position.x = projected[0];
  projected_pattern.position.y = projected[1];
  projected_pattern.position.z = projected[2];

  return updatePattern(projected_pattern);
}

sep::SEPResult PatternVisualizationPipeline::applyCoherenceOverlay(
    const std::vector<float> &history) {
  if (!handler_ || history.empty()) {
    return sep::SEPResult::INVALID_ARGUMENT;
  }
  float avg =
      std::accumulate(history.begin(), history.end(), 0.0f) / history.size();
  sep::SEPResult r =
      handler_->setUniformFloatLayer("coherence_overlay", history.back());
  if (r != sep::SEPResult::SUCCESS)
    return r;
  return handler_->setUniformFloatLayer("coherence_average", avg);
}

} // namespace blender
} // namespace sep
