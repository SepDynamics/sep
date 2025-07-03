find_package(CUDAToolkit REQUIRED)
find_package(TBB REQUIRED)
find_package(PkgConfig REQUIRED)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/extern/cycles/src/cmake/Modules)

find_package(Embree REQUIRED)
find_package(GLOG REQUIRED)
find_package(OSL REQUIRED)
find_package(OpenSubdiv REQUIRED)
pkg_check_modules(OIIO REQUIRED OpenImageIO)
pkg_check_modules(ZSTD REQUIRED libzstd)

add_library(sep_blender STATIC
    compression_utils.cpp
    gpu_context.cpp
    compression.cpp
    api.cpp
    pattern_visualization_pipeline.cpp
    blender_integration.cpp
    mesh_handler.cpp
    factory.cpp
)

# Define required features
target_compile_definitions(sep_blender
    PUBLIC
        SEP_HAS_BLENDER=1
    PRIVATE
        SEP_HAS_CUDA=1
)

# Link dependencies
target_link_libraries(sep_blender
    PUBLIC
        sep_core
        ${OIIO_LIBRARIES}
        glog
        ${ZSTD_LIBRARIES}
        ${EMBREE_LIBRARIES}
    PRIVATE
        sep_compat
        ${CMAKE_SOURCE_DIR}/libsep_compat.a
)

# Enable CUDA support
target_compile_definitions(sep_blender PRIVATE SEP_HAS_CUDA=1)
target_link_libraries(sep_blender PRIVATE CUDA::cudart)

target_sources(sep_blender PRIVATE
    cycles_renderer.cpp
)

set_source_files_properties(cycles_renderer.cpp
  PROPERTIES
  COMPILE_FLAGS "-DWITH_CYCLES_DEVICE_CUDA"
)

# Link directly against Blender's Cycles
set(BLENDER_VERSION "5.0")
set(BLENDER_USER_CONFIG_DIR "$ENV{HOME}/.config/blender/${BLENDER_VERSION}")

# Set Cycles root directory and enable integration
set(CYCLES_ROOT ${CMAKE_SOURCE_DIR}/extern/cycles)

# Use bundled Cycles headers
target_include_directories(sep_blender SYSTEM PRIVATE
  ${CYCLES_ROOT}/src
)
target_include_directories(sep_blender PUBLIC
    ${OIIO_INCLUDE_DIRS}
    ${GLOG_INCLUDE_DIRS}
    ${EMBREE_INCLUDE_DIRS}
    ${ZSTD_INCLUDE_DIRS}
    ${OSL_INCLUDE_DIRS}
    ${OPENSUBDIV_INCLUDE_DIRS}
)

# Define Cycles support
target_compile_definitions(sep_blender
  PUBLIC
    SEP_HAS_CYCLES=1
  PRIVATE
    WITH_CYCLES_DEVICE_CUDA
)

target_compile_definitions(sep_blender
    PUBLIC
        SEP_HAS_CYCLES=1
    PRIVATE
        WITH_CYCLES_DEVICE_CUDA
)

# Use bundled dependencies
target_include_directories(sep_blender PRIVATE
    ${CMAKE_SOURCE_DIR}/extern/nanovdb
    ${CMAKE_SOURCE_DIR}/extern/oidn
)

# Add library search paths
link_directories(${CMAKE_SOURCE_DIR}/lib)

# Link against Cycles libraries and required dependencies
if(TARGET cycles_scene)
  # Linking against Cycles built from source
  target_link_libraries(sep_blender PRIVATE
      cycles_device
      cycles_kernel
      cycles_util
      cycles_subd
      cycles_session
      cycles_scene
      cycles_graph
      cycles_kernel_osl
      cycles_bvh
      cycles_integrator
      bf_intern_guardedalloc
      TBB::tbb
      CUDA::cudart
      CUDA::cuda_driver
      CUDA::nvrtc
      glog
      gflags
      embree4
      extern_cuew
      extern_hipew
      ${OSL_LIBRARIES}
      ${OPENVDB_LIBRARIES}
      ${OPENSUBDIV_LIBRARIES}
      bf_intern_openvdb
      OpenVDB::openvdb
      bf_intern_opensubdiv
)
else()
  # Fallback to precompiled static libraries
  target_link_libraries(sep_blender PRIVATE
      cycles_device
      cycles_kernel
      cycles_util
      cycles_subd
      cycles_session
      cycles_scene
      cycles_graph
      cycles_kernel_osl
      cycles_bvh
      cycles_integrator
      bf_intern_guardedalloc
      CUDA::cudart
      CUDA::cuda_driver
      CUDA::nvrtc
      glog
      gflags
      embree4
      extern_cuew
      extern_hipew
      ${OSL_LIBRARIES}
      ${OPENVDB_LIBRARIES}
      ${OPENSUBDIV_LIBRARIES}
      bf_intern_openvdb
      openvdb
      bf_intern_opensubdiv
  )
endif()



# Add Blender's include directories for memory allocation functions
target_include_directories(sep_blender PRIVATE
    ${BLENDER_INCLUDE_DIRS}
    ${BLENDER_INCLUDE_DIRS}/blender
    ${BLENDER_INCLUDE_DIRS}/blender/makesdna
    ${BLENDER_INCLUDE_DIRS}/blender/blenlib
)

# Set library properties
set_target_properties(sep_blender PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    CXX_VISIBILITY_PRESET default
    VISIBILITY_INLINES_HIDDEN OFF
)

# Include directories
target_include_directories(sep_blender
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/extern
        ${CMAKE_SOURCE_DIR}/extern/glm
        ${CMAKE_SOURCE_DIR}/extern/eigen
        ${OSL_INCLUDE_DIRS}
        ${OPENSUBDIV_INCLUDE_DIRS}
)
target_include_directories(sep_blender PUBLIC
    ${CMAKE_SOURCE_DIR}/extern/cycles/src
)

set(OIIO_LIBRARIES ${OIIO_LIBRARIES} PARENT_SCOPE)
set(GLOG_LIBRARIES ${GLOG_LIBRARIES} PARENT_SCOPE)
set(ZSTD_LIBRARIES ${ZSTD_LIBRARIES} PARENT_SCOPE)
set(EMBREE_LIBRARIES ${EMBREE_LIBRARIES} PARENT_SCOPE)
