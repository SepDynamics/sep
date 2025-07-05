add_executable(annealing_sim
    annealing_sim.cpp
)

target_include_directories(annealing_sim PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/extern/glm
)

target_link_libraries(annealing_sim PRIVATE
    sep_core
    sep_quantum
    sep_compat
)

add_subdirectory(workbench)
