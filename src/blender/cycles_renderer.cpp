// Standard includes
#include <cstring> // For std::memcpy, std::memset, std::memcmp, std::strlen etc.
#include <ctime>   // For C-style time functions
#include <string>  // For std::string
#include <cmath>
#include <memory>
#include <utility>
#include <vector>
#include <algorithm> // For std::max

// Define Cycles namespace macros
#define CCL_NAMESPACE_BEGIN namespace ccl {
#define CCL_NAMESPACE_END }

// Core Cycles includes
#include "util/system.h"
#include "util/types.h"

// SEP includes
#include "blender/cycles_renderer.h"
#include "core/error_handler.h"
#include "core/types.h"
#include "quantum/data.hpp"

// Cycles core includes
#include "device/device.h"
#include "scene/camera.h"
#include "scene/image.h"
#include "scene/mesh.h"
#include "scene/scene.h"
#include "session/output_driver.h"
#include "session/session.h"

// Cycles utility includes
#include "blender/oiio_output_driver.h"
#include "util/array.h"
#include "util/math_base.h"
#include "util/param.h"
#include "util/profiling.h"
#include "util/stats.h"
#include "util/string.h"
#include "util/texture.h"
#include "util/unique_ptr.h"
#include "util/vector.h"

namespace sep {
namespace blender {
namespace ccl {

SEPResult CyclesRenderer::initialize() {
    try {
        ::ccl::DeviceInfo device_info;
        cycles_device_ = ::ccl::Device::create(device_info,
                                             cycles_stats_,
                                             cycles_profiler_,
                                             true);
        if (!cycles_device_) {
            return SEPResult::INITIALIZATION_FAILED;
        }
        ::ccl::SceneParams scene_params;
        cycles_scene_ = ::ccl::make_unique<::ccl::Scene>(scene_params, cycles_device_.get());
        initialized_ = true;
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
        
        // Create scene if not already created
        if (!cycles_scene_) {
            ::ccl::SceneParams scene_params;
            cycles_scene_ = ::ccl::make_unique<::ccl::Scene>(scene_params, cycles_device_.get());
        }

        // Create camera
        ::ccl::Camera *cam = cycles_scene_->camera;
        if (!cam) {
            cam = new ::ccl::Camera();
            cycles_scene_->camera = cam;
        }

        // Create geometry from patterns
        for (const auto& pattern : patterns_) {
            createGeometryFromPattern(pattern);
        }
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
        width_ = params.width;
        height_ = params.height;
        last_params_ = params;
        if (!cycles_scene_) {
            return SEPResult::NOT_INITIALIZED;
        }

        // Configure camera
        ::ccl::Camera *cam = cycles_scene_->camera;
        if (cam) {
            cam->set_screen_size(params.width, params.height);
            cam->set_fov(45.0f * (M_PI_F / 180.0f));
            cam->set_matrix(::ccl::transform_identity());
            cam->set_use_perspective_motion(true);
            cam->set_shuttertime(0.0f);
            cam->set_rolling_shutter_type(::ccl::Camera::ROLLING_SHUTTER_NONE);
            cam->set_camera_type(::ccl::CAMERA_PERSPECTIVE);
            cam->set_use_spherical_stereo(false);
            cam->set_stereo_eye(::ccl::Camera::STEREO_NONE);
            cam->set_interocular_distance(0.0f);
            cam->set_convergence_distance(0.0f);
            cam->set_use_pole_merge(false);
            cam->set_pole_merge_angle_from(0.0f);
            cam->set_pole_merge_angle_to(0.0f);
            cam->set_sensorwidth(36.0f);
            cam->set_sensorheight(24.0f);
            cam->set_nearclip(0.1f);
            cam->set_farclip(100.0f);
            cam->set_aperturesize(0.0f);
            cam->set_blades(0);
            cam->set_bladesrotation(0.0f);
            cam->set_focaldistance(10.0f);
            /* API update: compute_viewplane() was replaced with
             * compute_auto_viewplane() in newer Cycles versions. */
            cam->compute_auto_viewplane();
        }

        // Configure render settings
        last_params_ = params;
        cycles_scene_->integrator->set_aa_samples(static_cast<int>(params.samples));
        cycles_scene_->integrator->set_use_denoise(params.use_denoising);
        cycles_scene_->params.background = true;
        cycles_scene_->params.use_bvh_spatial_split = true;
        cycles_scene_->params.use_bvh_unaligned_nodes = true;
        cycles_scene_->params.num_bvh_time_steps = 0;
        cycles_scene_->params.bvh_type = ::ccl::BVH_TYPE_DYNAMIC;
        cycles_scene_->params.bvh_layout = ::ccl::BVH_LAYOUT_BVH2;

        if (cycles_scene_->film) {
            cycles_scene_->film->set_filter_type(::ccl::FILTER_GAUSSIAN);
            cycles_scene_->film->set_filter_width(1.5f);
            cycles_scene_->film->set_exposure(1.0f);
        }

        if (cycles_scene_->background) {
            cycles_scene_->background->set_transparent(false);
            cycles_scene_->background->set_transparent_glass(false);
            cycles_scene_->background->set_transparent_roughness_threshold(0.1f);
        }
        return SEPResult::SUCCESS;
    } catch (const std::exception& e) {
        return SEPResult::PROCESSING_ERROR;
    }
}

bool CyclesRenderer::render(const std::string& filepath) {
    if (!initialized_ || patterns_.empty() || !cycles_scene_) {
        return false;
    }

    // Initialize session
    ::ccl::SessionParams session_params;
    session_params.background = true;
    session_params.threads = 0; // Auto-detect thread count
    session_params.samples = static_cast<int>(last_params_.samples);
    
    auto session = std::make_unique<::ccl::Session>(session_params, cycles_scene_->params);
    session->scene = std::move(cycles_scene_);

    session->set_output_driver(::ccl::make_unique<::ccl::OIIOOutputDriver>(
        filepath.c_str(), "Combined", [](const ::ccl::string &msg) {
        (void)msg;
    }));

    // Start render
    session->start();
    session->wait();

    session.reset();
    return true;
}

void CyclesRenderer::createGeometryFromPattern(const pattern::PatternData& pattern) {
    // Create mesh
    auto mesh = std::make_unique<::ccl::Mesh>();
    
    // Convert pattern data to vertices and faces
    std::vector<::ccl::float3> verts;
    std::vector<::ccl::int3> triangles;
    convertPatternToMesh(pattern, verts, triangles);

    // Add vertices and faces to mesh
    ::ccl::vector<::ccl::float3> verts_vec(verts.begin(), verts.end());
    ::ccl::array<::ccl::float3> verts_array;
    verts_array = verts_vec;

    ::ccl::vector<int> tri_flat;
    tri_flat.reserve(triangles.size() * 3);
    for (const auto &t : triangles) {
        tri_flat.push_back(t.x);
        tri_flat.push_back(t.y);
        tri_flat.push_back(t.z);
    }
    ::ccl::array<int> tri_array;
    tri_array = tri_flat;

    mesh->set_verts(verts_array);
    mesh->set_triangles(tri_array);
    mesh->attributes.add(::ccl::ATTR_STD_UV, ::ccl::ustring("uvmap"));
    
    // Add mesh to scene
    cycles_scene_->geometry.push_back(std::move(mesh));
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

} // namespace ccl
} // namespace blender
} // namespace sep
