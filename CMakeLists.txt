# Core CUDA dependencies for all targets
set(SEP_CORE_LIBS
    ${CUDAToolkit_LIBRARIES}
    ${Threads_LIBRARIES}
    ${CMAKE_DL_LIBS}
    pthread
    dl
    rt
    m
    z
)

cmake_minimum_required(VERSION 3.15)

# Enable POSIX features for popen/pclose
add_compile_definitions(_POSIX_C_SOURCE=200809L)
add_definitions(-D_GNU_SOURCE)
add_compile_definitions(GLM_ENABLE_EXPERIMENTAL)
# Also propagate POSIX feature macros to all targets
add_compile_definitions(_POSIX_C_SOURCE=200809L)
add_compile_definitions(GLM_ENABLE_EXPERIMENTAL)

# Load CUDA toolchain file before project declaration
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/cuda-toolchain.cmake)

# Set C++ standard before project declaration
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_definitions(GLM_ENABLE_EXPERIMENTAL)

project(sep_engine VERSION 1.0.0 LANGUAGES C CXX)

# Enable testing at the top-level so subdirectories can register tests
include(CTest)
enable_testing()

# Core required components
set(SEP_HAS_CUDA ON CACHE BOOL "CUDA support (required)" FORCE)
set(CMAKE_GENERATOR "Ninja" CACHE STRING "Generator to use" FORCE)

# Set processor affinity for CUDA orchestration
add_compile_definitions(SEP_DEDICATED_CORE_ORCHESTRATION=1)
add_compile_definitions(SEP_GPU_EXCLUSIVE_ACCESS=1)

# Options to control optional modules
option(SEP_WITH_OPENSUBDIV "Enable OpenSubdiv support" OFF)
option(SEP_WITH_OPENPGL "Enable OpenPGL support" OFF)
option(SEP_WITH_OCIO "Enable OpenColorIO support" OFF)
option(SEP_WITH_OPENIMAGEIO "Enable OpenImageIO support" OFF)
option(SEP_WITH_EMBREE "Enable Embree support" OFF)
option(SEP_WITH_OSL "Enable OpenShadingLanguage support" OFF)
option(SEP_WITH_ALEMBIC "Enable Alembic support" OFF)
option(SEP_WORKBENCH_DEMO "Build sep_workbench in demo mode" OFF)
option(SEP_BUILD_ENGINE "Build the main engine component" ON)
option(SEP_BUILD_QUANTUM "Build quantum processing components" ON)
option(SEP_BUILD_TESTS "Build test components" ON)
option(SEP_BUILD_WORKBENCH "Build the interactive sep_workbench visualization" OFF)
option(SEP_USE_TBB "Enable TBB parallel features" ON)
option(SEP_HAS_EXCEPTIONS "Enable C++ exceptions" ON)
option(SEP_MINIMAL "Enable minimal build with core components only" ON)

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

# Disable previously required components
set(SEP_ENABLE_BLENDER OFF)
set(SEP_ENABLE_AUDIO OFF)
set(SEP_HAS_CYCLES 0)

# Set CUDA compiler flags before enabling the language
set(CMAKE_CUDA_FLAGS "-Xcompiler -fno-gnu-unique -Wno-deprecated-gpu-targets")
set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} --use_fast_math --expt-relaxed-constexpr")
set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -include ${CMAKE_SOURCE_DIR}/src/compat/cuda_unified_fix.h")
set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -D_GLIBCXX_USE_CXX11_ABI=1")
set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -DSEP_CUDACC_DISABLE_EXCEPTION_SPEC_CHECKS=1")

# Set C++ standard before enabling CUDA
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CUDA_STANDARD 17)
set(CMAKE_CUDA_STANDARD_REQUIRED ON)

# Add global definitions needed for CUDA compatibility
add_definitions(-DCUDA_UNIFIED_FIX_ENABLE_MATH_STUBS=1)
add_definitions(-DCUDA_UNIFIED_FIX_ENABLE_FUNCTION_RENAMING=1)
add_definitions(-D_GLIBCXX_USE_CXX11_ABI=1)
add_definitions(-DSEP_CUDACC_DISABLE_EXCEPTION_SPEC_CHECKS=1)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Add cmake directory to module path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Add library search paths
link_directories(${CMAKE_SOURCE_DIR}/lib)

# Set strict compiler flags
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # Temporarily disable -Werror to allow partial builds for testing during fixes
    add_compile_options(-Wall -Wextra)
    # Uncomment the line below when all issues are fixed
    # add_compile_options(-Wall -Wextra -Werror)
endif()

# Include unified configuration module
include(SEPConfig)
configure_sep()

if(NOT SEP_HAS_EXCEPTIONS)
    add_compile_options(-fno-exceptions)
endif()

# CUDA is now a required component
find_package(CUDAToolkit REQUIRED)
add_compile_definitions(SEP_HAS_CUDA=1)

# Ensure our custom Crow wrappers take precedence over the third-party headers
include_directories(BEFORE
    ${CMAKE_SOURCE_DIR}/extern/crow/include
    ${CMAKE_SOURCE_DIR}/extern/crow/src
    ${CMAKE_SOURCE_DIR}/src
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
    # GLFW package is typically exported as glfw3 on many systems
    find_package(glfw3 REQUIRED)
    # Use our custom FindGLEW.cmake module
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

# Build ImGui from the bundled third_party source
add_subdirectory(third_party/imgui)

# Add CUDA-specific optimizations for dedicated GPU access
add_compile_options(-DSEP_CUDA_DEDICATED_GPU=1)
add_compile_options(-DSEP_CUDA_CORE_AFFINITY=1)

# Set CUDA compute capability for specific GPU architecture
# These are reasonable defaults for modern NVIDIA GPUs
set(CMAKE_CUDA_ARCHITECTURES "75;80;86" CACHE STRING "CUDA architectures")

# Add the src directory which contains all module subdirectories and workbench
add_subdirectory(src)

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
        
    EXPORT sep-targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

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
    set(CMAKE_INSTALL_PREFIX "/usr/local" CACHE PATH "Installation prefix" FORCE)
endif()

# Add CUDA-specific core and GPU pinning
if(SEP_DEDICATED_CORE_ORCHESTRATION)
    message(STATUS "Configuring dedicated CPU core for CUDA orchestration")
    add_compile_definitions(SEP_DEDICATED_CPU_CORE=1)
endif()

if(SEP_GPU_EXCLUSIVE_ACCESS)
    message(STATUS "Configuring exclusive GPU access for CUDA operations")
    add_compile_definitions(SEP_EXCLUSIVE_GPU_ACCESS=1)
endif()
