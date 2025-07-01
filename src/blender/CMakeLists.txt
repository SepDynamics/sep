add_library(sep_blender STATIC
    compression_utils.cpp
    gpu_context.cpp
    compression.cpp
    api.cpp
    pattern_visualization_pipeline.cpp
    blender_integration.cpp
    mesh_handler.cpp
    factory.cpp
)

# Define required features
target_compile_definitions(sep_blender
    PUBLIC
        SEP_HAS_BLENDER=1
    PRIVATE
        SEP_HAS_CUDA=1
)

# Link dependencies
target_link_libraries(sep_blender
    PUBLIC
        sep_core
    PRIVATE
        sep_compat
)

# Add cycles_renderer.cpp unconditionally
target_sources(sep_blender PRIVATE cycles_renderer.cpp)

# Link directly against Blender's Cycles
set(BLENDER_VERSION "5.0")
set(BLENDER_USER_CONFIG_DIR "$ENV{HOME}/.config/blender/${BLENDER_VERSION}")

# Use bundled Cycles headers
target_include_directories(sep_blender PRIVATE
    ${CMAKE_SOURCE_DIR}/extern/cycles/src
)

target_compile_definitions(sep_blender
    PUBLIC
        SEP_HAS_CYCLES=1
    PRIVATE
        WITH_CYCLES_DEVICE_CUDA
)

# Configure Cycles build options
set(WITH_CYCLES_STANDALONE ON CACHE BOOL "Build Cycles standalone")
set(WITH_CYCLES_DEVICE_CUDA ON CACHE BOOL "Enable CUDA support")
set(WITH_CYCLES_DEVICE_OPTIX ON CACHE BOOL "Enable OptiX support")
set(WITH_CYCLES_HYDRA_RENDER_DELEGATE OFF CACHE BOOL "Disable Hydra delegate")
set(WITH_BLENDER OFF CACHE BOOL "Disable Blender integration")

# Build Cycles libraries
add_subdirectory(${CMAKE_SOURCE_DIR}/extern/cycles ${CMAKE_BINARY_DIR}/extern/cycles EXCLUDE_FROM_ALL)

# Set compiler flags for Cycles targets
set_target_properties(cycles_device cycles_kernel cycles_util cycles_subd PROPERTIES
    COMPILE_FLAGS "-D_LIBCPP_HAS_NO_INCOMPLETE_FORMAT -DFMT_DEPRECATED_OSTREAM"
)

# Link against Cycles libraries
target_link_libraries(sep_blender PRIVATE
    cycles_device
    cycles_kernel
    cycles_util
    cycles_subd
)

# Set library properties
set_target_properties(sep_blender PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    CXX_VISIBILITY_PRESET default
    VISIBILITY_INLINES_HIDDEN OFF
)

# Include directories
target_include_directories(sep_blender
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/extern
        ${CMAKE_SOURCE_DIR}/extern/glm
        ${CMAKE_SOURCE_DIR}/extern/eigen
        ${CMAKE_SOURCE_DIR}/extern/cycles/src
)
