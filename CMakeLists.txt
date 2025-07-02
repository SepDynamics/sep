cmake_minimum_required(VERSION 3.15)
project(sep_engine VERSION 1.0.0 LANGUAGES CXX)

# Add cmake directory to module path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Include unified configuration module
include(SEPConfig)
configure_sep()

# Detect CUDA toolkit and expose SEP_HAS_CUDA for conditional builds
find_package(CUDAToolkit QUIET)
if(CUDAToolkit_FOUND AND CUDAToolkit_NVCC_EXECUTABLE)
    set(SEP_HAS_CUDA 1)
else()
    set(SEP_HAS_CUDA 0)
endif()
message(STATUS "CUDA support: ${SEP_HAS_CUDA}")

add_compile_definitions(SEP_HAS_CUDA=${SEP_HAS_CUDA})

# Disable Blender integration by default to reduce dependencies
option(SEP_HAS_BLENDER "Build Blender integration" OFF)

# Build stub implementation of the OpenPGL API to satisfy linking against
# precompiled Cycles libraries when the real OpenPGL dependency is missing.
add_subdirectory(extern/openpgl_stub)

# Ensure our custom Crow wrappers take precedence over the third-party headers
include_directories(BEFORE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/extern/crow/include
)
add_compile_definitions(CROW_USE_BOOST)


# Find required packages
find_package(Threads REQUIRED)
find_package(CURL REQUIRED)
find_library(HTTP_PARSER_LIBRARY http_parser)
find_path(HTTP_PARSER_INCLUDE_DIR http_parser.h)
if(HTTP_PARSER_INCLUDE_DIR AND HTTP_PARSER_LIBRARY)
    add_library(http_parser::http_parser INTERFACE IMPORTED)
    set_target_properties(http_parser::http_parser PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES ${HTTP_PARSER_INCLUDE_DIR}
        INTERFACE_LINK_LIBRARIES ${HTTP_PARSER_LIBRARY}
    )
else()
    message(WARNING "http_parser library not found; using bundled header-only implementation")
    add_library(http_parser INTERFACE)
    add_library(http_parser::http_parser ALIAS http_parser)
    target_include_directories(http_parser INTERFACE ${CMAKE_SOURCE_DIR}/extern/crow/include)
    set(HTTP_PARSER_LIBRARY http_parser)
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
if(SEP_HAS_BLENDER)
    add_subdirectory(src/blender)
endif()

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
        ${HTTP_PARSER_LIBRARY}
)
