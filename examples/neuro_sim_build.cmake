add_executable(neuro_sim
    examples/neuro_sim.cpp
)

set_target_properties(neuro_sim PROPERTIES CXX_STANDARD 17)

target_include_directories(neuro_sim PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/extern/cycles/src
    ${PROJECT_SOURCE_DIR}/extern/glm
    ${PROJECT_SOURCE_DIR}/examples
)

target_link_libraries(neuro_sim PRIVATE
    sep_core
    sep_quantum
    sep_memory
    sep_audio
    sep_blender
    sep_compat
)
