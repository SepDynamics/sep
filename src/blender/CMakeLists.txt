add_library(sep_blender STATIC
    api.cpp
    blender_integration.cpp
    compression_utils.cpp
    compression.cpp
    gpu_context.cpp
    mesh_handler.cpp
    pattern_visualization_pipeline.cpp
    cycles_renderer.cpp
)

target_include_directories(sep_blender
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

# Link against Cycles if available
if(SEP_HAS_CYCLES)
    target_compile_definitions(sep_blender PUBLIC SEP_HAS_CYCLES=1)
    target_link_libraries(sep_blender PRIVATE ${CYCLES_LIBRARIES})
endif()
