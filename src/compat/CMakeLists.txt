# src/compat/CMakeLists.txt
# Include CUDA configuration for library paths
include(${CMAKE_SOURCE_DIR}/cmake/CustomCUDA.cmake)

set(CUDA_SOURCES
    core/core.cu
    cuda_api.cu
    event.cu
    pattern_kernels.cu
    quantum_kernels.cu
    utils.cu)

add_library(sep_compat STATIC
    core/stream.cpp
    raii.cpp
)

# Ensure the CUDA source files are treated correctly
set_source_files_properties(
    core/core.cu
    cuda_api.cu
    event.cu
    pattern_kernels.cu
    quantum_kernels.cu
    utils.cu
    PROPERTIES
)

# Add include directories needed by compat library sources
target_include_directories(sep_compat 
  PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/third_party/glm
    ${CMAKE_SOURCE_DIR}/src
)
