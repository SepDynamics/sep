cmake_minimum_required(VERSION 3.15)

# Set CMAKE_LIBRARY_PATH to help find system libraries like librt
set(CMAKE_LIBRARY_PATH "/usr/lib;/usr/lib64")

# Explicitly set CUDA host compiler
set(CMAKE_CUDA_HOST_COMPILER "/usr/bin/g++-14" CACHE FILEPATH "Host compiler for CUDA" FORCE)

# Set CUDA architectures
set(CMAKE_CUDA_ARCHITECTURES "70;75;80;86;89" CACHE STRING "CUDA architectures")
set(CMAKE_CUDA_FLAGS_INIT "${CMAKE_CUDA_FLAGS_INIT} -allow-unsupported-compiler -Xcompiler -D__NV_NO_HOST_COMPILER_CHECK=1 -D_GLIBCXX_USE_CXX11_ABI=0 -Xcompiler -Wno-error=deprecated-declarations -Xcompiler -Wno-error=ignored-attributes -D__NVCC_DIAG_PRAGMA_SUPPORT__=1")
set(CMAKE_CUDA_COMPILER_ID_RUN_FLAGS "${CMAKE_CUDA_COMPILER_ID_RUN_FLAGS} -allow-unsupported-compiler -Xcompiler -D__NV_NO_HOST_COMPILER_CHECK=1 -D_GLIBCXX_USE_CXX11_ABI=0 -Xcompiler -Wno-error=deprecated-declarations -Xcompiler -Wno-error=ignored-attributes -D__NVCC_DIAG_PRAGMA_SUPPORT__=1")

# Check if CUDA is available before enabling it as a language
find_package(CUDAToolkit)
if(CUDAToolkit_FOUND)
    project(sep_engine VERSION 1.0.0 LANGUAGES CXX CUDA)
    set(SEP_USE_CUDA ON)
else()
    project(sep_engine VERSION 1.0.0 LANGUAGES CXX)
    set(SEP_USE_CUDA OFF)
endif()

# Link librt to the main project target
target_link_libraries(sep_engine PUBLIC rt)

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

# Enable testing
enable_testing()

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

# CUDA configuration
if(SEP_USE_CUDA)
    # Set CUDA standard
    set(CMAKE_CUDA_STANDARD 17)
    set(CMAKE_CUDA_STANDARD_REQUIRED ON)
    
    # Find CUDA packages
    find_package(CUDA REQUIRED)
    find_package(CUDAToolkit REQUIRED)
    
    # Set CUDA architectures
    if(NOT DEFINED CMAKE_CUDA_ARCHITECTURES)
        set(CMAKE_CUDA_ARCHITECTURES "60;70;75;80")
    endif()
    
    # Add CUDA compile options to avoid sin/cos issues
    set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} --expt-relaxed-constexpr --extended-lambda -allow-unsupported-compiler -Xcompiler -D__NV_NO_HOST_COMPILER_CHECK=1 -D_GLIBCXX_USE_CXX11_ABI=0 -Xcompiler -Wno-error=deprecated-declarations -Xcompiler -Wno-error=ignored-attributes -D__NVCC_DIAG_PRAGMA_SUPPORT__=1")
    
    # Add librt to CUDA libraries for linking
    list(APPEND CMAKE_CUDA_LIBRARIES "rt")

    # Explicitly add linker directories for librt
    link_directories(/usr/lib /usr/lib64)

    # Set CUDA architectures
    set(CMAKE_CUDA_ARCHITECTURES "70;75;80;86;89" CACHE STRING "CUDA architectures")

    # Log CUDA flags for debugging
    message(STATUS "CMAKE_CUDA_COMPILER_ID_RUN_FLAGS: ${CMAKE_CUDA_COMPILER_ID_RUN_FLAGS}")
    message(STATUS "CMAKE_CUDA_FLAGS: ${CMAKE_CUDA_FLAGS}")
endif()

# Add subdirectories
add_subdirectory(src)
add_subdirectory(examples)

# Set build output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)

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
