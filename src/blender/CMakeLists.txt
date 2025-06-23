add_library(sep_blender STATIC api.cpp blender_integration.cpp compression_utils.cpp compression.cpp gpu_context.cpp mesh_handler.cpp pattern_visualization_pipeline.cpp)
include_directories(${CMAKE_SOURCE_DIR}/include)
