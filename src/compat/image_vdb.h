/* SPDX-FileCopyrightText: 2011-2022 Blender Foundation
 *
 * SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include <string>

#include "compat/image.h"
#include "compat/transform.h"
#include "compat/texture.h"

#ifdef WITH_OPENVDB
#  include <openvdb/openvdb.h>
#endif

#ifdef WITH_NANOVDB
#  include <nanovdb/NanoVDB.h>
#endif

CCL_NAMESPACE_BEGIN

/* Forward declarations for OpenVDB types when not available */
#ifndef WITH_OPENVDB
namespace openvdb {
    class GridBase {
    public:
        typedef GridBase* Ptr;
        typedef const GridBase* ConstPtr;
    };
    
    class Coord {
    public:
        Coord() {}
        int x() const { return 0; }
        int y() const { return 0; }
        int z() const { return 0; }
    };
    
    namespace math {
        class CoordBBox {
        public:
            CoordBBox() {}
            bool empty() const { return true; }
            Coord dim() const { return Coord(); }
            Coord min() const { return Coord(); }
        };
    }
}
#endif

/* VDB Image Loader */
class VDBImageLoader : public ImageLoader {
public:
    VDBImageLoader(const std::string &grid_name);
#ifdef WITH_OPENVDB
    VDBImageLoader(::openvdb::GridBase::ConstPtr grid_, const std::string &grid_name);
    ::openvdb::GridBase::ConstPtr get_grid();
#endif
    ~VDBImageLoader();

    bool load_metadata(const ImageDeviceFeatures &features, ImageMetaData &metadata);
    bool load_pixels(const ImageMetaData &metadata, 
                    void *pixels, 
                    const size_t pixels_size, 
                    const bool associate_alpha);
    std::string name() const;
    bool equals(const ImageLoader &other) const;
    void cleanup();
    bool is_vdb_loader() const;

private:
    std::string grid_name;
#ifdef WITH_OPENVDB
    ::openvdb::GridBase::ConstPtr grid;
    void* bbox_ptr = nullptr;  // Will hold openvdb::math::CoordBBox
#endif
#ifdef WITH_NANOVDB
    int precision = 0;
    void* nanogrid_ptr = nullptr;  // Will hold nanovdb::GridHandle<>
#endif
};

CCL_NAMESPACE_END
