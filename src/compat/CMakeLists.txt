# src/compat/CMakeLists.txt
# Enhanced CUDA compatibility library with explicit symbol exports


# Build sep_compat as a static library that encapsulates all CUDA functionality
add_library(sep_compat STATIC
    component_bridge.cpp
    cuda_api.cu
    core.cu
    pattern_kernels.cu
    quantum_kernels.cu
    utils.cu
    event.cu
    stream.cpp
    raii.cpp
)

# Set CUDA source file properties
set_source_files_properties(
    cuda_api.cu core.cu pattern_kernels.cu quantum_kernels.cu utils.cu event.cu
    PROPERTIES
    COMPILE_FLAGS "${CUDA_NVCC_FLAGS}"
)

# Configure CUDA target using custom pipeline

# Link CUDA libraries and dependencies
target_link_libraries(sep_compat
    PUBLIC
        ${CUDA_LIBRARIES}
        ${CUDAToolkit_LIBRARY_DIR}/libcudart.so
        sep_core
)

# Ensure CUDA compilation
set_target_properties(sep_compat PROPERTIES
    CUDA_SEPARABLE_COMPILATION ON
    CUDA_RESOLVE_DEVICE_SYMBOLS ON
    POSITION_INDEPENDENT_CODE ON
)

# Export CUDA includes through sep_compat interface
target_include_directories(sep_compat
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include/compat
        ${CMAKE_SOURCE_DIR}/include/core
    INTERFACE
        ${CUDA_INCLUDE_DIRS}
)
