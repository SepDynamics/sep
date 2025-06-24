# src/compat/CMakeLists.txt
# Include CUDA configuration for library paths
include(${CMAKE_SOURCE_DIR}/cmake/CustomCUDA.cmake)

add_library(sep_compat STATIC
    core/stream.cpp
    raii.cpp
    # Add all .cu files here to be compiled by NVCC
    core/core.cu
    cuda_api.cu
    event.cu
    pattern_kernels.cu
    quantum_kernels.cu
    utils.cu
)

# Ensure CUDA compilation for .cu files
set_source_files_properties(
    core/core.cu
    cuda_api.cu
    event.cu
    pattern_kernels.cu
    quantum_kernels.cu
    utils.cu
    PROPERTIES LANGUAGE CUDA
)

# Add include directories needed by compat library sources
target_include_directories(sep_compat PRIVATE
    ${CMAKE_SOURCE_DIR}/include # For core/error_handler.h, core/types.h, etc.
    ${CMAKE_SOURCE_DIR}/third_party/glm # For GLM headers
    ${CMAKE_SOURCE_DIR}/src # For other source directories
)
