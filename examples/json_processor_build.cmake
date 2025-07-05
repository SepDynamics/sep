# Build configuration for the JSON processor example

add_executable(json_processor
    ${CMAKE_CURRENT_LIST_DIR}/json_processor.cpp
)

set_target_properties(json_processor PROPERTIES CXX_STANDARD 17)

# Include directories
# Mirror the include paths used in build_json_processor.sh

# Add engine and dependency headers
target_include_directories(json_processor PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/extern/nlohmann
    ${CMAKE_SOURCE_DIR}/extern/glm
    ${CMAKE_SOURCE_DIR}/extern/cycles/src
    ${CMAKE_CURRENT_LIST_DIR}
)

# Preprocessor definitions
# Indicate that the engine was built with Cycles support
# to match the standalone build script
add_compile_definitions(SEP_HAS_CYCLES)

# Link against the engine libraries
# The order is handled automatically by CMake
# so no need for --start-group/--end-group

target_link_libraries(json_processor PRIVATE
    sep_api
    sep_audio
    sep_quantum
    sep_memory
    sep_compat
    sep_core
    pthread
)

# Output path within the examples build directory
set_target_properties(json_processor PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/examples
)
