cmake_minimum_required(VERSION 3.16)

# Gather all workbench sources and demos
file(GLOB_RECURSE WB_SOURCES
    "demos/*.cpp"
    "core/*.cpp"
    "renderer.cpp"
    "config.cpp"
    "${CMAKE_SOURCE_DIR}/third_party/imgui/*.cpp"
    "${CMAKE_SOURCE_DIR}/third_party/imgui/backends/imgui_impl_glfw.cpp"
    "${CMAKE_SOURCE_DIR}/third_party/imgui/backends/imgui_impl_opengl3.cpp"
)

# ImGui sources are compiled directly into the workbench library
set(IMGUI_SOURCES
    ${CMAKE_SOURCE_DIR}/third_party/imgui/imgui.cpp
    ${CMAKE_SOURCE_DIR}/third_party/imgui/imgui_draw.cpp
    ${CMAKE_SOURCE_DIR}/third_party/imgui/imgui_tables.cpp
    ${CMAKE_SOURCE_DIR}/third_party/imgui/imgui_widgets.cpp
    ${CMAKE_SOURCE_DIR}/third_party/imgui/backends/imgui_impl_glfw.cpp
    ${CMAKE_SOURCE_DIR}/third_party/imgui/backends/imgui_impl_opengl3.cpp
)
add_library(sep_workbench_lib STATIC ${WB_SOURCES} ${IMGUI_SOURCES})

# Public include directories for consumers
target_include_directories(sep_workbench_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/third_party/imgui
)

# Add the IMGUI_IMPL_OPENGL_LOADER_GLAD define to ensure consistency with our OpenGL loader choice
target_compile_definitions(sep_workbench_lib PRIVATE
    IMGUI_IMPL_OPENGL_LOADER_GLAD
)

# Link against core components
target_link_libraries(sep_workbench_lib PRIVATE
    sep_core sep_quantum sep_memory
    glad
    glfw
    OpenGL::GL
)
