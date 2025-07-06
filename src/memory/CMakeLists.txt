add_library(sep_memory
  STATIC 
    memory_tier.cpp
    memory_tier_manager.cpp
    memory_tier_manager_serialization.cpp
    redis_manager.cpp
    quantum_coherence_manager.cpp
)

# Find required Redis library
find_library(HIREDIS_LIB NAMES hiredis PATHS /usr/lib64 REQUIRED)
find_path(HIREDIS_INCLUDE_DIR NAMES hiredis.h PATHS /usr/include/hiredis REQUIRED)

# Add CUDA-specific compile definitions to handle exception specs
target_compile_definitions(sep_memory PRIVATE
    SEP_HAS_CUDA=1
    _GLIBCXX_USE_NOEXCEPT_SPEC=0
    SEP_CUDACC_DISABLE_EXCEPTION_SPEC_CHECKS=1)

# Link dependencies
target_link_libraries(sep_memory
    PUBLIC
        sep_core
        sep_compat # CUDA utilities required by memory tier
        ${HIREDIS_LIB}
        CUDA::cudart
        CUDA::cuda_driver
)

# Set CUDA properties
set_target_properties(sep_memory PROPERTIES
    CUDA_SEPARABLE_COMPILATION ON
    CUDA_RESOLVE_DEVICE_SYMBOLS ON
    POSITION_INDEPENDENT_CODE ON
    CUDA_ARCHITECTURES "75;70;61;60"
)

# CUDA and Redis are always required
target_compile_definitions(sep_memory PRIVATE
    SEP_HAS_CUDA=1
    SEP_USE_CUDA=1
    SEP_HAS_REDIS=1
)

target_include_directories(sep_memory PRIVATE ${HIREDIS_INCLUDE_DIR})

message(STATUS "Found hiredis: ${HIREDIS_LIB}")
message(STATUS "Found hiredis headers: ${HIREDIS_INCLUDE_DIR}")

# Set library properties
set_target_properties(sep_memory PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    CXX_VISIBILITY_PRESET default
    VISIBILITY_INLINES_HIDDEN OFF
)

# Include directories
target_include_directories(sep_memory
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/extern>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/extern/cycles/src>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/extern/blender/lib/linux_x64/osl/include>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/extern/eigen>
)

# Install headers
install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/memory/
        DESTINATION include/sep/memory
        FILES_MATCHING PATTERN "*.h")
