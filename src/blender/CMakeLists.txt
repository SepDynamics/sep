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
        ${CMAKE_SOURCE_DIR}/extern
        ${CMAKE_SOURCE_DIR}/extern/glm
        ${CMAKE_SOURCE_DIR}/cycles/src
        ${CMAKE_SOURCE_DIR}/extern/cycles/src
        ${CMAKE_SOURCE_DIR}/extern/blender/lib/linux_x64/osl/include
        ${CMAKE_SOURCE_DIR}/extern/eigen
)

# Link against Cycles if available
if(SEP_HAS_CYCLES)
    target_compile_definitions(sep_blender PUBLIC SEP_HAS_CYCLES=1)
    
    # For static libraries, mark the Cycles dependency but don't link it directly
    # The main executable will handle the actual linking with whole-archive flags
    target_link_libraries(sep_blender PRIVATE sep_core)
    
    # Add Cycles include directories for compilation
    target_include_directories(sep_blender PRIVATE
        ${CYCLES_INCLUDE_DIRS}
    )
endif()
