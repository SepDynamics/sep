cmake_minimum_required(VERSION 3.15)
project(sep_engine VERSION 1.0.0 LANGUAGES CXX)

# Configure CUDA
set(CMAKE_CUDA_STANDARD 17)
set(CMAKE_CUDA_STANDARD_REQUIRED ON)
set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -allow-unsupported-compiler")

# Find required packages
find_package(CUDAToolkit REQUIRED)
find_package(GTest REQUIRED)
find_package(fmt REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(PIPEWIRE REQUIRED libpipewire-0.3)

# Detect optional libraries for feature macros
find_path(NANOVDB_INCLUDE_DIR
    NAMES nanovdb/NanoVDB.h
    PATH_SUFFIXES include)
if(NANOVDB_INCLUDE_DIR)
    message(STATUS "NanoVDB found: ${NANOVDB_INCLUDE_DIR}")
    add_compile_definitions(WITH_NANOVDB=1)
else()
    message(STATUS "NanoVDB not found")
    add_compile_definitions(WITH_NANOVDB=0)
endif()

find_path(OPENIMAGEDENOISE_INCLUDE_DIR
    NAMES OpenImageDenoise/oidn.hpp
    PATH_SUFFIXES include)
if(OPENIMAGEDENOISE_INCLUDE_DIR)
    message(STATUS "OpenImageDenoise found: ${OPENIMAGEDENOISE_INCLUDE_DIR}")
    add_compile_definitions(WITH_OPENIMAGEDENOISE=1)
else()
    message(STATUS "OpenImageDenoise not found")
    add_compile_definitions(WITH_OPENIMAGEDENOISE=0)
endif()

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add subdirectories for main project components
add_subdirectory(src)

# Add tests directory
add_subdirectory(tests)

# Configure test data directory
set(SEP_TEST_DATA_DIR ${CMAKE_SOURCE_DIR}/tests/test_data)
file(MAKE_DIRECTORY ${SEP_TEST_DATA_DIR})

# Set build options with dependencies
set(SEP_BUILD_TESTS ON CACHE BOOL "Build test suite")
set(SEP_ENABLE_CUDA ON CACHE BOOL "Enable CUDA support")
set(SEP_HAS_BLENDER ON CACHE BOOL "Build Blender integration")

# Configure Blender paths
if(SEP_HAS_BLENDER)
    set(BLENDER_VERSION "5.0")
    set(BLENDER_USER_CONFIG_DIR "$ENV{HOME}/.config/blender/${BLENDER_VERSION}")
    add_compile_definitions(SEP_HAS_BLENDER=1)
endif()

# Configure feature flags
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(SEP_DEBUG=1)
endif()

add_compile_definitions(
    SEP_HAS_CUDA=1
    SEP_HAS_BLENDER=1
    SEP_HAS_CYCLES=1
)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Configure CUDA runtime linking
set(CMAKE_CUDA_RUNTIME_LIBRARY "Static")