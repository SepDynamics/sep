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

# Cycles is a required dependency
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")
find_package(Cycles REQUIRED HINTS
    /usr/lib/cmake
    /usr/local/lib/cmake
    ${BLENDER_ROOT}/lib/cmake
    $ENV{BLENDER_ROOT}/lib/cmake
)

target_compile_definitions(sep_blender PUBLIC SEP_HAS_CYCLES=1)
target_include_directories(sep_blender PRIVATE ${CYCLES_INCLUDE_DIRS})
target_link_libraries(sep_blender PUBLIC ${CYCLES_LIBRARIES})

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
