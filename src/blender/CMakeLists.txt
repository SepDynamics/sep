add_library(sep_blender STATIC api.cpp blender_integration.cpp compression_utils.cpp compression.cpp gpu_context.cpp mesh_handler.cpp pattern_visualization_pipeline.cpp)

target_include_directories(sep_blender
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)
