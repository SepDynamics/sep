#include "blender/cycles_renderer.h"
#include "core/error_handler.h"
#include "core/types.h"
#include "quantum/data.hpp"
#ifdef SEP_HAS_CYCLES
#  include "scene/camera.h"
#  include "scene/mesh.h"
#  include "session/session.h"
#  include "session/session_params.h"
#  include "util/stats.h"
#  include "util/profiling.h"
#endif

namespace sep {
namespace blender {
namespace ccl {

SEPResult CyclesRenderer::isCyclesAvailable() {
#ifdef SEP_HAS_CYCLES
    return SEPResult::SUCCESS;
#else
    return SEPResult::FEATURE_UNAVAILABLE;
#endif
}

SEPResult CyclesRenderer::initialize() {
    if (isCyclesAvailable() != SEPResult::SUCCESS) {
        return SEPResult::FEATURE_UNAVAILABLE;
    }
    try {
        initialized_ = true;
#ifdef SEP_HAS_CYCLES
        ::ccl::SceneParams scene_params;
        ::ccl::Stats stats;
        ::ccl::Profiler profiler;
        ::ccl::DeviceInfo device_info = ::ccl::Device::dummy_device();
        device_ = ::ccl::Device::create(device_info, stats, profiler, true);
        cycles_scene_ = new ::ccl::Scene(scene_params, device_.get());
#endif
        return SEPResult::SUCCESS;
    } catch (const std::exception& e) {
        return SEPResult::INITIALIZATION_FAILED;
    }
}

SEPResult CyclesRenderer::createSceneFromPatterns(const std::vector<pattern::PatternData>& patterns) {
    if (!initialized_) {
        return SEPResult::NOT_INITIALIZED;
    }
    if (patterns.empty()) {
        return SEPResult::INVALID_ARGUMENT;
    }
    try {
        patterns_ = patterns;
        return SEPResult::SUCCESS;
    } catch (const std::exception& e) {
        return SEPResult::PROCESSING_ERROR;
    }
}

SEPResult CyclesRenderer::renderScene(const RenderParams& params) {
    if (!initialized_) {
        return SEPResult::NOT_INITIALIZED;
    }
    if (patterns_.empty()) {
        return SEPResult::INVALID_ARGUMENT;
    }
    if (params.width <= 0 || params.height <= 0) {
        return SEPResult::INVALID_ARGUMENT;
    }
    try {
#ifdef SEP_HAS_CYCLES
        if (!cycles_scene_) {
            return SEPResult::NOT_INITIALIZED;
        }

        last_render_params_ = params;

        // Create camera
        ::ccl::Camera *cam = new ::ccl::Camera();
        cam->set_screen_size(params.width, params.height);
        cam->fov = 45.0f;
        cycles_scene_->camera = cam;

        // Create geometry from patterns
        for (const auto& pattern : patterns_) {
            createGeometryFromPattern(pattern);
        }
#endif
        return SEPResult::SUCCESS;
    } catch (const std::exception& e) {
        return SEPResult::PROCESSING_ERROR;
    }
}

bool CyclesRenderer::render(const std::string& filepath) {
    if (!initialized_ || patterns_.empty() || !cycles_scene_) {
        return false;
    }

#ifdef SEP_HAS_CYCLES
    // Initialize session
    ::ccl::SessionParams session_params;
    session_params.progressive = true;
    session_params.background = true;
    session_params.threads = 0; // Auto-detect thread count
    session_params.samples = static_cast<int>(last_render_params_.samples);

    ::ccl::Session *session = new ::ccl::Session(session_params, cycles_scene_->params);
    session->scene = cycles_scene_;

    // Start render
    session->start();
    session->wait();

    // Save render result
    ::ccl::ImageFormat format;
    format.width = last_render_params_.width;
    format.height = last_render_params_.height;
    format.type = ::ccl::IMAGE_DATA_TYPE_FLOAT;
    format.channels = 4;

    session->write_render_tile(filepath.c_str(), &format);

    delete session;
    return true;
#else
    (void)filepath;
    return false;
#endif
}

#ifdef SEP_HAS_CYCLES
void CyclesRenderer::createGeometryFromPattern(const pattern::PatternData& pattern) {
    // Create mesh
    ::ccl::Mesh *mesh = new ::ccl::Mesh();
    
    // Convert pattern data to vertices and faces
    std::vector<::ccl::float3> verts;
    std::vector<::ccl::int3> triangles;
    convertPatternToMesh(pattern, verts, triangles);
    
    // Add vertices and faces to mesh
    mesh->verts = verts;
    mesh->triangles = triangles;
    mesh->attributes.add(::ccl::ATTR_STD_UV, "uvmap");
    
    // Add mesh to scene
    cycles_scene_->geometry.push_back(std::unique_ptr<::ccl::Geometry>(mesh));
}

void CyclesRenderer::convertPatternToMesh(const pattern::PatternData& pattern,
                                        std::vector<::ccl::float3>& verts,
                                        std::vector<::ccl::int3>& triangles) {
    // Convert pattern data into mesh vertices and triangles
    // This is a simple example - you'll want to implement your own conversion logic
    float scale = 0.1f;
    float z = scale * pattern.amplitude.real();
    const size_t grid = 10;
    for (size_t i = 0; i < grid * grid; ++i) {
        float x = scale * static_cast<float>(i % grid);
        float y = scale * static_cast<float>(i / grid);
        verts.push_back(::ccl::make_float3(x, y, z));
    }

    // Create triangles from vertices
    for (size_t i = 0; i < verts.size() - grid - 1; i++) {
        if ((i + 1) % grid != 0) {
            triangles.push_back(::ccl::make_int3(i, i + 1, i + grid));
            triangles.push_back(::ccl::make_int3(i + 1, i + grid + 1, i + grid));
        }
    }
}
#endif

} // namespace ccl
} // namespace blender
} // namespace sep
