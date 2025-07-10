cmake_minimum_required(VERSION 3.15)

# --- Graphics and Windowing ---
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)

# --- ImGui (Third Party) ---
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/third_party/imgui)
file(GLOB IMGUI_SOURCES
    ${IMGUI_DIR}/*.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

# --- Workbench Executable ---
add_executable(sep_workbench
    main.cpp
    ${IMGUI_SOURCES}
)

# --- Include Directories ---
target_include_directories(sep_workbench PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)

# --- Link Libraries ---
target_link_libraries(sep_workbench PRIVATE
    # SEP Core for logging
    sep_core

    # Graphics
    GLEW::GLEW
    glfw
    OpenGL::GL
)