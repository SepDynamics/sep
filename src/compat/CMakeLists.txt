# src/compat/CMakeLists.txt
# Enhanced CUDA compatibility library with explicit symbol exports

# Find CUDA if enabled (Note: CUDAToolkit is already found in the root CMakeLists.txt)

# Build sep_compat as a static library that encapsulates all CUDA functionality
# Use Cycles naming convention for feature flags
set(WITH_CYCLES_NANOVDB ON)
set(WITH_CYCLES_OPENIMAGEDENOISE ON)
set(WITH_CYCLES_OPENVDB ON)
set(WITH_CYCLES_OPENCOLORIO ON)
set(WITH_CYCLES_OPENSUBDIV ON)

# Install headers
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
        DESTINATION include/sep/compat
        FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.hpp")

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
        DESTINATION include/sep/compat/internal
        FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.hpp")

# Define CUDA sources
set(CUDA_SOURCES
    pattern_kernels.cu
    cuda_api.cu
    event.cu
    utils.cu
    kernels.cu
    core.cu
    quantum_kernels.cu
)

# Define C++ sources
set(CPP_SOURCES
    stream.cpp
    raii.cpp
    cuda_impl.cpp
)

add_library(sep_compat STATIC
    ${CPP_SOURCES}
    ${CUDA_SOURCES}
)

# Set properties for the shared library
set_target_properties(sep_compat PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    CUDA_SEPARABLE_COMPILATION ON
    CUDA_RESOLVE_DEVICE_SYMBOLS ON
    CUDA_STANDARD 17
    CXX_STANDARD 17
    CUDA_ARCHITECTURES "75" # Keep it simple for now, add others back later
    C_VISIBILITY_PRESET default
    CXX_VISIBILITY_PRESET default
    CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS} -I${CMAKE_SOURCE_DIR}/src -I${CMAKE_SOURCE_DIR}/src/core -I${CMAKE_SOURCE_DIR}/src/quantum"
)

target_include_directories(sep_compat
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include/sep/compat>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/core>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/quantum>
)

if(SEP_USE_CUDA)
    target_compile_definitions(sep_compat PUBLIC SEP_USE_CUDA=1)
# Link against modern CUDA targets. This transitively handles include paths and libraries.
target_link_libraries(sep_compat PUBLIC
    CUDA::toolkit
    sep_core
    sep_quantum
)
else()
    target_compile_definitions(sep_compat PUBLIC SEP_USE_CUDA=0)
endif()
