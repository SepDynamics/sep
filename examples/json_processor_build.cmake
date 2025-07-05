# Build the JSON processor example
# This mirrors the behavior of build_json_processor.sh but integrates with CMake

# Ensure Threads package is available
find_package(Threads REQUIRED)

add_executable(json_processor
    ${CMAKE_SOURCE_DIR}/examples/json_processor.cpp
)

# Include directories used by the example
# We reference the same directories as in the shell script
# plus the project include path

target_include_directories(json_processor PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/extern/nlohmann
    ${CMAKE_SOURCE_DIR}/extern/glm
    ${CMAKE_SOURCE_DIR}/extern/cycles/src
    ${CMAKE_SOURCE_DIR}/examples
)

# Define preprocessor macros

target_compile_definitions(json_processor PRIVATE
    SEP_HAS_CYCLES
)

# Link against SEP static libraries and pthread

target_link_libraries(json_processor PRIVATE
    sep_api
    sep_audio
    sep_quantum
    sep_memory
    sep_compat
    sep_core
    Threads::Threads
)
