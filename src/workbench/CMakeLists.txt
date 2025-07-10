cmake_minimum_required(VERSION 3.16)

# Gather all workbench sources and demos
file(GLOB_RECURSE WB_SOURCES
    "demos/*.cpp"
    "renderer.cpp"
    "${CMAKE_SOURCE_DIR}/third_party/imgui/*.cpp"
    "${CMAKE_SOURCE_DIR}/third_party/imgui/backends/imgui_impl_glfw.cpp"
    "${CMAKE_SOURCE_DIR}/third_party/imgui/backends/imgui_impl_opengl3.cpp"
)

add_library(sep_workbench_lib STATIC ${WB_SOURCES})

# Public include directories for consumers
target_include_directories(sep_workbench_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/third_party/imgui
    ${CMAKE_SOURCE_DIR}/third_party/imgui/backends
)

# Link against core components
 target_link_libraries(sep_workbench_lib PRIVATE
    sep_core sep_quantum sep_memory
)
