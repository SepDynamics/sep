cmake_minimum_required(VERSION 3.15)

# Set up CUDA BEFORE project()
option(SEP_USE_CUDA "Enable CUDA support" ON)

if(SEP_USE_CUDA)
    # Set the host compiler for CUDA. This is the standard and correct way.
    # Using clang++-15 as specified in the original wrapper script's intent.
    # set(CMAKE_CUDA_HOST_COMPILER "/usr/bin/clang++-15") # Let NVCC use its default host compiler
    set(CMAKE_CUDA_ARCHITECTURES 75)
endif()

# NOW call project() - CUDA will use the correct compiler
project(sep_engine VERSION 1.0.0 LANGUAGES CXX CUDA)

# Include the template
include(cmake/template.cmake)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
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

# Add subdirectories
add_subdirectory(src)
add_subdirectory(examples)


