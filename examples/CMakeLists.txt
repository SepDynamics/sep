# Examples CMakeLists.txt

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/engine
    ${CMAKE_SOURCE_DIR}/src/quantum
    ${CMAKE_SOURCE_DIR}/src/memory
    ${CMAKE_SOURCE_DIR}/src/connectors
)

# Pattern Metric Example
add_executable(pattern_metric_example
    pattern_metric_example.cpp
)

target_link_libraries(pattern_metric_example
    sep_engine
    sep_quantum  
    sep_memory
    sep_connectors
    fmt::fmt
    ${FMT_DYNAMIC_LIB}
    ${CMAKE_THREAD_LIBS_INIT}
    nlohmann_json::nlohmann_json
)

# Set output directory
set_target_properties(pattern_metric_example 
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/examples
)
