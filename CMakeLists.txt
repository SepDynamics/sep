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

# Find required packages
find_package(CUDAToolkit REQUIRED)
find_package(GTest REQUIRED)
find_package(fmt REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(PIPEWIRE REQUIRED libpipewire-0.3)

# Set CMake policies for Cycles build
cmake_policy(SET CMP0002 NEW)
cmake_policy(SET CMP0079 NEW)
# Disable unneeded components from the bundled Cycles build
set(WITH_CYCLES_STANDALONE OFF CACHE BOOL "Disable Cycles standalone" FORCE)
set(WITH_CYCLES_STANDALONE_GUI OFF CACHE BOOL "Disable Cycles GUI" FORCE)
set(WITH_CYCLES_PRECOMPUTE OFF CACHE BOOL "Disable Cycles precompute tool" FORCE)
set(WITH_CYCLES_CUDA_BINARIES OFF CACHE BOOL "Disable Cycles CUDA kernels" FORCE)
set(WITH_CYCLES_HIP_BINARIES OFF CACHE BOOL "Disable Cycles HIP kernels" FORCE)
set(WITH_CYCLES_ONEAPI_BINARIES OFF CACHE BOOL "Disable Cycles oneAPI kernels" FORCE)
set(WITH_CYCLES_HYDRA_RENDER_DELEGATE OFF CACHE BOOL "Disable Cycles Hydra" FORCE)
set(WITH_GTESTS OFF CACHE BOOL "Disable Cycles GTests" FORCE)
set(BUILD_TESTING OFF CACHE BOOL "Disable Cycles tests" FORCE)

# Disable standalone Cycles executables and tests
set(WITH_CYCLES_STANDALONE OFF CACHE BOOL "Disable Cycles standalone executable" FORCE)
set(WITH_CYCLES_STANDALONE_GUI OFF CACHE BOOL "Disable Cycles standalone GUI" FORCE)
set(WITH_CYCLES_PRECOMPUTE OFF CACHE BOOL "Disable Cycles precompute executable" FORCE)
set(WITH_CYCLES_HYDRA_RENDER_DELEGATE OFF CACHE BOOL "Disable Cycles Hydra delegate" FORCE)
set(WITH_GTESTS OFF CACHE BOOL "Disable Cycles tests" FORCE)

# Add Cycles source directory with custom binary dir and target prefix
set(CYCLES_TARGET_PREFIX "cycles_ext_" CACHE STRING "Prefix for Cycles targets")

# Disable standalone executables and heavy kernel build targets from Cycles.
set(WITH_CYCLES_STANDALONE OFF CACHE BOOL "Disable standalone Cycles executable" FORCE)
set(WITH_CYCLES_STANDALONE_GUI OFF CACHE BOOL "Disable Cycles standalone GUI" FORCE)
set(WITH_CYCLES_PRECOMPUTE OFF CACHE BOOL "Disable Cycles precompute executable" FORCE)
set(WITH_CYCLES_CUDA_BINARIES OFF CACHE BOOL "Disable precompiled CUDA kernels" FORCE)
set(WITH_CYCLES_HIP_BINARIES OFF CACHE BOOL "Disable precompiled HIP kernels" FORCE)
set(WITH_CYCLES_ONEAPI_BINARIES OFF CACHE BOOL "Disable precompiled oneAPI kernels" FORCE)

add_subdirectory(extern/cycles ${CMAKE_BINARY_DIR}/cycles-build EXCLUDE_FROM_ALL)

# Use Cycles' include directories for NanoVDB and OpenImageDenoise
include_directories(
    ${CMAKE_SOURCE_DIR}/extern/cycles/third_party/nanovdb
    ${CMAKE_SOURCE_DIR}/extern/cycles/third_party/OpenImageDenoise/include
)

# Set CMake module path for OpenVDB
list(APPEND CMAKE_MODULE_PATH "/usr/lib64/cmake/OpenVDB")

# Find dependencies
find_package(OpenVDB REQUIRED)
find_package(OpenColorIO REQUIRED)
find_package(OpenSubdiv REQUIRED)

# Set all feature flags to enabled
set(WITH_CYCLES_NANOVDB 1)
set(WITH_CYCLES_OPENIMAGEDENOISE 1)
set(WITH_CYCLES_OPENVDB 1)
set(WITH_CYCLES_OPENCOLORIO 1)
set(WITH_CYCLES_OPENSUBDIV 1)

# Ensure Cycles uses our enabled features
set(WITH_NANOVDB 1)
set(WITH_OPENIMAGEDENOISE 1)
set(WITH_OPENVDB 1)
set(WITH_OPENCOLORIO 1)
set(WITH_OPENSUBDIV 1)

# Report found dependencies
message(STATUS "NanoVDB found: ${NANOVDB_INCLUDE_DIR}")
message(STATUS "OpenImageDenoise found: ${OpenImageDenoise_ROOT}")
message(STATUS "OpenVDB found: ${OpenVDB_ROOT}")
message(STATUS "OpenColorIO found: ${OpenColorIO_ROOT}")
message(STATUS "OpenSubdiv found: ${OpenSubdiv_ROOT}")

# Make custom CMake modules available
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

# Set C++ standard
include(SetCXXStandard)

# Add subdirectories for main project components
add_subdirectory(src)

# Add tests directory
add_subdirectory(tests)

# Configure test data directory
set(SEP_TEST_DATA_DIR ${CMAKE_SOURCE_DIR}/tests/test_data)
file(MAKE_DIRECTORY ${SEP_TEST_DATA_DIR})

# Force all features enabled
set(SEP_BUILD_TESTS 1)
set(SEP_ENABLE_CUDA 1)
set(SEP_HAS_BLENDER 1)
set(SEP_HAS_CUDA 1)
set(SEP_HAS_CYCLES 1)

# Configure Blender paths
set(BLENDER_VERSION "5.0")
set(BLENDER_USER_CONFIG_DIR "$ENV{HOME}/.config/blender/${BLENDER_VERSION}")

# Set all feature flags
add_compile_definitions(
    SEP_BUILD_TESTS=1
    SEP_ENABLE_CUDA=1
    SEP_HAS_BLENDER=1
    SEP_HAS_CUDA=1
    SEP_HAS_CYCLES=1
    WITH_CYCLES_NANOVDB=1
    WITH_CYCLES_OPENIMAGEDENOISE=1
    WITH_CYCLES_OPENVDB=1
    WITH_CYCLES_OPENCOLORIO=1
    WITH_CYCLES_OPENSUBDIV=1
)

# Always enable debug info
add_compile_definitions(SEP_DEBUG=1)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Configure CUDA runtime linking
set(CMAKE_CUDA_RUNTIME_LIBRARY "Static")
