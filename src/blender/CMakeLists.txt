# Require C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find necessary packages
find_package(CUDAToolkit REQUIRED)
find_package(OpenImageIO REQUIRED)
find_package(OSL REQUIRED)
find_package(TBB REQUIRED)
# Add other find_package() calls here for Embree, OpenVDB etc. as needed

add_library(sep_blender SHARED
    api.cpp
    blender_integration.cpp
    compression.cpp
    compression_utils.cpp
    cycles_renderer.cpp
    factory.cpp
    gpu_context.cpp
    mesh_handler.cpp
    oiio_output_driver.cpp
    pattern_visualization_pipeline.cpp
)

target_precompile_headers(sep_blender PRIVATE blender_pch.h)

# Define compile definitions in one place
target_compile_definitions(sep_blender PRIVATE
    SEP_HAS_BLENDER=1
    SEP_HAS_CYCLES=1
    SEP_HAS_CUDA=1
    SEP_CUDACC_DISABLE_EXCEPTION_SPEC_CHECKS=1
    WITH_CYCLES_DEVICE_CUDA
    "CCL_NAMESPACE_BEGIN=namespace ccl {"
    "CCL_NAMESPACE_END=}"
)

# Link all dependencies to the target
target_link_libraries(sep_blender PUBLIC
    sep_core
    sep_memory
    sep_quantum
    sep_compat
    ${OpenImageIO_LIBRARIES}
    ${OSL_LIBRARIES}
    ${TBB_LIBRARIES}
    cycles_device
    cycles_kernel
    cycles_scene
    cycles_util
    CUDA::cudart
    CUDA::cuda_driver
    ${CMAKE_DL_LIBS}
    Threads::Threads
)

# Include directories
target_include_directories(sep_blender PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    ${OpenImageIO_INCLUDE_DIRS}
    ${OSL_INCLUDE_DIRS}
    PRIVATE
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/extern/cycles/src>
)

# Properties for building a shared library
set_target_properties(sep_blender PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    CUDA_SEPARABLE_COMPILATION ON
    CUDA_RESOLVE_DEVICE_SYMBOLS ON
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
