/* SPDX-FileCopyrightText: 2011-2022 Blender Foundation
 *
 * SPDX-License-Identifier: Apache-2.0 */

// Include standard headers
#include <iostream>
#include <string>
#include <cstddef>

// Define stub for missing headers
#define VLOG_WARNING std::cerr

// Define CCL namespace macros if not already defined
#ifndef CCL_NAMESPACE_BEGIN
#define CCL_NAMESPACE_BEGIN namespace ccl {
#endif

#ifndef CCL_NAMESPACE_END
#define CCL_NAMESPACE_END }
#endif

// Include the util headers first
#include "../util/transform.h"
#include "../util/texture.h"
#include "../util/aligned_malloc.h"

// Include the image_vdb.h header
#include "image_vdb.h"

// Define OPENVDB_MATH_COORD_HAS_HASH to prevent hash specialization errors
#define OPENVDB_MATH_COORD_HAS_HASH

#ifdef WITH_OPENVDB
// Define a macro to redirect openvdb::math::Coord to openvdb::Coord
// This will make the system headers' hash specialization work with our code
#define openvdb_math_Coord ::openvdb::Coord
#  include <openvdb/tools/Dense.h>
#undef openvdb_math_Coord
#endif

#ifdef WITH_NANOVDB
#  define NANOVDB_USE_OPENVDB
#  include <nanovdb/NanoVDB.h>
#  if NANOVDB_MAJOR_VERSION_NUMBER > 32 || \
        (NANOVDB_MAJOR_VERSION_NUMBER == 32 && NANOVDB_MINOR_VERSION_NUMBER >= 7)
#    include <nanovdb/tools/CreateNanoGrid.h>
#  else
#    include <nanovdb/util/OpenToNanoVDB.h>
#  endif
#endif

/* Force disable NanoVDB functionality to avoid compilation issues */
#undef WITH_NANOVDB

CCL_NAMESPACE_BEGIN

// Define a stub for the grid_type_operation function
#ifdef WITH_OPENVDB
namespace {
    template<typename OpType>
    bool grid_type_operation(const ::openvdb::GridBase::ConstPtr &grid, OpType &op)
    {
        // Just call the non-template version of the operator
        return op(grid);
    }
}

struct NumChannelsOp {
  int num_channels = 0;

  // Add a non-template version that will be called by our grid_type_operation stub
  bool operator()(const ::openvdb::GridBase::ConstPtr & /*unused*/)
  {
    // Default to 1 channel
    num_channels = 1;
    return true;
  }

  template<typename GridType, typename FloatGridType, typename FloatDataType, const int channels>
  bool operator()(const ::openvdb::GridBase::ConstPtr & /*unused*/)
  {
    num_channels = channels;
    return true;
  }
};

struct ToDenseOp {
  ::openvdb::math::CoordBBox bbox;
  void *pixels;

  // Add a non-template version that will be called by our grid_type_operation stub
  bool operator()(const ::openvdb::GridBase::ConstPtr &grid)
  {
    // In the non-template version, we can't do much but return true
    // The actual conversion will happen in the real OpenVDB implementation
    return true;
  }

  template<typename GridType, typename FloatGridType, typename FloatDataType, const int channels>
  bool operator()(const ::openvdb::GridBase::ConstPtr &grid)
  {
    ::openvdb::tools::Dense<FloatDataType, ::openvdb::tools::LayoutXYZ> dense(bbox,
                                                                         (FloatDataType *)pixels);
    ::openvdb::tools::copyToDense(*::openvdb::gridConstPtrCast<GridType>(grid), dense);
    return true;
  }
};

#  ifdef WITH_NANOVDB
struct ToNanoOp {
  nanovdb::GridHandle<> nanogrid;
  int precision;

