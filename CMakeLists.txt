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

# Enable testing at the top-level so subdirectories can register tests
include(CTest)
enable_testing()

# Options to control optional modules. These allow building a minimal engine
# without the heavyweight integrations.
option(SEP_WITH_CYCLES "Enable Blender Cycles integration" OFF)
option(SEP_WITH_OPENSUBDIV "Enable OpenSubdiv support" ON)
option(SEP_WITH_OPENPGL "Enable OpenPGL support" ON)
option(SEP_WITH_OCIO "Enable OpenColorIO support" ON)
option(SEP_WITH_OPENIMAGEIO "Enable OpenImageIO support" ON)
option(SEP_WITH_EMBREE "Enable Embree support" ON)
option(SEP_WITH_OSL "Enable OpenShadingLanguage support" ON)
option(SEP_WITH_ALEMBIC "Enable Alembic support" ON)
option(SEP_WITH_AUDIO "Enable PipeWire audio integration" ON)
option(SEP_WORKBENCH_DEMO "Build sep_workbench in demo mode" ON)
option(SEP_BUILD_ENGINE "Build the main engine component" ON)
option(SEP_BUILD_QUANTUM "Build quantum processing components" ON)
option(SEP_BUILD_TESTS "Build test components" ON)
option(SEP_BUILD_WORKBENCH "Build the interactive sep_workbench visualization" ON)
option(SEP_USE_TBB "Enable TBB parallel features" OFF)

if(SEP_MINIMAL)
    add_compile_definitions(SEP_MINIMAL=1)
endif()

# Configure include paths
set(SEP_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/src)
set(SEP_SRC_DIR ${CMAKE_SOURCE_DIR}/src)

# Add include paths after project declaration
include_directories(BEFORE
    ${SEP_INCLUDE_DIR}
    ${SEP_SRC_DIR}
    $<BUILD_INTERFACE:/usr/src>
)

# Ensure core headers are available
# Build the core subsystem first so that memory and other components can
# link against it. The previous invocation attempted to use the unsupported
# `BEFORE` argument to `add_subdirectory`, which caused configuration errors
# with newer versions of CMake. We simply add the directories in the
# required order instead.

# Backwards compatibility variables
set(SEP_ENABLE_BLENDER ${SEP_WITH_CYCLES})
set(SEP_ENABLE_AUDIO ${SEP_WITH_AUDIO})

set(SEP_HAS_CYCLES 0)
if(SEP_WITH_CYCLES)
    set(SEP_HAS_CYCLES 1)
endif()

# Set CUDA compiler flags before enabling the language
set(CMAKE_CUDA_FLAGS "-Xcompiler -fno-gnu-unique -Wno-deprecated-gpu-targets")
set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} --use_fast_math --expt-relaxed-constexpr")
set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -include ${CMAKE_SOURCE_DIR}/src/compat/cuda_unified_fix.h")
set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -D__STRICT_ANSI__ -D_GLIBCXX_USE_CXX11_ABI=1")
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
add_definitions(-DSEP_CUDACC_DISABLE_EXCEPTION_SPEC_CHECKS=1)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Add cmake directory to module path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
# Use Cycles' custom CMake modules for dependencies like OSL and Alembic.
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/extern/cycles/src/cmake/Modules")

# Add library search paths
link_directories(${CMAKE_SOURCE_DIR}/lib)

# Set strict compiler flags
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Werror)
endif()

# Include unified configuration module
include(SEPConfig)
configure_sep()

# CUDA is optional for many unit tests. Allow disabling via SEP_HAS_CUDA
option(SEP_HAS_CUDA "Enable CUDA support" ON)
if(SEP_HAS_CUDA)
    find_package(CUDAToolkit REQUIRED)
    add_compile_definitions(SEP_HAS_CUDA=1)
else()
    add_compile_definitions(SEP_HAS_CUDA=0)
endif()

if(SEP_WITH_CYCLES)
    add_compile_definitions(SEP_HAS_CYCLES=1)
    add_compile_definitions(SEP_HAS_BLENDER=1)
