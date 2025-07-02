#pragma once

#include "blender/mesh_handler.h"
#include "blender/gpu_context.h"
#include "blender/pattern_common.h"
#include "quantum/data.hpp"
#include "compat/shim.h"
#include <array>
#include <vector>

namespace sep {
namespace blender {

class PatternVisualizationPipeline {
public:
    explicit PatternVisualizationPipeline(MeshHandler* handler,
                                          GPUContext* gpu_ctx = nullptr);

    SEPResult updatePattern(const sep::pattern::PatternData& pattern);
    SEPResult deformMesh(const MeshHandler::DeformParams& params);

    // Generate a mesh with the specified dimensionality. Dimensions > 3 will
    // store additional coordinates in custom data layers for debugging.
    SEPResult generateMesh(const sep::pattern::PatternData& pattern,
                           int dimensionality);

    // Render a manifold of arbitrary dimensionality.
    SEPResult renderManifold(const sep::pattern::PatternData& pattern,
                             int dimensionality);

    // Map coherence history to an overlay layer for debugging.
    SEPResult applyCoherenceOverlay(const std::vector<float>& history);

private:
    std::array<float, 3> projectNDim(const std::vector<float>& coords) const;
    SEPResult checkShaderReload();

    MeshHandler* handler_;
    GPUContext* gpu_ctx_;
    uint32_t last_shader_revision_;
};

} // namespace blender
} // namespace sep
