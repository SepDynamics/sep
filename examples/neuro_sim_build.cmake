add_executable(neuro_sim
    examples/neuro_sim.cpp
)

set_target_properties(neuro_sim PROPERTIES CXX_STANDARD 17)

target_include_directories(neuro_sim PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/extern/glm
    ${CMAKE_SOURCE_DIR}/extern/cycles/src
)

target_link_libraries(neuro_sim PRIVATE
    sep_core
    sep_quantum
    sep_memory
)
