add_library(sep_memory
  STATIC 
    memory_tier.cpp
    memory_tier_manager.cpp
    redis_manager.cpp
    quantum_coherence_manager.cpp
)

# Find required Redis library
find_library(HIREDIS_LIB NAMES hiredis PATHS /usr/lib64 REQUIRED)
find_path(HIREDIS_INCLUDE_DIR NAMES hiredis.h PATHS /usr/include/hiredis REQUIRED)

# Link dependencies
target_link_libraries(sep_memory
    PUBLIC
        sep_core
    PRIVATE
        sep_compat # Link compat once, it handles CUDA internally
        ${HIREDIS_LIB}
)

# CUDA and Redis are always required
target_compile_definitions(sep_memory PRIVATE
    SEP_HAS_CUDA=1
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
