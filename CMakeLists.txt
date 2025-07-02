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
find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)
find_path(HTTP_PARSER_INCLUDE_DIR http_parser.h)
find_library(HTTP_PARSER_LIBRARY http_parser)
if(HTTP_PARSER_INCLUDE_DIR AND HTTP_PARSER_LIBRARY)
    add_library(http_parser::http_parser INTERFACE IMPORTED)
    set_target_properties(http_parser::http_parser PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES ${HTTP_PARSER_INCLUDE_DIR}
        INTERFACE_LINK_LIBRARIES ${HTTP_PARSER_LIBRARY}
    )
else()
    message(FATAL_ERROR "http_parser library not found")
endif()

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
        http_parser::http_parser
        fmt::fmt
        spdlog::spdlog
        glog
        gflags
        embree4
)
