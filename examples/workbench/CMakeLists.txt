cmake_minimum_required(VERSION 3.15)
project(SEPWorkbench CXX)

# Find SEP package
find_package(sep REQUIRED)

# Workbench executable
add_executable(sep_workbench
    main.cpp
    demo_manager.cpp
    demos/genesis_pattern.cpp
    demos/audio_visualizer.cpp
    demos/memory_garden.cpp
)

# Include directories
target_include_directories(sep_workbench PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_INSTALL_PREFIX}/include
    ${CMAKE_SOURCE_DIR}/extern/imgui
    ${CMAKE_SOURCE_DIR}/extern/glm
)

# Link libraries
target_link_libraries(sep_workbench PRIVATE
    sep::core
    sep::quantum
    sep::memory
    sep::blender
    sep::audio
    sep::compat
    imgui
    glm
)

# Set C++ standard
set_target_properties(sep_workbench PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

# Install target
install(TARGETS sep_workbench
    RUNTIME DESTINATION bin
)