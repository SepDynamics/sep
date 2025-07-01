# src/compat/CMakeLists.txt
# Enhanced CUDA compatibility library with explicit symbol exports


# Build sep_compat as a static library that encapsulates all CUDA functionality
# Use Cycles naming convention for feature flags
if(WITH_NANOVDB)
    set(WITH_CYCLES_NANOVDB ${WITH_NANOVDB})
endif()

if(WITH_OPENIMAGEDENOISE)
    set(WITH_CYCLES_OPENIMAGEDENOISE ${WITH_OPENIMAGEDENOISE})
endif()

if(WITH_OPENVDB)
    set(WITH_CYCLES_OPENVDB ${WITH_OPENVDB})
endif()

if(WITH_OPENCOLORIO)
    set(WITH_CYCLES_OPENCOLORIO ${WITH_OPENCOLORIO})
endif()

if(WITH_OPENSUBDIV)
    set(WITH_CYCLES_OPENSUBDIV ${WITH_OPENSUBDIV})
endif()

add_library(sep_compat STATIC
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

# Configure CUDA properties
set_target_properties(sep_compat PROPERTIES
    CUDA_SEPARABLE_COMPILATION ON
    CUDA_RESOLVE_DEVICE_SYMBOLS ON
    POSITION_INDEPENDENT_CODE ON
)

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
    PRIVATE
        ${CUDA_LIBRARIES}
        ${CUDAToolkit_LIBRARY_DIR}/libcudart.so
)
