cmake_minimum_required(VERSION 3.16)

add_executable(sep_workbench
    demo_manager.cpp
    demos/audio_visualizer.cpp
    demos/memory_garden.cpp
    demos/annealing_demo.cpp
    demos/drug_discovery_demo.cpp
    demos/neural_demo.cpp
    demos/cosmo_demo.cpp
    demos/digital_physics_demo.cpp
    demos/drug_optimizer.cpp
    demos/annealing_sim.cpp
    demos/cosmo_sim.cpp
    demos/neuro_sim.cpp
    demos/flocking_demo.cpp
    demos/genesis_pattern.cpp
    legacy_main.cpp
    main.cpp
    config.cpp
)

set_target_properties(sep_workbench PROPERTIES CXX_STANDARD 17)

# Build the workbench in "demo" mode so it links against the
# lightweight wrapper implementations defined in
# `sep_engine_wrapper.h`.  This avoids pulling in the full engine
# dependencies and provides stubbed renderer/engine behaviour that
# matches the workbench examples.
if(SEP_WORKBENCH_DEMO)
    target_compile_definitions(sep_workbench PRIVATE SEP_WORKBENCH_DEMO=1)
else()
    target_compile_definitions(sep_workbench PRIVATE SEP_WORKBENCH_DEMO=0)
endif()

# Set C++ standard and compiler flags
set_target_properties(sep_workbench PROPERTIES CXX_STANDARD 17)
target_compile_features(sep_workbench PRIVATE cxx_std_17)

# Add compiler-specific flags
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(sep_workbench PRIVATE
        -isystem /usr/include/c++/v1
        -isystem /usr/local/include
        -isystem /usr/include
    )
elseif(MSVC)
    target_compile_options(sep_workbench PRIVATE /EHsc)
endif()

# Add system include paths
target_include_directories(sep_workbench SYSTEM PRIVATE
    ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/extern/glm
    /usr/include
    /usr/local/include
    /usr/local/include
)

# Common includes for both modes
target_include_directories(sep_workbench PRIVATE
    ${PROJECT_SOURCE_DIR}/examples/workbench
    ${PROJECT_SOURCE_DIR}/extern/glm
)

# Additional includes for non-demo mode
if(NOT SEP_WORKBENCH_DEMO)
    target_include_directories(sep_workbench PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/extern/cycles/src>
    )
endif()


target_link_libraries(sep_workbench PRIVATE
    sep_api
    sep_quantum
    sep_memory
    sep_compat
    sep_core
    Threads::Threads
    ${CMAKE_DL_LIBS}
)

# Only link against blender if enabled
if(SEP_ENABLE_BLENDER)
    target_link_libraries(sep_workbench PRIVATE sep_blender)
endif()

# Only link against audio if enabled
if(SEP_ENABLE_AUDIO)
    target_link_libraries(sep_workbench PRIVATE sep_audio)
endif()
