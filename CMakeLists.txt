cmake_minimum_required(VERSION 3.15)

# Set up CUDA BEFORE project()
option(SEP_USE_CUDA "Enable CUDA support" ON)

if(SEP_USE_CUDA)
    # Set the host compiler for CUDA. This is the standard and correct way.
    # Using clang++-15 as specified in the original wrapper script's intent.
    set(CMAKE_CUDA_HOST_COMPILER "/usr/bin/clang++-15")
    set(CMAKE_CUDA_ARCHITECTURES 75)
    # Add the flag to allow unsupported compiler versions
    set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} --allow-unsupported-compiler")
endif()

# NOW call project() - CUDA will use the correct compiler
project(sep_engine VERSION 1.0.0 LANGUAGES CXX CUDA)

# Add our cmake modules to the module path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

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
find_package(yaml-cpp REQUIRED)
find_package(OpenGL REQUIRED)
# Add subdirectories
add_subdirectory(third_party/imgui)
add_subdirectory(src)
add_subdirectory(examples)

add_executable(sep_workbench src/workbench/core/workbench_main.cpp)
target_link_libraries(sep_workbench PRIVATE sep_workbench_lib)


