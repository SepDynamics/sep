cmake_minimum_required(VERSION 3.16)

# Gather all workbench sources and demos
file(GLOB_RECURSE WB_SOURCES
    "demos/*.cpp"
    "core/*.cpp"
    "simple_renderer.cpp"
    "config.cpp"
    "stubs/*.cpp"
)

add_library(sep_workbench_lib STATIC ${WB_SOURCES})

# Public include directories for consumers
target_include_directories(sep_workbench_lib PUBLIC
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/workbench
    ${CMAKE_SOURCE_DIR}/src/workbench/demos
    ${CMAKE_SOURCE_DIR}/src/workbench/core
    ${CMAKE_SOURCE_DIR}/third_party
    ${CMAKE_SOURCE_DIR}/third_party/imgui
    ${CMAKE_SOURCE_DIR}/src/quantum
    ${CMAKE_SOURCE_DIR}/src/engine
    ${CMAKE_SOURCE_DIR}/src/memory
    ${CMAKE_SOURCE_DIR}/third_party/imgui/backends
    ${CMAKE_SOURCE_DIR}/src/glad
)

# Link against core components
target_link_libraries(sep_workbench_lib PUBLIC
    sep_engine
    sep_quantum
    sep_memory
    imgui
    glad
    OpenGL::GL
    glfw
)
