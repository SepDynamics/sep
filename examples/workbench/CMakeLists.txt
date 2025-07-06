cmake_minimum_required(VERSION 3.16)

add_executable(sep_workbench
    main.cpp
    config.cpp
    demo_manager.cpp
    demos/genesis_pattern.cpp
    demos/audio_visualizer.cpp
    demos/memory_garden.cpp
    demos/cosmo_demo.cpp
    demos/cosmo_sim.cpp
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

target_include_directories(sep_workbench PRIVATE
    ${PROJECT_SOURCE_DIR}/examples/workbench
    ${PROJECT_SOURCE_DIR}/include
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/extern/cycles/src>
)


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
