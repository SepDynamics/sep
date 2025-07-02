cmake_minimum_required(VERSION 3.15)

# Workbench executable
add_executable(sep_workbench
    main.cpp
    demo_manager.cpp
    demos/genesis_pattern.cpp
)

# Include directories
target_include_directories(sep_workbench PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/extern/imgui
)

# Link libraries
target_link_libraries(sep_workbench PRIVATE
    sep_core
    sep_quantum
    sep_memory
    sep_blender
    imgui
)

# Set C++ standard
set_target_properties(sep_workbench PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# Install target
install(TARGETS sep_workbench
    RUNTIME DESTINATION bin
)