else()
    add_compile_definitions(SEP_HAS_CYCLES=0)
    add_compile_definitions(SEP_HAS_BLENDER=0)
endif()

# Ensure our custom Crow wrappers take precedence over the third-party headers
include_directories(BEFORE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/extern/crow/src
)
add_compile_definitions(CROW_USE_BOOST)


# Find required packages
find_package(Threads REQUIRED)
find_package(CURL REQUIRED)
find_package(fmt REQUIRED)
find_package(http_parser REQUIRED)
find_package(glm REQUIRED)
find_package(spdlog)
find_package(OpenGL REQUIRED)
if(SEP_BUILD_WORKBENCH)
    find_package(GLFW REQUIRED)
    find_package(GLEW REQUIRED)
endif()
if(spdlog_FOUND)
    message(STATUS "Using system spdlog")
else()
    message(WARNING "spdlog not found, falling back to stub implementation")
    add_compile_definitions(SEP_SPDLOG_FALLBACK)
endif()
find_package(OpenVDB REQUIRED)
if(SEP_WITH_OPENSUBDIV)
    find_package(OpenSubdiv REQUIRED)
endif()
if(SEP_WITH_OPENPGL)
    find_package(OpenPGL REQUIRED)
endif()
if(SEP_WITH_OCIO)
    find_package(OpenColorIO REQUIRED)
endif()
find_package(Imath REQUIRED)

# Set TBB_ROOT to prefer the newer TBB 12.x version only when a valid path exists
if(NOT DEFINED ENV{TBB_ROOT})
  if(EXISTS "/home/ajn/Documents/AVarietyOShit/BUILTASF/tbb")
    set(ENV{TBB_ROOT} "/home/ajn/Documents/AVarietyOShit/BUILTASF/tbb")
  elseif(EXISTS "/home/ajn/Documents/AVarietyOShit/BUILTASF/oneTBB-2021.9.0/build/gnu_12.3_cxx11_64_release")
    set(ENV{TBB_ROOT} "/home/ajn/Documents/AVarietyOShit/BUILTASF/oneTBB-2021.9.0/build/gnu_12.3_cxx11_64_release")
  endif()
endif()
if(SEP_USE_TBB)
    add_compile_definitions(SEP_USE_TBB)
    find_package(TBB REQUIRED)
endif()
if(SEP_WITH_OPENIMAGEIO)
    find_package(OpenImageIO REQUIRED)
endif()
if(SEP_WITH_EMBREE)
    find_package(embree REQUIRED)
endif()
if(SEP_WITH_OSL)
    find_package(OSL REQUIRED)
endif()
if(SEP_WITH_ALEMBIC)
    find_package(Alembic REQUIRED)
endif()

# Add the src directory which contains all module subdirectories and workbench
add_subdirectory(src)
if(SEP_WITH_CYCLES)
    add_subdirectory(extern/cycles/third_party/cuew)
    add_subdirectory(extern/cycles/third_party/hipew)
    add_subdirectory(extern/cycles/third_party/sky)
endif()


# Build Cycles from source when prebuilt libraries are not available
if(SEP_WITH_CYCLES)
  if(NOT EXISTS "${CMAKE_SOURCE_DIR}/lib/libcycles_device.a")
    message(STATUS "Prebuilt Cycles libraries not found; building from source")
    set(WITH_CYCLES_DEVICE_HIP OFF CACHE BOOL "Disable HIP" FORCE)
    set(WITH_CYCLES_DEVICE_HIPRT OFF CACHE BOOL "Disable HIPRT" FORCE)
    add_subdirectory(extern/cycles)
  endif()
endif()

# Consolidated examples are built in a single workbench target

# Main executable is defined in src/CMakeLists.txt

# Only link Cycles libraries if they exist and SEP_WITH_CYCLES is enabled


# Export and install targets
include(GNUInstallDirs)

# Install headers
install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/
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
    sep_manifold
    EXPORT sep-targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

if(TARGET sep_audio)
    install(TARGETS sep_audio
        EXPORT sep-targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif()

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
install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/
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
