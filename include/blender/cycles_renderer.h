#pragma once
#include <string>
#include <vector>
#include <memory>
#include "quantum/data.hpp"
#include "core/types.h"
#include "core/common.h"
#include "compat/cycles.h"

// Ensure SEP_HAS_CYCLES has a sensible default if the build system did not
// define it. This keeps conditional compilation sections well-formed.
#ifndef SEP_HAS_CYCLES
#define SEP_HAS_CYCLES 0
#endif

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
#ifdef SEP_HAS_CYCLES
    using ScenePtr = std::unique_ptr<::ccl::Scene>;
#endif

    bool initialized_{false};
    int width_{0};
    int height_{0};
    std::vector<pattern::PatternData> patterns_;
#ifdef SEP_HAS_CYCLES
    ScenePtr cycles_scene_{nullptr};
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
