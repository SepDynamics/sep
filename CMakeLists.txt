cmake_minimum_required(VERSION 3.10)
project(SEPEngine)

# Use C++20 across all targets to satisfy third-party requirements
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${CMAKE_SOURCE_DIR}/include
                    ${CMAKE_SOURCE_DIR}/third_party
                    ${CMAKE_SOURCE_DIR}/third_party/crow/include)
add_subdirectory(src/api)
add_subdirectory(src/audio)
add_subdirectory(src/blender)
add_subdirectory(src/compat)
add_subdirectory(src/context)
add_subdirectory(src/core)
add_subdirectory(src/memory)
add_subdirectory(src/quantum)
add_executable(sep_engine src/main.cpp)
target_link_libraries(sep_engine
                      sep_core
                      sep_api
                      sep_context
                      sep_memory
                      sep_quantum
                      sep_audio
                      sep_blender
                      sep_compat)
include_directories(${CMAKE_SOURCE_DIR}/third_party/nlohmann)
