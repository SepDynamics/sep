# src/compat/CMakeLists.txt
# Include CUDA configuration for library paths
include(${CMAKE_SOURCE_DIR}/cmake/CustomCUDA.cmake)

add_library(sep_compat STATIC
    stream.cpp
    raii.cpp
    # Add all .cu files here to be compiled by NVCC
    core.cu
    cuda_api.cu
    event.cu
    pattern_kernels.cu
    quantum_kernels.cu
    utils.cu
)

if(NOT SEP_CUDA_AVAILABLE)
  target_sources(sep_compat PRIVATE core_stub.cpp)
endif()

# Comment out CUDA configuration
set_source_files_properties(
     core.cu
     cuda_api.cu
     event.cu
     pattern_kernels.cu
     quantum_kernels.cu
     utils.cu
)

# Add include directories needed by compat library sources
target_include_directories(sep_compat 
  PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/third_party/glm
    ${CMAKE_SOURCE_DIR}/src
)
