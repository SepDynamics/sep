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

add_library(sep_blender SHARED
    compression_utils.cpp
    gpu_context.cpp
    compression.cpp
    api.cpp
    pattern_visualization_pipeline.cpp
    blender_integration.cpp
    mesh_handler.cpp
    factory.cpp
    oiio_output_driver.cpp
)

# Require C++17 for this target
target_compile_features(sep_blender PUBLIC cxx_std_17)

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
        sep_embeddings
        sep_quantum
        sep_memory
        ${OIIO_LIBRARIES}
        glog
        ${ZSTD_LIBRARIES}
        ${EMBREE_LIBRARIES}
    PRIVATE
        sep_compat
)

# Enable CUDA support
target_compile_definitions(sep_blender
    PRIVATE
        SEP_HAS_CUDA=1
        _GLIBCXX_USE_NOEXCEPT_SPEC=0
        SEP_CUDACC_DISABLE_EXCEPTION_SPEC_CHECKS=1
)
target_link_libraries(sep_blender
    PRIVATE
        CUDA::cudart
        CUDA::cuda_driver
        CUDA::nvrtc
)

target_sources(sep_blender PRIVATE
    cycles_renderer.cpp
    oiio_output_driver.cpp
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
  ${CYCLES_ROOT}/src/device
  ${CYCLES_ROOT}/src/scene
  ${CYCLES_ROOT}/src/session
  ${CYCLES_ROOT}/src/util
)
target_include_directories(sep_blender PUBLIC
    ${OIIO_INCLUDE_DIRS}
    ${GLOG_INCLUDE_DIRS}
    ${EMBREE_INCLUDE_DIRS}
    ${ZSTD_INCLUDE_DIRS}
    ${OSL_INCLUDE_DIRS}
    ${OPENSUBDIV_INCLUDE_DIRS}
)

# Define Cycles support and namespace
target_compile_definitions(sep_blender
  PUBLIC
    SEP_HAS_CYCLES=1
    "CCL_NAMESPACE_BEGIN=namespace ccl {"
    "CCL_NAMESPACE_END=}"
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
      embree
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
    PREFIX "lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    CUDA_SEPARABLE_COMPILATION ON
    CUDA_RESOLVE_DEVICE_SYMBOLS ON
    CUDA_ARCHITECTURES "75;70;61;60"
)

# Ensure proper symbol visibility for shared library
target_compile_options(sep_blender PRIVATE
    -fvisibility=default
    -fPIC
)

# Include directories
target_include_directories(sep_blender
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        ${OSL_INCLUDE_DIRS}
        ${OPENSUBDIV_INCLUDE_DIRS}
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/extern>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/extern/glm>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/extern/eigen>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/extern/cycles/src>
)

# Install headers
install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/blender/
        DESTINATION include/sep/blender
        FILES_MATCHING PATTERN "*.h")

set(OIIO_LIBRARIES ${OIIO_LIBRARIES} PARENT_SCOPE)
set(GLOG_LIBRARIES ${GLOG_LIBRARIES} PARENT_SCOPE)
set(ZSTD_LIBRARIES ${ZSTD_LIBRARIES} PARENT_SCOPE)
set(EMBREE_LIBRARIES ${EMBREE_LIBRARIES} PARENT_SCOPE)
