# Build configuration for the JSON processor example

add_executable(json_processor ${CMAKE_CURRENT_LIST_DIR}/json_processor.cpp)

# Include directories mirroring the build_json_processor.sh script
target_include_directories(json_processor PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/extern/nlohmann
    ${CMAKE_SOURCE_DIR}/extern/glm
    ${CMAKE_SOURCE_DIR}/extern/cycles/src
    ${CMAKE_CURRENT_LIST_DIR}
)

# Define required macros
target_compile_definitions(json_processor PRIVATE
    SEP_HAS_CYCLES=1
)

# Link against SEP engine static libraries
target_link_libraries(json_processor PRIVATE
    sep_api
    sep_audio
    sep_quantum
    sep_memory
    sep_compat
    sep_core
    Threads::Threads
)

# Place the built executable in the examples/build directory
set_target_properties(json_processor PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/build
)
