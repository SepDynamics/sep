add_library(sep_memory 
  STATIC 
    memory_tier.cpp
    memory_tier_manager.cpp
    redis_manager.cpp
    quantum_coherence_manager.cpp
)

# Link dependencies
target_link_libraries(sep_memory
    PUBLIC
        sep_core
    PRIVATE
        sep_compat # Link compat once, it handles CUDA internally
)

# Configure CUDA support if available
if(SEP_CUDA_AVAILABLE)
    target_compile_definitions(sep_memory PRIVATE SEP_HAS_CUDA=1)
    # CUDA includes and device symbols are handled by sep_compat
endif()

# Set library properties
set_target_properties(sep_memory PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    CXX_VISIBILITY_PRESET default
    VISIBILITY_INLINES_HIDDEN OFF
)

# Include directories
target_include_directories(sep_memory
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
        ${CMAKE_SOURCE_DIR}/cycles/src
        ${CMAKE_SOURCE_DIR}/extern
        ${CMAKE_SOURCE_DIR}/extern/cycles/src
        ${CMAKE_SOURCE_DIR}/extern/blender/lib/linux_x64/osl/include
        ${CMAKE_SOURCE_DIR}/extern/eigen
)
