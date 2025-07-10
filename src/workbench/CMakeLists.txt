cmake_minimum_required(VERSION 3.16)

# Gather all workbench sources and demos
file(GLOB_RECURSE WB_SOURCES
    "demos/*.cpp"
    "renderer.cpp"
)

add_library(sep_workbench_lib STATIC ${WB_SOURCES})

# Public include directories for consumers
 target_include_directories(sep_workbench_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
)

# Link against core components
 target_link_libraries(sep_workbench_lib PRIVATE
    sep_core sep_quantum sep_memory
)
