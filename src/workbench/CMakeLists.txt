cmake_minimum_required(VERSION 3.16)

file(GLOB_RECURSE WB_SOURCES
    "demos/*.cpp"
    "renderer.cpp"
    "config.cpp"
)

add_library(sep_workbench_lib STATIC ${WB_SOURCES})

target_include_directories(sep_workbench_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/demos
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(sep_workbench_lib PRIVATE
    sep_core
    sep_quantum
    sep_memory
)
