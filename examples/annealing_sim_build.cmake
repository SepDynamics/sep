add_executable(annealing_sim
    examples/annealing_sim.cpp
)

set_target_properties(annealing_sim PROPERTIES CXX_STANDARD 17)

target_include_directories(annealing_sim PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/extern/cycles/src
    ${CMAKE_SOURCE_DIR}/extern/glm
)

link_directories(
    /sep/src
    /sep/src/core
    /sep/src/quantum
    /sep/src/memory
    /sep/src/audio
    /sep/src/blender
    /sep/src/compat
)

target_link_libraries(annealing_sim PRIVATE
    sep_core
    sep_quantum
    sep_memory
    sep_audio
    sep_blender
    sep_compat
    sep_api
)


