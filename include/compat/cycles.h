#pragma once

// This header provides compatibility for Cycles integration
// It defines the CCL_NAMESPACE_BEGIN and CCL_NAMESPACE_END macros
// that are used throughout the Cycles codebase

#ifdef __cplusplus
// Include C++ Standard Library headers FIRST
#include <cmath>
#include <cstring>
#include <memory>
#include <string>
#include <vector>

// Define CCL namespace macros for compatibility
#ifndef CCL_NAMESPACE_BEGIN
#define CCL_NAMESPACE_BEGIN namespace ccl {
#endif

#ifndef CCL_NAMESPACE_END
#define CCL_NAMESPACE_END }
#endif

#ifndef CCL_NAMESPACE_USING_DIRECTIVE
#define CCL_NAMESPACE_USING_DIRECTIVE using namespace ccl;
#endif

// Core Cycles headers - using device-relative paths
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

// Utility headers - using device-relative paths
#include "util/array.h"
#include "util/math_base.h"
#include "util/param.h"
#include "util/string.h"
#include "util/vector.h"

// OpenVDB types needed for compilation
namespace openvdb {
    class GridBase {
    public:
        typedef GridBase* Ptr;
        typedef std::shared_ptr<const GridBase> ConstPtr;
    };
    
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
        class Mat4f {
        public:
            Mat4f() {}
            float operator()(int row, int col) const { return 0.0f; }
            float operator[](int i) const { return 0.0f; }
            float& operator[](int i) { static float dummy = 0.0f; return dummy; }
        };
        
        class CoordBBox {
        public:
            CoordBBox() {}
            CoordBBox(const Coord& min, const Coord& max) {}
            CoordBBox& operator=(const CoordBBox& other) { return *this; }
            bool empty() const { return true; }
            Coord dim() const { return Coord(); }
            Coord min() const { return Coord(); }
        };
    }
}

// Note: We use explicit ccl:: prefixes in the code to avoid namespace pollution

#endif // __cplusplus
