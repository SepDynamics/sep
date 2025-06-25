#pragma once

// Forward declarations and includes
#include "macros.h"

#if SEP_HAS_CYCLES
// Use the actual Cycles headers
#include "/sep/cycles/src/scene/scene.h"
#include "/sep/cycles/src/session/session.h"
#include "/sep/cycles/src/util/util_param.h"
#include "/sep/cycles/src/util/util_path.h"
#include "/sep/cycles/src/util/util_logging.h"
#include "/sep/cycles/src/render/film.h"
#include "/sep/cycles/src/render/integrator.h"
#include "/sep/cycles/src/render/camera.h"
#include "/sep/cycles/src/render/mesh.h"
#include "/sep/cycles/src/render/object.h"
#include "/sep/cycles/src/render/light.h"

// Use the Cycles namespace
using namespace ccl;
#else
// Define stub types for Cycles when not available

namespace ccl {

// Minimal SceneParams class
class SceneParams {
public:
    bool background = true;
};

// Minimal Scene class
class Scene {
public:
    Scene(const SceneParams& params) {}
    ~Scene() {}
};

// Minimal SessionParams class
class SessionParams {
public:
    bool background = true;
    bool progressive_refine = true;
};

// Minimal Session class
class Session {
public:
    Session(const SessionParams& params) {}
    ~Session() {}
    
    void set_scene(Scene* scene) {}
    void start() {}
    void wait() {}
    void cancel() {}
};

} // namespace ccl

#endif // SEP_HAS_CYCLES