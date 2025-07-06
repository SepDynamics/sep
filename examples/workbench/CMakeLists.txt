# SEP Workbench Example Build Configuration
cmake_minimum_required(VERSION 3.16)

# Add the workbench executable with its source files
add_executable(sep_workbench
    main.cpp
    demo_manager.cpp
    config.cpp
    # Demo sources
    demos/genesis_pattern.cpp
    demos/audio_visualizer.cpp
    demos/memory_garden.cpp
    demos/annealing_demo.cpp
    demos/drug_optimizer.cpp
    demos/drug_discovery_demo.cpp
    demos/digital_physics_demo.cpp
    demos/flocking_demo.cpp
    demos/cosmo_demo.cpp
    demos/cosmo_sim.cpp
    demos/annealing_sim.cpp
    demos/neural_demo.cpp
    demos/neuro_sim.cpp
)

# Set C++ standard
set_target_properties(sep_workbench PROPERTIES CXX_STANDARD 17)

# Add include directories for the main engine and dependencies
target_include_directories(sep_workbench PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Define preprocessor macros
target_compile_definitions(sep_workbench PRIVATE
    GLM_ENABLE_EXPERIMENTAL
    SEP_WORKBENCH_DEMO
)

# Link against the SEP libraries via CMake targets
target_link_libraries(sep_workbench PRIVATE
    sep_api
    sep_audio
    sep_blender
    sep_compat
    sep_core
    sep_memory
    sep_quantum
    sep_embeddings
)

# Add this subdirectory to your root CMakeLists.txt:
# add_subdirectory(examples/workbench)
