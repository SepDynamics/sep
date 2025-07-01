include(SetCXXStandard)

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

# Add Cycles renderer when available
if(SEP_HAS_CYCLES)
  target_sources(sep_blender PRIVATE cycles_renderer.cpp)
endif()

# Link directly against Blender's Cycles
set(BLENDER_VERSION "5.0")
set(BLENDER_USER_CONFIG_DIR "$ENV{HOME}/.config/blender/${BLENDER_VERSION}")

# Use bundled Cycles headers when available
if(SEP_HAS_CYCLES)
  target_include_directories(sep_blender PRIVATE
      ${CMAKE_SOURCE_DIR}/extern/cycles/src
  )
endif()

target_compile_definitions(sep_blender
    PUBLIC
        SEP_HAS_CYCLES=${SEP_HAS_CYCLES}
    PRIVATE
        WITH_CYCLES_DEVICE_CUDA
)

# Use bundled dependencies
target_include_directories(sep_blender PRIVATE
    ${CMAKE_SOURCE_DIR}/extern/nanovdb
    ${CMAKE_SOURCE_DIR}/extern/oidn
)

# Link against Cycles libraries built in the extern/cycles subdirectory.
# Link against the Cycles libraries built in the extern/cycles directory.
# The CYCLES_TARGET_PREFIX is empty by default but can be overridden if
# custom target names are desired.
target_link_libraries(sep_blender PRIVATE
    ${CYCLES_TARGET_PREFIX}cycles_device
    ${CYCLES_TARGET_PREFIX}cycles_kernel
    ${CYCLES_TARGET_PREFIX}cycles_util
    ${CYCLES_TARGET_PREFIX}cycles_subd
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
)
if(SEP_HAS_CYCLES)
  target_include_directories(sep_blender PUBLIC
      ${CMAKE_SOURCE_DIR}/extern/cycles/src
  )
endif()
