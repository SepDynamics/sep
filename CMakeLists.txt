cmake_minimum_required(VERSION 3.15)
project(sep_engine VERSION 1.0.0 LANGUAGES CXX)

# Add custom cmake module path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Default build type if not specified
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find required packages
find_package(Threads REQUIRED)
find_package(fmt REQUIRED)
find_package(TBB REQUIRED)
find_package(benchmark REQUIRED)
find_package(nlohmann_json REQUIRED)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party
)

# Set policy for CUDA modules
cmake_policy(SET CMP0146 NEW)

# Add compiler definition to bypass CUDA host compiler check
add_compile_definitions(__NV_NO_HOST_COMPILER_CHECK=1)

# Set CUDA flags for the host compiler check bypass
set(CMAKE_CUDA_FLAGS "-D__NV_NO_HOST_COMPILER_CHECK=1")

# CUDA configuration
option(SEP_USE_CUDA "Enable CUDA support" ON)
if(SEP_USE_CUDA)
    enable_language(CUDA)
    if(CMAKE_CUDA_COMPILER)
        message(STATUS "CUDA compiler found: ${CMAKE_CUDA_COMPILER}")
        set(CMAKE_CUDA_STANDARD 17)
        set(CMAKE_CUDA_STANDARD_REQUIRED ON)
        
        # For compatibility with existing code
        set(SEP_HAS_CUDA ${SEP_USE_CUDA})
        add_definitions(-DSEP_USE_CUDA=1 -DSEP_HAS_CUDA=1)
        
        # Create targets compatible with old CUDA::* targets
        add_library(CUDA::cudart INTERFACE IMPORTED)
        add_library(CUDA::cuda_driver INTERFACE IMPORTED)
        add_library(CUDA::cufft INTERFACE IMPORTED)
        
        # Set the include directories and link libraries
        target_link_libraries(CUDA::cudart INTERFACE CUDA::cudart_static)
        target_link_libraries(CUDA::cuda_driver INTERFACE ${CMAKE_CUDA_LIBRARIES})
        target_link_libraries(CUDA::cufft INTERFACE CUDA::cufft_static)
    else()
        message(WARNING "CUDA requested but compiler not found, disabling CUDA support")
        set(SEP_USE_CUDA OFF)
        set(SEP_HAS_CUDA OFF)
        add_definitions(-DSEP_USE_CUDA=0 -DSEP_HAS_CUDA=0)
    endif()
endif()

# Add subdirectories
add_subdirectory(src)
add_subdirectory(examples)

# Set build output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Create the export set
set(SEP_TARGETS
    sep_core
    sep_memory
    sep_quantum
    sep_compat
)

# Install configuration
include(GNUInstallDirs)
configure_file(cmake/sep-config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/sep-config.cmake" @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/sep-config.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/sep)

# Export the targets (define the export)
install(TARGETS ${SEP_TARGETS}
    EXPORT sep-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install the export
install(EXPORT sep-targets
    FILE sep-targets.cmake
    NAMESPACE sep::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/sep)
