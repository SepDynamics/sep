#pragma once

// Forward declarations and includes
#include "macros.h"

// This header provides compatibility for Cycles integration
// When SEP_HAS_CYCLES is defined, the actual Cycles headers should be included
// Otherwise, we provide stub implementations for development and testing

// Define stub types for Cycles
namespace ccl {
    class SceneParams {
    public:
        bool background = true;
    };
    
    class Scene {
    public:
        Scene(const SceneParams& params) {}
        ~Scene() {}
    };
    
    class SessionParams {
    public:
        bool background = true;
        bool progressive_refine = true;
        bool use_auto_tile = true;
        int tile_size = 64;
    };
    
    class Session {
    public:
        Session(const SessionParams& params) {}
        Session(const SessionParams& params, const SceneParams& scene_params) {}
        ~Session() {}
        
        void set_scene(Scene* scene) {}
        void start() {}
        void wait() {}
        void cancel() {}
    };
    
    class BufferParams {
    public:
        int width = 0;
        int height = 0;
    };
}

// Note: We don't use 'using namespace ccl' here to avoid namespace pollution
// Instead, we use explicit ccl:: prefixes in the code