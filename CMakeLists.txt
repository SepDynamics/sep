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

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add subdirectories for main project components
add_subdirectory(src)

# Add tests directory
add_subdirectory(tests)

# Configure test data directory
set(SEP_TEST_DATA_DIR ${CMAKE_SOURCE_DIR}/tests/test_data)
file(MAKE_DIRECTORY ${SEP_TEST_DATA_DIR})

# Set build options
option(SEP_BUILD_TESTS "Build test suite" ON)
option(SEP_ENABLE_CUDA "Enable CUDA support" ON)
option(SEP_HAS_CYCLES "Enable Blender Cycles integration" OFF)
option(SEP_HAS_BLENDER "Enable Blender integration" ON)

# Configure compiler flags
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(SEP_DEBUG)
endif()

if(SEP_ENABLE_CUDA)
    add_compile_definitions(SEP_HAS_CUDA)
endif()

if(SEP_HAS_CYCLES)
    add_compile_definitions(SEP_HAS_CYCLES)
endif()
if(SEP_HAS_BLENDER)
    add_compile_definitions(SEP_HAS_BLENDER=1)
else()
    add_compile_definitions(SEP_HAS_BLENDER=0)
endif()

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Configure CUDA runtime linking
set(CMAKE_CUDA_RUNTIME_LIBRARY "Static")