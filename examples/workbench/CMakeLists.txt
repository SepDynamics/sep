cmake_minimum_required(VERSION 3.16)

file(GLOB DEMO_SOURCES demos/*.cpp)

add_executable(sep_workbench
    main.cpp
    ${DEMO_SOURCES}
)

set_target_properties(sep_workbench PROPERTIES CXX_STANDARD 17)

# Include directories
target_include_directories(sep_workbench PRIVATE
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/examples/workbench
)

# Enable GLM experimental features
target_compile_definitions(sep_workbench PRIVATE GLM_ENABLE_EXPERIMENTAL)

# Link libraries
target_link_libraries(sep_workbench PRIVATE
    sep_api
    sep_quantum
    sep_memory
    sep_core
    sep_compat
    Threads::Threads
    ${CMAKE_DL_LIBS}
)

if(SEP_ENABLE_AUDIO)
    target_link_libraries(sep_workbench PRIVATE sep_audio)
endif()

if(SEP_ENABLE_BLENDER)
    target_link_libraries(sep_workbench PRIVATE sep_blender)
endif()
