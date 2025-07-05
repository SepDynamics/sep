# SEP Workbench Example Build Configuration
cmake_minimum_required(VERSION 3.16)

# Add the workbench executable with its source files
add_executable(sep_workbench
    main.cpp
    demo_manager.cpp
    config.cpp
    demos/genesis_pattern.cpp
    demos/audio_visualizer.cpp
    demos/memory_garden.cpp
    demos/annealing_demo.cpp
    demos/drug_optimizer.cpp
)

# Set C++ standard
set_target_properties(sep_workbench PROPERTIES CXX_STANDARD 17)

# Add include directories for the main engine and dependencies
target_include_directories(sep_workbench PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/extern/cycles/src
    ${PROJECT_SOURCE_DIR}/extern/glm
)

# Define preprocessor macros
target_compile_definitions(sep_workbench PRIVATE
    GLM_ENABLE_EXPERIMENTAL
    SEP_WORKBENCH_DEMO
)

# Add library search paths
link_directories(
    /sep/src
    /sep/src/core
    /sep/src/quantum
    /sep/src/memory
    /sep/src/audio
    /sep/src/blender
    /sep/src/compat
    /sep/build/lib
    /sep/cmake-make/src/core
    /sep/cmake-make/src/quantum
    /sep/cmake-make/src/memory
    /sep/cmake-make/src/audio
    /sep/cmake-make/src/blender
    /sep/cmake-make/src/compat
)

# Link against the SEP libraries via CMake targets
target_link_libraries(sep_workbench PRIVATE
    sep_core
    sep_quantum
    sep_memory
    sep_audio
    sep_blender
    sep_compat
)

# Add this subdirectory to your root CMakeLists.txt:
# add_subdirectory(examples/workbench)
