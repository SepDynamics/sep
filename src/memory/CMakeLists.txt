add_library(sep_memory
  STATIC
    memory_tier.cpp
    memory_tier_manager.cpp
    memory_tier_manager_serialization.cpp
)

if(SEP_BUILD_QUANTUM)
  target_sources(sep_memory PRIVATE quantum_coherence_manager.cpp)
endif()

# Find required Redis library
set(SEP_HAS_REDIS 0)
set(HIREDIS_FOUND FALSE)
add_compile_definitions(SEP_NO_REDIS)

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
        fmt::fmt
    PRIVATE
        Threads::Threads
)
if(HIREDIS_FOUND)
    target_link_libraries(sep_memory PRIVATE ${HIREDIS_LIB})
endif()

# Configure include directories
target_include_directories(sep_memory
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
)
if(HIREDIS_FOUND)
    target_include_directories(sep_memory PRIVATE ${HIREDIS_INCLUDE_DIR})
endif()

# Add compile definitions
target_compile_definitions(sep_memory
    PUBLIC
        SEP_HAS_CUDA=$<IF:$<BOOL:${SEP_HAS_CUDA}>,1,0>
    PRIVATE
        SEP_USE_CUDA=$<IF:$<BOOL:${SEP_HAS_CUDA}>,1,0>
        SEP_HAS_REDIS=${SEP_HAS_REDIS}
)

if(HIREDIS_FOUND)
    message(STATUS "Found hiredis: ${HIREDIS_LIB}")
    message(STATUS "Found hiredis headers: ${HIREDIS_INCLUDE_DIR}")
endif()

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
