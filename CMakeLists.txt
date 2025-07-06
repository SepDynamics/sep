cmake_minimum_required(VERSION 3.15)

# Enable POSIX features for popen/pclose
add_compile_definitions(_POSIX_C_SOURCE=200809L)
add_definitions(-D_GNU_SOURCE)
# Also propagate POSIX feature macros to all targets
add_compile_definitions(_POSIX_C_SOURCE=200809L)

# Load CUDA toolchain file before project declaration
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/cuda-toolchain.cmake)

# Set C++ standard before project declaration
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(sep_engine VERSION 1.0.0 LANGUAGES C CXX)

# Add include paths after project declaration
include_directories(BEFORE ${CMAKE_SOURCE_DIR}/include)

# Set CUDA compiler flags before enabling the language
set(CMAKE_CUDA_FLAGS "-Xcompiler -fno-gnu-unique -Wno-deprecated-gpu-targets")
set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} --use_fast_math --expt-relaxed-constexpr")
set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -include ${CMAKE_SOURCE_DIR}/include/compat/cuda_unified_fix.h")
set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -D__STRICT_ANSI__ -D_GLIBCXX_USE_CXX11_ABI=1")
set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -D_GLIBCXX_USE_NOEXCEPT_SPEC=0")
set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -DSEP_CUDACC_DISABLE_EXCEPTION_SPEC_CHECKS=1")

# Set C++ standard before enabling CUDA
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CUDA_STANDARD 17)
set(CMAKE_CUDA_STANDARD_REQUIRED ON)

# Add global definitions needed for CUDA compatibility
add_definitions(-DCUDA_UNIFIED_FIX_ENABLE_MATH_STUBS=1)
add_definitions(-DCUDA_UNIFIED_FIX_ENABLE_FUNCTION_RENAMING=1)
add_definitions(-D__STRICT_ANSI__)
add_definitions(-D_GLIBCXX_USE_CXX11_ABI=1)
add_definitions(-D_GLIBCXX_USE_NOEXCEPT_SPEC=0)
add_definitions(-DSEP_CUDACC_DISABLE_EXCEPTION_SPEC_CHECKS=1)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Add cmake directory to module path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
# Use Cycles' custom CMake modules for dependencies like OSL and Alembic.
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/extern/cycles/src/cmake/Modules")

# Add library search paths
link_directories(${CMAKE_SOURCE_DIR}/lib)

# Include unified configuration module
include(SEPConfig)
configure_sep()

# CUDA is a hard requirement for the project
find_package(CUDAToolkit REQUIRED)
set(SEP_HAS_CUDA 1)
add_compile_definitions(SEP_HAS_CUDA=1)

# Blender integration is always built
# Blender integration is disabled for the console workbench
set(SEP_HAS_BLENDER 0)

# Ensure our custom Crow wrappers take precedence over the third-party headers
include_directories(BEFORE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/extern/crow/include
)
add_compile_definitions(CROW_USE_BOOST)


# Find required packages
find_package(Threads REQUIRED)
find_package(CURL REQUIRED)
find_package(fmt REQUIRED)
find_package(http_parser REQUIRED)
find_package(spdlog REQUIRED)
find_package(OpenVDB REQUIRED)
find_package(OpenSubdiv REQUIRED)
find_package(OpenPGL REQUIRED)
find_package(OpenColorIO REQUIRED)
find_package(Imath REQUIRED)
find_package(TBB REQUIRED)
find_package(OpenImageIO REQUIRED)
find_package(embree REQUIRED)
find_package(OSL REQUIRED)
find_package(Alembic REQUIRED)

# Audio support can be disabled
option(SEP_ENABLE_AUDIO "Enable audio support" ON)
# Invoke cmake with `-DSEP_ENABLE_AUDIO=OFF` to build without audio capture

# Add subdirectories
add_subdirectory(src/core)
add_subdirectory(src/compat)
add_subdirectory(src/memory)
add_subdirectory(src/quantum)
add_subdirectory(src/embeddings)
add_subdirectory(src/api)
if(SEP_ENABLE_AUDIO)
    add_subdirectory(src/audio)
    add_compile_definitions(SEP_HAS_AUDIO=1)
endif()
# add_subdirectory(src/blender) # Temporarily disabled to isolate build issues
add_subdirectory(extern/cycles/third_party/cuew)
add_subdirectory(extern/cycles/third_party/hipew)
add_subdirectory(extern/cycles/third_party/sky)

# Build Cycles from source when prebuilt libraries are not available
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/lib/libcycles_device.a")
  message(STATUS "Prebuilt Cycles libraries not found; building from source")
  set(WITH_CYCLES_DEVICE_HIP OFF CACHE BOOL "Disable HIP" FORCE)
  set(WITH_CYCLES_DEVICE_HIPRT OFF CACHE BOOL "Disable HIPRT" FORCE)
  add_subdirectory(extern/cycles)
endif()

# Consolidated examples are built in a single workbench target
add_subdirectory(examples/workbench)

# Main executable
# Minimal CLI to test the core engine logic
add_executable(sep_engine_cli src/main.cpp)
target_include_directories(sep_engine_cli PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(sep_engine_cli PRIVATE
    sep_core
    sep_memory
    sep_quantum
    sep_compat
)

# Export and install targets
include(GNUInstallDirs)

# Install headers
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/sep
    FILES_MATCHING
    PATTERN "*.h"
    PATTERN "*.hpp"
)

# Install libraries
install(TARGETS
    sep_core
    sep_compat
    sep_memory
    sep_quantum
    sep_embeddings
    sep_audio
    sep_api
    EXPORT sep-targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install executable separately

# Install CMake config files
install(EXPORT sep-targets
    NAMESPACE sep::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/sep
)

# Generate and install CMake config file
configure_file(
    ${CMAKE_SOURCE_DIR}/cmake/sep-config.cmake.in
    ${CMAKE_BINARY_DIR}/sep-config.cmake
    @ONLY
)
install(FILES
    ${CMAKE_BINARY_DIR}/sep-config.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/sep
)

# Install headers from include directory
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/
    DESTINATION include/sep
    FILES_MATCHING
    PATTERN "*.h"
    PATTERN "*.hpp"
)

# Set install prefix if not specified
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "/usr/local" CACHE PATH "Installation prefix" FORCE

        # 2. Cycles and its dependencies (order is important here too)
        # Link the main Cycles libs first
        cycles_session
        cycles_scene
        cycles_device
        cycles_kernel_osl
        cycles_subd
        cycles_util
        extern_sky
        extern_cuew
        extern_hipew
        extern_sky

        # Now link the external dependencies that Cycles needs
        OpenImageIO
        ${OPENVDB_LIBRARIES}
        ${OPENSUBDIV_LIBRARIES}
        openvdb
        bf_intern_opensubdiv
        ${OSL_LIBRARIES} # Link the real OSL libs, not the stub
        ${TBB_LIBRARIES}

        # 3. Other direct dependencies
        ${CURL_LIBRARIES}
        http_parser::http_parser
        fmt::fmt
        spdlog::spdlog
        hiredis
        pipewire-0.3

        # 4. System libraries
        ${EMBREE_LIBRARIES}
        Imath
        ${OpenPGL_LIBRARIES}
        ${OpenColorIO_LIBRARIES}
        ${ALEMBIC_LIBRARIES}
        ${CMAKE_DL_LIBS}
        ${Threads_LIBRARIES}
        ${CUDAToolkit_LIBRARIES}
        pthread
        dl
        rt
        m
        z
)
endif()
