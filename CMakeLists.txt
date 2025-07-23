cmake_minimum_required(VERSION 3.15)

# Enable compile commands database generation for IDE support and static analysis
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

# Project declaration with initial C++ support
project(sep_engine VERSION 1.0.0 LANGUAGES CXX)

# Set up CUDA support
option(SEP_USE_CUDA "Enable CUDA support" ON)

if(SEP_USE_CUDA)
    # First verify CUDA paths
    if(NOT DEFINED ENV{CUDA_HOME})
        message(FATAL_ERROR "CUDA_HOME environment variable not set")
    endif()
    
    if(NOT EXISTS "$ENV{CUDA_HOME}/bin/nvcc")
        message(FATAL_ERROR "NVCC not found at $ENV{CUDA_HOME}/bin/nvcc")
    endif()
    
    # Set CUDA paths before enabling language
    set(CMAKE_CUDA_COMPILER "$ENV{CUDA_HOME}/bin/nvcc")
    set(CMAKE_CUDA_HOST_COMPILER "/usr/bin/clang++-15")
    set(CMAKE_CUDA_ARCHITECTURES 75)
    
    # Now enable CUDA language
    enable_language(CUDA)
    
    # Add flags for better diagnostics and features
    set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} --allow-unsupported-compiler --extended-lambda --display-error-number")
    
    # Print CUDA configuration for debugging
    message(STATUS "CUDA Home: $ENV{CUDA_HOME}")
    message(STATUS "CUDA Compiler: ${CMAKE_CUDA_COMPILER}")
    message(STATUS "CUDA Host Compiler: ${CMAKE_CUDA_HOST_COMPILER}")
    message(STATUS "CUDA Architectures: ${CMAKE_CUDA_ARCHITECTURES}")
    
    # Add CUDA paths to include and link directories
    include_directories("$ENV{CUDA_HOME}/include")
    link_directories("$ENV{CUDA_HOME}/lib64")
endif()

# Add our cmake modules to the module path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Include the template
include(cmake/template.cmake)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17 CACHE STRING "C++ standard")
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable testing
enable_testing()

# Find required packages
find_package(Threads REQUIRED)
find_package(fmt REQUIRED)
find_package(TBB REQUIRED)
find_package(benchmark REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(spdlog REQUIRED)
find_package(glm REQUIRED)
find_package(OpenGL REQUIRED)

include(FetchContent)
FetchContent_Declare(
  yaml-cpp
  GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
  GIT_TAG 0.8.0
)
FetchContent_MakeAvailable(yaml-cpp)
set_target_properties(yaml-cpp PROPERTIES CXX_STANDARD 17)

# Add subdirectories
# Trigger rebuild again
add_subdirectory(third_party/imgui)
add_subdirectory(src)
add_subdirectory(src/apps)

# Custom command to generate documentation
add_custom_target(generate_docs
    COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/parse_docs.py
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Generating documentation from source headers"
)


