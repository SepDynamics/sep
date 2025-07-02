cmake_minimum_required(VERSION 3.15)
project(sep_engine VERSION 1.0.0 LANGUAGES CXX)

# Add cmake directory to module path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Include unified configuration module
include(SEPConfig)
configure_sep()

# Find required packages
find_package(Threads REQUIRED)
find_package(CURL REQUIRED)
find_library(HTTP_PARSER_LIBRARY http_parser)
if(NOT HTTP_PARSER_LIBRARY)
    message(FATAL_ERROR "http_parser library not found")
endif()
find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)

# Add subdirectories
add_subdirectory(src/core)
add_subdirectory(src/compat)
add_subdirectory(src/memory)
add_subdirectory(src/quantum)
add_subdirectory(src/api)
add_subdirectory(src/audio)
add_subdirectory(src/blender)

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
        ${EMBREE_LIBRARIES}
        ${GLOG_LIBRARIES}
        ${OIIO_LIBRARIES}
        ${ZSTD_LIBRARIES}
        ${CURL_LIBRARIES}
        fmt::fmt
        spdlog::spdlog
        glog
        gflags
        embree4
        ${HTTP_PARSER_LIBRARY}
)
