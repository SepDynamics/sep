cmake_minimum_required(VERSION 3.15)
project(sep_engine VERSION 1.0.0 LANGUAGES C CXX CUDA)

# Enable CUDA
set(CMAKE_CUDA_STANDARD 17)
set(CMAKE_CUDA_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Add cmake directory to module path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Add library search paths
link_directories(${CMAKE_SOURCE_DIR}/lib)

# Include unified configuration module
include(SEPConfig)
configure_sep()

# CUDA is a hard requirement for the project
find_package(CUDAToolkit REQUIRED)
set(SEP_HAS_CUDA 1)
add_compile_definitions(SEP_HAS_CUDA=1)

# Blender integration is always built
set(SEP_HAS_BLENDER 1)

# Ensure our custom Crow wrappers take precedence over the third-party headers
include_directories(BEFORE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/extern/crow/include
)
add_compile_definitions(CROW_USE_BOOST)


# Find required packages
find_package(Threads REQUIRED)
find_package(CURL REQUIRED)
find_package(fmt REQUIRED)
find_package(http_parser REQUIRED)
find_package(spdlog REQUIRED)
find_package(OpenVDB REQUIRED)
find_package(OpenPGL REQUIRED)
find_package(OpenColorIO REQUIRED)
find_package(Imath REQUIRED)
find_package(OSL REQUIRED)

# Add subdirectories
add_subdirectory(src/core)
add_subdirectory(src/compat)
add_subdirectory(src/memory)
add_subdirectory(src/quantum)
add_subdirectory(src/embeddings)
add_subdirectory(src/api)
add_subdirectory(src/audio)
add_subdirectory(src/blender)
add_subdirectory(extern/cycles/third_party/cuew)
add_subdirectory(extern/cycles/third_party/hipew)

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
        sep_embeddings
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
        extern_cuew
        extern_hipew
        ${OpenVDB_LIBRARIES}
        ${OpenPGL_LIBRARIES}
        ${OpenColorIO_LIBRARIES}
        openvdb
        OpenColorIO
        Imath
)
