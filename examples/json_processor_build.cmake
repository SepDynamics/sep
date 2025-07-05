# Build configuration for the JSON processor example

add_executable(json_processor
    ${CMAKE_SOURCE_DIR}/examples/json_processor.cpp
)

set_target_properties(json_processor PROPERTIES
    CXX_STANDARD 17
)

target_include_directories(json_processor PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/extern/nlohmann
    ${CMAKE_SOURCE_DIR}/extern/glm
    ${CMAKE_SOURCE_DIR}/extern/cycles/src
    ${CMAKE_SOURCE_DIR}/examples
)

target_compile_definitions(json_processor PRIVATE
    SEP_HAS_CYCLES
)

# Link against the SEP engine libraries
# Keep the same order as the shell build script to satisfy dependencies

# Start group equivalent isn't needed in CMake, but maintain order

target_link_libraries(json_processor PRIVATE
    sep_api
    sep_audio
    sep_quantum
    sep_memory
    sep_compat
    sep_core
    pthread
)

# Specify output directory similar to build_json_processor.sh
set_target_properties(json_processor PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/examples/build
)

