cmake_minimum_required(VERSION 3.10)
project(SEPEngine)

include_directories(
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/third_party
  ${CMAKE_SOURCE_DIR}/third_party/crow/include
  ${CMAKE_SOURCE_DIR}/third_party/glm
  ${CMAKE_SOURCE_DIR}/third_party/tbb
)
add_subdirectory(src/api)
add_subdirectory(src/audio)
add_subdirectory(src/blender)
add_subdirectory(src/compat)
add_subdirectory(src/core)
add_subdirectory(src/memory)
add_subdirectory(src/quantum)
add_executable(sep_engine src/main.cpp)
target_include_directories(sep_engine
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(sep_engine
    sep_core
    sep_api
    sep_memory
    sep_quantum
    sep_audio
    sep_blender
    sep_compat)
