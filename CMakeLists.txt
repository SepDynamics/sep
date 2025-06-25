cmake_minimum_required(VERSION 3.10)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
project(SEPEngine LANGUAGES CXX)

# Ensure RTTI is disabled consistently across all targets when building with
# the Crow isolation headers.
add_compile_definitions(CROW_DISABLE_RTTI=1)

# Include CUDA configuration for library paths
include(${CMAKE_SOURCE_DIR}/cmake/CustomCUDA.cmake)

# Locate external dependencies required by the engine
find_package(CUDAToolkit QUIET)
find_package(HIREDIS QUIET)
find_package(CURL QUIET)
find_package(CROW QUIET)
find_package(TBB QUIET)
find_package(glm QUIET)
find_package(fmt QUIET)
find_package(spdlog QUIET)

include(FetchContent)

# Use C++17 across all targets
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define Crow compile-time configuration globally to avoid
# redundant macro definitions in individual compilation units
add_compile_definitions(CROW_DISABLE_RTTI=1)

# Locate external dependencies required for the final executable
find_package(CURL REQUIRED)
# Hiredis doesn't ship a CMake package file in many distributions,
# so fall back to a simple find_library/find_path approach.
find_library(HIREDIS_LIBRARIES NAMES hiredis)
find_path(HIREDIS_INCLUDE_DIR hiredis/hiredis.h)
if(HIREDIS_LIBRARIES AND HIREDIS_INCLUDE_DIR)
    set(SEP_HAS_HIREDIS 1)
else()
    set(SEP_HAS_HIREDIS 0)
    set(HIREDIS_LIBRARIES "")
    set(HIREDIS_INCLUDE_DIR "")
endif()
add_compile_definitions(SEP_HAS_HIREDIS=${SEP_HAS_HIREDIS})

# Blender support is disabled in this build
set(SEP_HAS_BLENDER 0)
add_compile_definitions(SEP_HAS_BLENDER=${SEP_HAS_BLENDER})


# This ensures our fixed headers are used instead of the problematic ones
include_directories(
  BEFORE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/blender
    ${CMAKE_SOURCE_DIR}/include/compat
    ${CMAKE_SOURCE_DIR}/include/core
    ${CMAKE_SOURCE_DIR}/include/crow
    ${CMAKE_SOURCE_DIR}/include/memory
    ${CMAKE_SOURCE_DIR}/include/quantum
)

# Add our fixed Crow headers subdirectory
add_subdirectory(include/crow)

include_directories(
  ${CMAKE_SOURCE_DIR}/extern
  ${CMAKE_SOURCE_DIR}/extern/glm
  ${CMAKE_SOURCE_DIR}/extern/tbb
)

# Optional Cycles renderer
if(EXISTS "${CMAKE_SOURCE_DIR}/extern/cycles/CMakeLists.txt")
    set(SEP_HAS_CYCLES 1)
    # Use our custom FindCycles module
    list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
    include(FindCycles)
else()
    set(SEP_HAS_CYCLES 0)
endif()
add_compile_definitions(SEP_HAS_CYCLES=${SEP_HAS_CYCLES})

# Fetch the Crow web framework if it is not present
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/extern/crow/include")
    message(STATUS "Fetching Crow framework...")
    FetchContent_Declare(
        crow
        GIT_REPOSITORY https://github.com/CrowCpp/Crow.git
        GIT_TAG v1.2.1.2
    )
    FetchContent_MakeAvailable(crow)
    include_directories(${crow_SOURCE_DIR}/include)
else()
    include_directories(${CMAKE_SOURCE_DIR}/extern/crow/include)
endif()

set(CUDA_SOURCES
    compat/core.cu
    compat/cuda_api.cu
    compat/event.cu
    compat/pattern_kernels.cu
    compat/quantum_kernels.cu
    compat/utils.cu
)
add_executable(sep_engine src/main.cpp)

add_subdirectory(src/api)
add_subdirectory(src/audio)
add_subdirectory(src/blender)
add_subdirectory(src/compat)
add_subdirectory(src/core)
add_subdirectory(src/memory)
add_subdirectory(src/quantum)
add_subdirectory(src/tests)


target_include_directories(sep_engine
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)
if(HIREDIS_INCLUDE_DIR)
    target_include_directories(sep_engine PRIVATE ${HIREDIS_INCLUDE_DIR})
endif()


target_link_libraries(sep_engine
    sep_api
    sep_memory
    sep_quantum
    sep_core
    sep_audio
    sep_blender
    sep_compat
    ${CURL_LIBRARIES}
    ${CUDA_LIBRARIES}
    ${HIREDIS_LIBRARIES}
    fmt::fmt
    spdlog::spdlog
)
