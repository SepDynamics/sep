cmake_minimum_required(VERSION 3.15)

# Locate required graphics packages
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(sep_workbench
    main.cpp
)

# Gather demo sources
file(GLOB DEMO_SOURCES demos/*.cpp)
target_sources(sep_workbench PRIVATE ${DEMO_SOURCES})

# Include Dear ImGui
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/third_party/imgui)
file(GLOB IMGUI_SOURCES
    ${IMGUI_DIR}/*.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

target_sources(sep_workbench PRIVATE ${IMGUI_SOURCES})

# Include directories
target_include_directories(sep_workbench PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)

# Enable GLM experimental features
target_compile_definitions(sep_workbench PRIVATE GLM_ENABLE_EXPERIMENTAL)

# Link libraries
target_link_libraries(sep_workbench PRIVATE
    # Graphics and UI
    ${GLEW_LIBRARIES}
    glfw
    OpenGL::GL

    # Engine libraries
    sep_api
    sep_core
    sep_compat
    sep_quantum
    sep_memory
    sep_blender
    sep_audio
)
