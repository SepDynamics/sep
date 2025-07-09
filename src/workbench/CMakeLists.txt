# Set directories for better readability
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/extern/imgui)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/src)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

# Find required packages
find_package(OpenGL REQUIRED)

# Use GLFW from the subdirectory we added
set(GLFW_LIBRARIES glfw)

# ImGui source files
set(IMGUI_SRC
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

# Workbench source files
set(WORKBENCH_SRC
    demo_main.cpp
    demo_manager.cpp
    demo_selection.cpp
    window.cpp
    ui_manager.cpp
    renderer.cpp
    sep_engine_wrapper.cpp
    workbench_demo_adapter.cpp
    config.cpp
)

# Demo source files
file(GLOB DEMO_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/demos/*.cpp"
)

# Add demo files to workbench source
list(APPEND WORKBENCH_SRC ${DEMO_SRC})

# Create the executable (only once!)
add_executable(sep_workbench ${WORKBENCH_SRC})

# Set include directories
target_include_directories(sep_workbench PRIVATE
    ${INCLUDE_DIR}
    ${SRC_DIR}
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
    ${OPENGL_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Set compiler features
set_target_properties(sep_workbench PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# Link libraries
target_link_libraries(sep_workbench PRIVATE
    # SEP Engine Libraries
    sep_quantum
    sep_memory
    sep_compat
    sep_core
    
    # External Dependencies
    ${GLFW_LIBRARIES}
    ${OPENGL_LIBRARIES}
    dl
    pthread
)

# Add compile definitions
target_compile_definitions(sep_workbench PRIVATE
    GL_SILENCE_DEPRECATION
    SEP_WORKBENCH=1
    GLM_ENABLE_EXPERIMENTAL
)

# Make sure we can find the engine header paths
target_include_directories(sep_workbench PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}/src
)