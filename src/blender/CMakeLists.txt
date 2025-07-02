find_package(CUDAToolkit REQUIRED)
find_package(TBB REQUIRED)
find_package(PkgConfig REQUIRED)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/extern/cycles/src/cmake/Modules)

find_package(Embree REQUIRED)
find_package(Glog REQUIRED)
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
        ${GLOG_LIBRARIES}
        ${ZSTD_LIBRARIES}
        ${EMBREE_LIBRARIES}
        openpgl_stub
    PRIVATE
        sep_compat
        ${CMAKE_SOURCE_DIR}/libsep_compat.a
        TBB::tbb
)

# Enable CUDA support
if(SEP_HAS_CUDA)
    target_compile_definitions(sep_blender PRIVATE SEP_HAS_CUDA=1)
    target_link_libraries(sep_blender PRIVATE CUDA::cudart)
endif()

# Always compile the Cycles renderer. When SEP_HAS_CYCLES is 0, the
# implementation falls back to stub definitions via preprocessor checks.
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
set(SEP_HAS_CYCLES 1)

# Use bundled Cycles headers
target_include_directories(sep_blender SYSTEM PRIVATE
  ${CYCLES_ROOT}/src
)
target_include_directories(sep_blender PUBLIC
    ${OIIO_INCLUDE_DIRS}
    ${GLOG_INCLUDE_DIRS}
    ${EMBREE_INCLUDE_DIRS}
    ${ZSTD_INCLUDE_DIRS}
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
        SEP_HAS_CYCLES=${SEP_HAS_CYCLES}
    PRIVATE
        WITH_CYCLES_DEVICE_CUDA
)

# Use bundled dependencies
target_include_directories(sep_blender PRIVATE
    ${CMAKE_SOURCE_DIR}/extern/nanovdb
    ${CMAKE_SOURCE_DIR}/extern/oidn
)

# Link against Cycles libraries and required dependencies
if(SEP_HAS_CYCLES)
  target_link_libraries(sep_blender PRIVATE
      ${CMAKE_SOURCE_DIR}/lib/libcycles_device.a
      ${CMAKE_SOURCE_DIR}/lib/libcycles_kernel.a
      ${CMAKE_SOURCE_DIR}/lib/libcycles_util.a
      ${CMAKE_SOURCE_DIR}/lib/libcycles_subd.a
      ${CMAKE_SOURCE_DIR}/lib/libbf_intern_guardedalloc.a # For MEM_* functions
      TBB::tbb  # For parallel execution
      CUDA::cudart  # For CUDA runtime
      CUDA::cuda_driver
      CUDA::nvrtc
      cuew_stub
      openpgl_stub
      glog
      gflags
      embree4
      openpgl_stub
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
)
if(SEP_HAS_CYCLES)
  target_include_directories(sep_blender PUBLIC
      ${CMAKE_SOURCE_DIR}/extern/cycles/src
  )
endif()

set(OIIO_LIBRARIES ${OIIO_LIBRARIES} PARENT_SCOPE)
set(GLOG_LIBRARIES ${GLOG_LIBRARIES} PARENT_SCOPE)
set(ZSTD_LIBRARIES ${ZSTD_LIBRARIES} PARENT_SCOPE)
set(EMBREE_LIBRARIES ${EMBREE_LIBRARIES} PARENT_SCOPE)
