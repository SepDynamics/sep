#pragma once
#include <string>
#include <vector>
#include "quantum/data.hpp"
#include "core/types.h"
#include "core/common.h"
#include "blender/cycles_compat.h"
#include <memory>

namespace sep {
namespace blender {
namespace ccl {


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
    bool initialized_{false};
    std::vector<pattern::PatternData> patterns_;
    ::ccl::Scene* cycles_scene_{nullptr};
#ifdef SEP_HAS_CYCLES
    ::ccl::Stats cycles_stats_;
    ::ccl::Profiler cycles_profiler_;
    std::unique_ptr<::ccl::Device> cycles_device_;
#endif

#ifdef SEP_HAS_CYCLES
    void createGeometryFromPattern(const pattern::PatternData& pattern);
    void convertPatternToMesh(const pattern::PatternData& pattern,
                             std::vector<::ccl::float3>& verts,
                             std::vector<::ccl::int3>& triangles);
#endif
};

} // namespace ccl
} // namespace blender
} // namespace sep
