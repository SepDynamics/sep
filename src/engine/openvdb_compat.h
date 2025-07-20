#pragma once

// This header provides compatibility for OpenVDB and NanoVDB types
// It defines the necessary types for compilation

#ifdef __cplusplus
// Include standard headers
#include <memory>
#include <functional> // For std::hash

// Forward declarations for OpenVDB types
namespace openvdb {
    // Define Coord in openvdb namespace to match system headers
    class Coord {
    public:
        Coord() : mX(0), mY(0), mZ(0) {}
        Coord(int x, int y, int z) : mX(x), mY(y), mZ(z) {}
        int x() const { return mX; }
        int y() const { return mY; }
        int z() const { return mZ; }
        
        // Add min method for CoordBBox
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
        
        // Forward declaration to redirect openvdb::math::Coord to openvdb::Coord
        // This helps with system headers that expect Coord in the math namespace
        using Coord = openvdb::Coord;
    }
    
    class GridBase {
    public:
        // Use shared_ptr for ConstPtr to support reset()
        typedef GridBase* Ptr;
        typedef std::shared_ptr<const GridBase> ConstPtr;
        
        // Add missing methods
        math::CoordBBox evalActiveVoxelBoundingBox() const {
            return math::CoordBBox();
        }
        
        class Transform {
        public:
            class BaseMap {
            public:
                typedef BaseMap* Ptr;
                class AffineMap {
                public:
                    typedef AffineMap* Ptr;
                    math::Mat4f getMat4() const {
                        return math::Mat4f();
                    }
                };
                AffineMap::Ptr getAffineMap() const { return nullptr; }
            };
            BaseMap::Ptr baseMap() const { return nullptr; }
        };
        
        Transform transform() const { return Transform(); }
    };
    
    // Add grid_type_operation function
    template<typename OpType>
    bool grid_type_operation(const GridBase::ConstPtr& grid, OpType& op) {
        // Call the non-template version of operator()
        return op(grid);
    }
}

// Forward declarations for NanoVDB types
namespace nanovdb {
    // Minimal implementation of GridHandle
    template<typename BufferT = std::shared_ptr<void>>
    class GridHandle {
    private:
        bool mValid = false;
        unsigned long mSize = 0;
        void* mData = nullptr;
    
    public:
        GridHandle() : mValid(false), mSize(0), mData(nullptr) {}
        
        // Allow implicit conversion to bool for validity checks
        operator bool() const { return mValid; }
        
        template<typename ValueT>
        const ValueT* grid() const { return nullptr; }
        
        bool isValid() const { return mValid; }
        
        // Data access methods
        void* data() const { return mData; }
        unsigned long size() const { return mSize; }
        
        // Reset method
        void reset() { mValid = false; mSize = 0; mData = nullptr; }
    };
}

/* Force disable NanoVDB functionality to avoid compilation issues */
#undef WITH_NANOVDB

// Add hash specialization for openvdb::math::Coord to match the system headers
// This is needed because the system headers expect openvdb::math::Coord
namespace std {
    template<>
    struct hash<openvdb::math::Coord> {
        size_t operator()(const openvdb::math::Coord& coord) const {
            // Simple hash function based on the coordinates
            size_t seed = 0;
            seed ^= std::hash<int>()(coord.x()) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            seed ^= std::hash<int>()(coord.y()) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            seed ^= std::hash<int>()(coord.z()) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            return seed;
        }
    };
}

#endif // __cplusplus