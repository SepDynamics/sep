# src/compat/CMakeLists.txt
# Enhanced CUDA compatibility library with explicit symbol exports

# Build sep_compat as a static library that encapsulates all CUDA functionality
# Use Cycles naming convention for feature flags
set(WITH_CYCLES_NANOVDB ON)
set(WITH_CYCLES_OPENIMAGEDENOISE ON)
set(WITH_CYCLES_OPENVDB ON)
set(WITH_CYCLES_OPENCOLORIO ON)
set(WITH_CYCLES_OPENSUBDIV ON)

# Build full CUDA implementation
find_package(CUDAToolkit REQUIRED)

add_library(sep_compat STATIC
    stream.cpp
    raii.cpp
    hip_compat.cpp
    pattern_kernels.cu
    cuda_api.cu
    event.cu
    utils.cu
    kernels.cu
    core.cu
    quantum_kernels.cu
)

set_source_files_properties(
    COMPILE_FLAGS "${CUDA_NVCC_FLAGS}"
)

set_target_properties(sep_compat PROPERTIES
    CUDA_SEPARABLE_COMPILATION ON
    CUDA_RESOLVE_DEVICE_SYMBOLS ON
    POSITION_INDEPENDENT_CODE ON
    CUDA_ARCHITECTURES native
)

set_property(TARGET sep_compat PROPERTY CUDA_STANDARD 17)

target_compile_definitions(sep_compat PUBLIC SEP_USE_CUDA=1)

# Include directories
target_include_directories(sep_compat
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include/compat
        ${CMAKE_SOURCE_DIR}/include/core
    PRIVATE
        ${CUDA_INCLUDE_DIRS}
        ${CMAKE_SOURCE_DIR}/extern/cycles/src
)

# Dependencies
target_link_libraries(sep_compat
    PUBLIC
        sep_core
        CUDA::cudart
        CUDA::cuda_driver
)
