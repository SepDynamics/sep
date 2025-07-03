cmake_minimum_required(VERSION 3.15)
project(sep_engine VERSION 1.0.0 LANGUAGES C CXX)

# Enable CUDA
set(CMAKE_CUDA_STANDARD 17)
set(CMAKE_CUDA_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Add cmake directory to module path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
# Use Cycles' custom CMake modules for dependencies like OSL and Alembic.
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/extern/cycles/src/cmake/Modules")

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
find_package(TBB REQUIRED)
find_package(OpenImageIO REQUIRED)
find_package(embree REQUIRED)
find_package(OSL REQUIRED)
find_package(Alembic REQUIRED)

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

# Build Cycles from source when precompiled libraries are not available
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/lib/libcycles_device.a")
  message(STATUS "Prebuilt Cycles libraries not found; building from source")
  add_subdirectory(extern/cycles)
endif()

# Main executable
add_executable(sep_engine src/main.cpp)

target_include_directories(sep_engine
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(sep_engine
    PRIVATE
        # 1. High-level SEP Engine static libraries
        sep_api
        sep_blender
        sep_audio
        sep_embeddings
        sep_quantum
        sep_memory
        sep_compat # GPU abstraction, depends on core
        sep_core   # Foundational library, linked last among SEP libs

        # 2. Cycles and its dependencies (order is important here too)
        # Link the main Cycles libs first
        libcycles_session
        libcycles_scene
        libcycles_device
        libcycles_kernel_osl # The one with the real symbols
        libcycles_subd
        cycles_util
        extern_cuew
        extern_hipew

        # Now link the external dependencies that Cycles needs
        OpenImageIO
        OpenVDB
        OpenSubdiv
        ${OSL_LIBRARIES} # Link the real OSL libs, not the stub
        ${TBB_LIBRARIES}

        # 3. Other direct dependencies
        ${CURL_LIBRARIES}
        http_parser::http_parser
        fmt::fmt
        spdlog::spdlog
        hiredis
        pipewire-0.3

        # 4. System libraries
        ${EMBREE_LIBRARIES}
        Imath
        ${OpenPGL_LIBRARIES}
        ${OpenColorIO_LIBRARIES}
        ${ALEMBIC_LIBRARIES}
        ${CMAKE_DL_LIBS}
        ${Threads_LIBRARIES}
        ${CUDAToolkit_LIBRARIES}
        pthread
        dl
        rt
        m
        z
)
