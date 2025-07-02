#pragma once

// This header provides compatibility for Cycles integration
// It defines the CCL_NAMESPACE_BEGIN and CCL_NAMESPACE_END macros
// that are used throughout the Cycles codebase

#ifdef __cplusplus
// Include standard headers
#include <memory>

// Define CCL namespace macros for compatibility
// These are also defined at the compiler level with -D flags
#ifndef CCL_NAMESPACE_BEGIN
#define CCL_NAMESPACE_BEGIN namespace ccl {
#endif

#ifndef CCL_NAMESPACE_END
#define CCL_NAMESPACE_END }
#endif

#ifndef CCL_NAMESPACE_USING_DIRECTIVE
#define CCL_NAMESPACE_USING_DIRECTIVE using namespace ccl;
#endif

// Core Cycles headers - only include if SEP_HAS_CYCLES is defined
#if defined(SEP_HAS_CYCLES) && SEP_HAS_CYCLES
// Core Cycles headers
#include "device/device.h"
#include "scene/scene.h"
#include "scene/camera.h"
#include "scene/mesh.h"
#include "scene/shader.h"
#include "scene/light.h"
#include "scene/background.h"
#include "scene/film.h"
#include "scene/integrator.h"
#include "session/session.h"
#include "session/buffers.h"

// Utility headers
#include "util/array.h"
#include "util/math_base.h"
#include "util/param.h"
#include "util/string.h"
#include "util/vector.h"

// No need for stub implementations when using real Cycles
#else
// Using stub implementations either because SEP_HAS_CYCLES is not defined
// or because SEP_USE_CYCLES_STUB is explicitly set to 1
// Define OpenVDB types needed for compilation when Cycles is not available
namespace openvdb {
    class GridBase {
    public:
        typedef GridBase* Ptr;
        typedef std::shared_ptr<const GridBase> ConstPtr;
    };
    
    // Define Coord in openvdb namespace to match system headers
    class Coord {
    public:
        Coord() : mX(0), mY(0), mZ(0) {}
        Coord(int x, int y, int z) : mX(x), mY(y), mZ(z) {}
        int x() const { return mX; }
        int y() const { return mY; }
        int z() const { return mZ; }
        Coord min() const { return *this; }
    private:
        int mX, mY, mZ;
    };
    
    namespace math {
        // Forward declare Mat4f
        class Mat4f {
        public:
            Mat4f() {}
            
            // Support 2D array access
            float operator()(int row, int col) const { return 0.0f; }
            
            // Support 1D array access
            float operator[](int i) const { return 0.0f; }
            float& operator[](int i) { static float dummy = 0.0f; return dummy; }
        };
        
        // Complete definition for CoordBBox
        class CoordBBox {
        public:
            CoordBBox() {}
            CoordBBox(const Coord& min, const Coord& max) {}
            
            // Add assignment operator
            CoordBBox& operator=(const CoordBBox& other) { return *this; }
            
            bool empty() const { return true; }
            Coord dim() const { return Coord(); }
            Coord min() const { return Coord(); }
        };
    }
}

// Define NanoVDB types needed for compilation
namespace nanovdb {
    // Forward declarations
    template<typename T> class Vec3;
    template<typename T> class BBox;
    template<typename T> class CoordBBox;
    template<typename T> class NanoGrid;
    
    // Minimal implementation of GridHandle
    template<typename BufferT = std::shared_ptr<void>>
    class GridHandle {
    private:
        bool mValid = false;
        size_t mSize = 0;
        void* mData = nullptr;
    
    public:
        GridHandle() : mValid(false), mSize(0), mData(nullptr) {}
        
        // Allow implicit conversion to bool for validity checks
        operator bool() const { return mValid; }
        
        template<typename ValueT>
        const NanoGrid<ValueT>* grid() const { return nullptr; }
        
        bool isValid() const { return mValid; }
        
        // Data access methods
        void* data() const { return mData; }
        size_t size() const { return mSize; }
        
        // Reset method
        void reset() { mValid = false; mSize = 0; mData = nullptr; }
    };
}

// Define stub types for Cycles when real Cycles is not available
namespace ccl {
    // Forward declarations for common Cycles types
    class ImageLoader;
    class ImageDeviceFeatures;
    struct ImageMetaData;
    struct Transform;
    
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

    /* Minimal profiler and statistics stubs used when Cycles is not available. */
    class Profiler {
    public:
        Profiler() = default;
        ~Profiler() = default;

        void reset(int /*num_shaders*/, int /*num_objects*/) {}
        void start() {}
        void stop() {}
    };

    class Stats {
    public:
        size_t mem_used = 0;
        size_t mem_peak = 0;

        Stats() = default;
        explicit Stats(int /*static_init*/) {}

        void mem_alloc(size_t /*size*/) {}
        void mem_free(size_t /*size*/) {}
    };
}
#endif // SEP_HAS_CYCLES

// Note: We don't use 'using namespace ccl' here to avoid namespace pollution
// Instead, we use explicit ccl:: prefixes in the code

#endif // __cplusplus
