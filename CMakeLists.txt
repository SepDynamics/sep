cmake_minimum_required(VERSION 3.15)
project(sep_engine VERSION 1.0.0 LANGUAGES CXX)

# Install required dependencies
execute_process(
    COMMAND sudo dnf5 install -y openvdb-devel
)

# Configure CUDA
set(CMAKE_CUDA_STANDARD 17)
set(CMAKE_CUDA_STANDARD_REQUIRED ON)
set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -allow-unsupported-compiler")

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
    set(HIREDIS_FOUND TRUE)
else()
    set(HIREDIS_FOUND FALSE)
    message(WARNING "Hiredis not found, Redis persistence disabled")
endif()

if(CUDAToolkit_FOUND)
    set(SEP_CUDA_AVAILABLE 1)
else()
  message(WARNING "Cycles submodule not found, disabling Cycles integration")
  set(SEP_HAS_CYCLES 0)
endif()

# Use Cycles' include directories for NanoVDB and OpenImageDenoise
if(SEP_HAS_CYCLES)
  include_directories(
      ${CMAKE_SOURCE_DIR}/extern/cycles/third_party/nanovdb
      ${CMAKE_SOURCE_DIR}/extern/cycles/third_party/OpenImageDenoise/include
  )
endif()

set(CUDA_SOURCES
    core/core.cu
    cuda_api.cu
    event.cu
    pattern_kernels.cu
    quantum_kernels.cu
    utils.cu
)

add_subdirectory(src/api)
add_subdirectory(src/audio)
add_subdirectory(src/blender)
add_subdirectory(src/compat)
add_subdirectory(src/core)
add_subdirectory(src/memory)
add_subdirectory(src/quantum)

add_executable(sep_engine src/main.cpp)

target_include_directories(sep_engine
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)
if(HIREDIS_FOUND)
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
    fmt::fmt
    spdlog::spdlog
)
