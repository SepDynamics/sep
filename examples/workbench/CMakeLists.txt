cmake_minimum_required(VERSION 3.16)

add_executable(sep_workbench
    main.cpp
    config.cpp
    demo_manager.cpp
    demos/genesis_pattern.cpp
)

set_target_properties(sep_workbench PROPERTIES CXX_STANDARD 17)

target_include_directories(sep_workbench PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/examples/workbench
    ${PROJECT_SOURCE_DIR}/extern/cycles/src
)

target_link_libraries(sep_workbench PRIVATE
    sep_api
    sep_blender
    sep_audio
    sep_quantum
    sep_memory
    sep_compat
    sep_core
    Threads::Threads
    ${CMAKE_DL_LIBS}
)
