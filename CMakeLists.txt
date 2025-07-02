cmake_minimum_required(VERSION 3.15)
project(sep_engine VERSION 1.0.0 LANGUAGES CXX)

# Add cmake directory to module path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Include unified configuration module
include(SEPConfig)
configure_sep()

# Find required packages
find_package(Threads REQUIRED)

# Add subdirectories
add_subdirectory(src/core)
add_subdirectory(src/compat)
add_subdirectory(src/memory)
add_subdirectory(src/quantum)
add_subdirectory(src/api)
add_subdirectory(src/audio)
add_subdirectory(src/blender)
add_subdirectory(extern/cuew_stub)
add_subdirectory(extern/openpgl_stub)

# Main executable
add_executable(sep_engine src/main.cpp)

target_include_directories(sep_engine
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(sep_engine
    PRIVATE
        sep_api
        sep_core
        sep_compat
        sep_memory
        sep_quantum
        sep_audio
        sep_blender
        ${CURL_LIBRARIES}
        fmt::fmt
        spdlog::spdlog
        glog
        gflags
        embree4
        cuew_stub
        openpgl_stub
)