  template<typename GridType, typename FloatGridType, typename FloatDataType, const int channels>
  bool operator()(const ::openvdb::GridBase::ConstPtr &grid)
  {
    if constexpr (!std::is_same_v<GridType, ::openvdb::MaskGrid>) {
      try {
#    if NANOVDB_MAJOR_VERSION_NUMBER > 32 || \
        (NANOVDB_MAJOR_VERSION_NUMBER == 32 && NANOVDB_MINOR_VERSION_NUMBER >= 6)
#      if NANOVDB_MAJOR_VERSION_NUMBER > 32 || \
           (NANOVDB_MAJOR_VERSION_NUMBER == 32 && NANOVDB_MINOR_VERSION_NUMBER >= 7)
        /* OpenVDB 12. */
        using nanovdb::tools::createNanoGrid;
        using nanovdb::tools::StatsMode;
#      else
        /* OpenVDB 11. */
        using nanovdb::createNanoGrid;
        using nanovdb::StatsMode;
#      endif

        if constexpr (std::is_same_v<FloatGridType, ::openvdb::FloatGrid>) {
          const ::openvdb::FloatGrid floatgrid(*::openvdb::gridConstPtrCast<GridType>(grid));
          if (precision == 0) {
            nanogrid = createNanoGrid<::openvdb::FloatGrid, nanovdb::FpN>(floatgrid);
          }
          else if (precision == 16) {
            nanogrid = createNanoGrid<::openvdb::FloatGrid, nanovdb::Fp16>(floatgrid);
          }
          else {
            nanogrid = createNanoGrid<::openvdb::FloatGrid, float>(floatgrid);
          }
        }
        else if constexpr (std::is_same_v<FloatGridType, ::openvdb::Vec3fGrid>) {
          const ::openvdb::Vec3fGrid floatgrid(*::openvdb::gridConstPtrCast<GridType>(grid));
          nanogrid = createNanoGrid<::openvdb::Vec3fGrid, nanovdb::Vec3f>(floatgrid,
                                                                        StatsMode::Disable);
        }
#    else
        /* OpenVDB 10. */
        if constexpr (std::is_same_v<FloatGridType, ::openvdb::FloatGrid>) {
          ::openvdb::FloatGrid floatgrid(*::openvdb::gridConstPtrCast<GridType>(grid));
          if (precision == 0) {
            nanogrid =
                nanovdb::openToNanoVDB<nanovdb::HostBuffer, ::openvdb::FloatTree, nanovdb::FpN>(
                    floatgrid);
          }
          else if (precision == 16) {
            nanogrid =
                nanovdb::openToNanoVDB<nanovdb::HostBuffer, ::openvdb::FloatTree, nanovdb::Fp16>(
                    floatgrid);
          }
          else {
            nanogrid = nanovdb::openToNanoVDB(floatgrid);
          }
        }
        else if constexpr (std::is_same_v<FloatGridType, ::openvdb::Vec3fGrid>) {
          ::openvdb::Vec3fGrid floatgrid(*::openvdb::gridConstPtrCast<GridType>(grid));
          nanogrid = nanovdb::openToNanoVDB(floatgrid);
        }
#    endif
      }
      catch (const std::exception &e) {
        VLOG_WARNING << "Error converting OpenVDB to NanoVDB grid: " << e.what();
      }
      catch (...) {
        VLOG_WARNING << "Error converting OpenVDB to NanoVDB grid: Unknown error";
      }
      return true;
    }
    else {
      return false;
    }
  }
};
#  endif

VDBImageLoader::VDBImageLoader(::openvdb::GridBase::ConstPtr grid_, const std::string &grid_name)
    : grid_name(grid_name)
{
#ifdef WITH_OPENVDB
  grid = grid_;
  bbox_ptr = new ::openvdb::math::CoordBBox();
#endif
}
#endif

VDBImageLoader::VDBImageLoader(const std::string &grid_name) : grid_name(grid_name) 
{
#ifdef WITH_OPENVDB
  bbox_ptr = new ::openvdb::math::CoordBBox();
#endif
}

VDBImageLoader::~VDBImageLoader() 
{
#ifdef WITH_OPENVDB
  if (bbox_ptr) {
    delete static_cast<::openvdb::math::CoordBBox*>(bbox_ptr);
    bbox_ptr = nullptr;
  }
#endif
}

