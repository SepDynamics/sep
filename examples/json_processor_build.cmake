add_executable(json_processor
    examples/json_processor.cpp
)

set_target_properties(json_processor PROPERTIES CXX_STANDARD 17)

target_include_directories(json_processor PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/extern/nlohmann
    ${PROJECT_SOURCE_DIR}/extern/glm
    ${PROJECT_SOURCE_DIR}/extern/cycles/src
    ${PROJECT_SOURCE_DIR}/examples
)

target_link_libraries(json_processor PRIVATE
    sep_api
    sep_audio
    sep_quantum
    sep_memory
    sep_compat
    sep_core
)
