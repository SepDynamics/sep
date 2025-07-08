add_library(sep_memory
  STATIC 
    memory_tier.cpp
    memory_tier_manager.cpp
    memory_tier_manager_serialization.cpp
    redis_manager.cpp
    quantum_coherence_manager.cpp
)

# Find required Redis library
find_library(HIREDIS_LIB NAMES hiredis PATHS /usr/lib64 REQUIRED)
find_path(HIREDIS_INCLUDE_DIR NAMES hiredis.h PATHS /usr/include/hiredis REQUIRED)

# Add CUDA-specific compile definitions to handle exception specs
if(SEP_HAS_CUDA)
  target_compile_definitions(sep_memory PRIVATE
      SEP_HAS_CUDA=1
      SEP_CUDACC_DISABLE_EXCEPTION_SPEC_CHECKS=1)
  find_package(CUDA REQUIRED)
  set_target_properties(sep_memory PROPERTIES
      CUDA_SEPARABLE_COMPILATION ON
      CUDA_RESOLVE_DEVICE_SYMBOLS ON
      CUDA_ARCHITECTURES "75;70;61;60")
else()
  target_compile_definitions(sep_memory PRIVATE
      SEP_HAS_CUDA=0)
endif()

find_package(Threads REQUIRED)

set_target_properties(sep_memory PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    CXX_VISIBILITY_PRESET default
    VISIBILITY_INLINES_HIDDEN OFF
)

# Link dependencies
target_link_libraries(sep_memory
    PUBLIC
        sep_core
        sep_compat
    PRIVATE
        ${HIREDIS_LIB}
        Threads::Threads
)

# Configure include directories
target_include_directories(sep_memory
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${HIREDIS_INCLUDE_DIR}
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
)

# Add compile definitions
target_compile_definitions(sep_memory
    PUBLIC
        SEP_HAS_CUDA=$<IF:$<BOOL:${SEP_HAS_CUDA}>,1,0>
    PRIVATE
        SEP_USE_CUDA=$<IF:$<BOOL:${SEP_HAS_CUDA}>,1,0>
        SEP_HAS_REDIS=1
)

message(STATUS "Found hiredis: ${HIREDIS_LIB}")
message(STATUS "Found hiredis headers: ${HIREDIS_INCLUDE_DIR}")

# Install headers
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/memory/
        DESTINATION include/sep/memory
        FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.hpp")

install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/memory/
        DESTINATION include/sep/memory/internal
        FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.hpp")
