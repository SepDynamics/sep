add_library(sep_blender STATIC
    compression_utils.cpp
    gpu_context.cpp
    compression.cpp
    cycles_renderer.cpp
    api.cpp
    pattern_visualization_pipeline.cpp
    blender_integration.cpp
    mesh_handler.cpp
)

# Configure CUDA support
if(SEP_CUDA_AVAILABLE)
    target_compile_definitions(sep_blender PRIVATE SEP_HAS_CUDA=1)
    target_include_directories(sep_blender PRIVATE ${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES})
    target_link_libraries(sep_blender PRIVATE sep_compat)
endif()

# Link dependencies
target_link_libraries(sep_blender
    PUBLIC
        sep_core
    PRIVATE
        sep_compat
)

# Set library properties
set_target_properties(sep_blender PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    CXX_VISIBILITY_PRESET default
    VISIBILITY_INLINES_HIDDEN OFF
)

# Include directories (deduplicated)
target_include_directories(sep_blender
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/include/api
        ${CMAKE_SOURCE_DIR}/include/audio
        ${CMAKE_SOURCE_DIR}/include/blender
        ${CMAKE_SOURCE_DIR}/include/compat
        ${CMAKE_SOURCE_DIR}/include/core
        ${CMAKE_SOURCE_DIR}/include/crow
        ${CMAKE_SOURCE_DIR}/include/memory
        ${CMAKE_SOURCE_DIR}/include/quantum
        ${CMAKE_SOURCE_DIR}/include/quantum/api
        ${CMAKE_SOURCE_DIR}/include/quantum/audio
        ${CMAKE_SOURCE_DIR}/include/quantum/blender
        ${CMAKE_SOURCE_DIR}/include/quantum/compat
        ${CMAKE_SOURCE_DIR}/include/quantum/core
        ${CMAKE_SOURCE_DIR}/include/quantum/crow
        ${CMAKE_SOURCE_DIR}/include/quantum/memory
        ${CMAKE_SOURCE_DIR}/include/quantum/quantum
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
