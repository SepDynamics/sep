#pragma once
#include <string>
#include <vector>
#include "quantum/data.hpp"
#include "core/types.h"
#include "core/common.h"

namespace sep {
namespace blender {

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
};

} // namespace blender
} // namespace sep
