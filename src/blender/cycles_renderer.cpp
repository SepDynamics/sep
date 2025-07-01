#include "blender/cycles_renderer.h"
#include <memory>
#include "core/error_handler.h"
#include "core/types.h"
#include "quantum/data.hpp"
#ifdef SEP_HAS_CYCLES
#include "device/device.h"
#include "scene/camera.h"
#include "scene/mesh.h"
#include "session/session.h"
#include "util/math_base.h"
#include "util/texture.h"
#endif

#ifdef SEP_HAS_CYCLES
#  include <cmath>
#  include <memory>
#  include "util/math_base.h"
#  include "scene/camera.h"
#  include "scene/mesh.h"
#  include "scene/scene.h"
#  include "session/session.h"
#  include "util/stats.h"
#  include "util/profiling.h"
#  include "device/device.h"
#  include "scene/image.h"
#  include "util/vector.h"
#  include "util/array.h"
#  include "util/string.h"
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
#ifdef SEP_HAS_CYCLES
        ::ccl::DeviceInfo device_info;
        cycles_device_ = ::ccl::Device::create(device_info,
                                               cycles_stats_,
                                               cycles_profiler_,
                                               true);
        if (!cycles_device_) {
            return SEPResult::INITIALIZATION_FAILED;
        }
        ::ccl::SceneParams scene_params;
        cycles_scene_ = new ::ccl::Scene(scene_params, cycles_device_.get());
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
        width_ = params.width;
        height_ = params.height;
        if (!cycles_scene_) {
            return SEPResult::NOT_INITIALIZED;
        }

        // Configure camera
        ::ccl::Camera *cam = cycles_scene_->camera;
        if (cam) {
            cam->set_screen_size(params.width, params.height);
            cam->set_fov(45.0f * (M_PI_F / 180.0f));
        }

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
    session_params.background = true;
    session_params.threads = 0; // Auto-detect thread count

    ::ccl::Session *session = new ::ccl::Session(session_params, cycles_scene_->params);
    session->scene = cycles_scene_;

    // Start render
    session->start();
    session->wait();

    // Save render result
    ::ccl::ImageFormat format;
    format.width = cycles_scene_->camera->get_full_width();
    format.height = cycles_scene_->camera->get_full_height();
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
    ::ccl::vector<::ccl::float3> verts_vec(verts.begin(), verts.end());
    ::ccl::vector<::ccl::int3> tris_vec(triangles.begin(), triangles.end());
    ::ccl::array<::ccl::float3> verts_array;
    ::ccl::array<::ccl::int3> triangles_array;
    verts_array = verts_vec;
    triangles_array = tris_vec;
    mesh->set_verts(verts_array);
    mesh->set_triangles(triangles_array);
    mesh->attributes.add(::ccl::ATTR_STD_UV, ::ccl::ustring("uvmap"));
    
    // Add mesh to scene
    cycles_scene_->geometry.push_back(std::unique_ptr<::ccl::Geometry>(mesh));
}

void CyclesRenderer::convertPatternToMesh(const pattern::PatternData& pattern,
                                        std::vector<::ccl::float3>& verts,
                                        std::vector<::ccl::int3>& triangles) {
    // Convert pattern data into mesh vertices and triangles
    // This is a simple example - you'll want to implement your own conversion logic
    float scale = 0.1f;
    ::ccl::float3 base = ::ccl::make_float3(pattern.position.x,
                                            pattern.position.y,
                                            pattern.position.z);
    verts.push_back(base);
    verts.push_back(::ccl::make_float3(base.x + scale, base.y, base.z));
    verts.push_back(::ccl::make_float3(base.x, base.y + scale, base.z));
    triangles.push_back(::ccl::make_int3(0, 1, 2));
}
#endif

} // namespace ccl
} // namespace blender
} // namespace sep
