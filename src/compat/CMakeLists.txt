# src/compat/CMakeLists.txt
# Enhanced CUDA compatibility library with explicit symbol exports

# Build sep_compat as a static library that encapsulates all CUDA functionality
# Use Cycles naming convention for feature flags
set(WITH_CYCLES_NANOVDB ON)
set(WITH_CYCLES_OPENIMAGEDENOISE ON)
set(WITH_CYCLES_OPENVDB ON)
set(WITH_CYCLES_OPENCOLORIO ON)
set(WITH_CYCLES_OPENSUBDIV ON)



# Install headers
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/compat/
        DESTINATION include/sep/compat
        FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.hpp")

install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/compat/
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
    hip_compat.cpp
)

add_library(sep_compat STATIC
    ${CUDA_SOURCES}
    ${CPP_SOURCES}
)

# Ensure CUDA symbols are exported
set_target_properties(sep_compat PROPERTIES
    CUDA_VISIBILITY_PRESET "default"
    C_VISIBILITY_PRESET "default"
    CXX_VISIBILITY_PRESET "default"
    VISIBILITY_INLINES_HIDDEN OFF
    CUDA_SEPARABLE_COMPILATION ON
    CUDA_RESOLVE_DEVICE_SYMBOLS ON
    POSITION_INDEPENDENT_CODE ON
)

target_compile_options(sep_compat PRIVATE $<$<COMPILE_LANGUAGE:CUDA>:${CUDA_NVCC_FLAGS}>)

set_target_properties(sep_compat PROPERTIES
    CUDA_SEPARABLE_COMPILATION ON
    CUDA_RESOLVE_DEVICE_SYMBOLS ON
    POSITION_INDEPENDENT_CODE ON
    CUDA_ARCHITECTURES "75;70;61;60"
)

if(SEP_HAS_CUDA)
    target_link_libraries(sep_compat
        PUBLIC
            CUDA::cudart
            CUDA::cuda_driver
    )
endif()

set_property(TARGET sep_compat PROPERTY CUDA_STANDARD 17)

if(SEP_HAS_CUDA)
    target_compile_definitions(sep_compat PUBLIC SEP_USE_CUDA=1)
else()
    target_compile_definitions(sep_compat PUBLIC SEP_USE_CUDA=0)
endif()

# Include directories
target_include_directories(sep_compat
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/extern/cycles/src>
        ${CUDA_INCLUDE_DIRS}
)

# Dependencies
target_link_libraries(sep_compat
    PUBLIC
        sep_core
)
if(SEP_HAS_CUDA)
    target_link_libraries(sep_compat PUBLIC CUDA::cudart CUDA::cuda_driver)
endif()
