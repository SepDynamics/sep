add_executable(cosmo_sim examples/cosmo_sim.cpp)

set_target_properties(cosmo_sim PROPERTIES CXX_STANDARD 17)

target_include_directories(cosmo_sim PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/extern/nlohmann
    ${CMAKE_SOURCE_DIR}/extern/glm
    ${CMAKE_SOURCE_DIR}/extern/cycles/src
    ${CMAKE_SOURCE_DIR}/examples
)

target_link_libraries(cosmo_sim PRIVATE
    sep_api
    sep_audio
    sep_quantum
    sep_memory
    sep_compat
    sep_core
)
