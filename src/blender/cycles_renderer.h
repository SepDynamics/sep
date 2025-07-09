#pragma once
#include <string>
#include <vector>
#include <memory>
#include "quantum/data.hpp"
#include "core/types.h"
#include "core/common.h"
#include "compat/cycles.h"

namespace sep {
namespace blender {
namespace ccl {

#ifdef SEP_HAS_CYCLES
class CyclesRenderer {
public:
    struct RenderParams {
        int width{1920};
        int height{1080};
        float samples{100.0f};
        bool use_denoising{true};
        std::string output_format{"PNG"};
    };

    CyclesRenderer() = default;
    
    static SEPResult isCyclesAvailable();
    SEPResult initialize();
    SEPResult createSceneFromPatterns(const std::vector<pattern::PatternData>& patterns);
    SEPResult renderScene(const RenderParams& params);
    bool render(const std::string& filepath);

private:
    using ScenePtr = std::unique_ptr<::ccl::Scene>;

    bool initialized_{false};
    int width_{0};
    int height_{0};
    RenderParams last_params_{};
    std::vector<pattern::PatternData> patterns_;
    ScenePtr cycles_scene_{nullptr};
    ::ccl::Stats cycles_stats_;
    ::ccl::Profiler cycles_profiler_;
    std::unique_ptr<::ccl::Device> cycles_device_;

    void createGeometryFromPattern(const pattern::PatternData& pattern);
    void convertPatternToMesh(const pattern::PatternData& pattern,
                              std::vector<::ccl::float3>& verts,
                              std::vector<::ccl::int3>& triangles);
};
#else
// Forward declaration for when SEP_HAS_CYCLES is disabled
class CyclesRenderer;
#endif // SEP_HAS_CYCLES

} // namespace ccl
} // namespace blender
} // namespace sep