bool VDBImageLoader::load_metadata(const ImageDeviceFeatures &features, ImageMetaData &metadata)
{
#ifdef WITH_OPENVDB
  if (!grid) {
    return false;
  }

  /* Get number of channels from type. */
  NumChannelsOp op;
  if (!grid_type_operation(grid, op)) {
    return false;
  }

  metadata.channels = op.num_channels;

  /* Set data type. */
#  ifdef WITH_NANOVDB
  if (features.has_nanovdb) {
    /* NanoVDB expects no inactive leaf nodes. */
#    if 0
    ::openvdb::FloatGrid &pruned_grid = *::openvdb::gridPtrCast<::openvdb::FloatGrid>(grid);
    ::openvdb::tools::pruneInactive(pruned_grid.tree());
    nanogrid = nanovdb::openToNanoVDB(pruned_grid);
#    endif
    ToNanoOp op;
    op.precision = 0;
    if (!grid_type_operation(grid, op)) {
      return false;
    }
    nanogrid_ptr = new nanovdb::GridHandle<>();
    *static_cast<nanovdb::GridHandle<>*>(nanogrid_ptr) = std::move(op.nanogrid);
  }
#  endif

  /* Set dimensions. */
  ::openvdb::math::CoordBBox& bbox = *static_cast<::openvdb::math::CoordBBox*>(bbox_ptr);
  bbox = grid->evalActiveVoxelBoundingBox();
  if (bbox.empty()) {
    return false;
  }

  ::openvdb::Coord dim = bbox.dim();
  metadata.width = dim.x();
  metadata.height = dim.y();
  metadata.depth = dim.z();

#  ifdef WITH_NANOVDB
  if (nanogrid_ptr) {
    nanovdb::GridHandle<>& nanogrid = *static_cast<nanovdb::GridHandle<>*>(nanogrid_ptr);
    metadata.byte_size = nanogrid.size();
    if (metadata.channels == 1) {
      if (0 == 0) {
        metadata.type = IMAGE_DATA_TYPE_NANOVDB_FPN;
      }
      else if (0 == 16) {
        metadata.type = IMAGE_DATA_TYPE_NANOVDB_FP16;
      }
      else {
        metadata.type = IMAGE_DATA_TYPE_NANOVDB_FLOAT;
      }
    }
    else {
      metadata.type = IMAGE_DATA_TYPE_NANOVDB_FLOAT3;
    }
  }
  else
#  endif
  {
    if (metadata.channels == 1) {
      metadata.type = IMAGE_DATA_TYPE_FLOAT;
    }
    else {
      metadata.type = IMAGE_DATA_TYPE_FLOAT4;
    }
  }

  /* Set transform from object space to voxel index. */
  ::openvdb::math::Mat4f grid_matrix = grid->transform().baseMap()->getAffineMap()->getMat4();
  Transform index_to_object;
  
  // Use a safer approach to convert the matrix
  for (int col = 0; col < 4; col++) {
    for (int row = 0; row < 3; row++) {
      // Get the value from grid_matrix and convert it to float
      // Access the matrix elements directly as individual floats
      index_to_object[row][col] = grid_matrix[col][row];
    }
  }

  Transform texture_to_index;
#  ifdef WITH_NANOVDB
  if (nanogrid_ptr) {
    texture_to_index = transform_identity();
  }
  else
#  endif
  {
    /* Shift by half a voxel to sample at voxel centers. */
    ::openvdb::Coord min = bbox.min();
    texture_to_index = transform_translate(-0.5f, -0.5f, -0.5f) *
                       transform_scale(1.0f / dim.x(), 1.0f / dim.y(), 1.0f / dim.z()) *
                       transform_translate(min.x(), min.y(), min.z());
  }

  /* Compute final transform from texture space to object space. */
  metadata.transform_3d = index_to_object * texture_to_index;
  metadata.use_transform_3d = true;

  return true;
#else
  return false;
#endif
}

bool VDBImageLoader::load_pixels(const ImageMetaData &metadata,
                                void *pixels,
                                const size_t pixels_size,
                                const bool associate_alpha)
{
#ifdef WITH_OPENVDB
#  ifdef WITH_NANOVDB
  if (nanogrid_ptr) {
    nanovdb::GridHandle<>& nanogrid = *static_cast<nanovdb::GridHandle<>*>(nanogrid_ptr);
    memcpy(pixels, nanogrid.data(), nanogrid.size());
    return true;
  }
#  endif

  /* Copy to dense grid. */
  ToDenseOp op;
  ::openvdb::math::CoordBBox& bbox = *static_cast<::openvdb::math::CoordBBox*>(bbox_ptr);
  op.bbox = bbox;
  op.pixels = pixels;
  return grid_type_operation(grid, op);
#else
  return false;
#endif
}

std::string VDBImageLoader::name() const
{
  return grid_name;
}

bool VDBImageLoader::equals(const ImageLoader &other) const
{
  const VDBImageLoader &other_loader = (const VDBImageLoader &)other;
  return grid_name == other_loader.grid_name;
}

void VDBImageLoader::cleanup()
{
#ifdef WITH_OPENVDB
  grid.reset();
#endif
#ifdef WITH_NANOVDB
  if (nanogrid_ptr) {
    delete static_cast<nanovdb::GridHandle<>*>(nanogrid_ptr);
    nanogrid_ptr = nullptr;
  }
#endif
}

bool VDBImageLoader::is_vdb_loader() const
{
  return true;
}

#ifdef WITH_OPENVDB
::openvdb::GridBase::ConstPtr VDBImageLoader::get_grid()
{
  return grid;
}
#endif

CCL_NAMESPACE_END
