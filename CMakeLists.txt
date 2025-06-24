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


# This ensures our fixed headers are used instead of the problematic ones
include_directories(BEFORE
  ${CMAKE_SOURCE_DIR}/include
)

# Add our fixed Crow headers subdirectory
add_subdirectory(include/crow)

include_directories(
  ${CMAKE_SOURCE_DIR}/third_party
  ${CMAKE_SOURCE_DIR}/third_party/glm
  ${CMAKE_SOURCE_DIR}/third_party/tbb
)

# Fetch the Crow web framework if it is not present
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/third_party/crow/include")
    message(STATUS "Fetching Crow framework...")
    FetchContent_Declare(
        crow
        GIT_REPOSITORY https://github.com/CrowCpp/Crow.git
        GIT_TAG v1.2.1.2
    )
    FetchContent_MakeAvailable(crow)
    include_directories(${crow_SOURCE_DIR}/include)
else()
    include_directories(${CMAKE_SOURCE_DIR}/third_party/crow/include)
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


target_include_directories(sep_engine
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)


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
