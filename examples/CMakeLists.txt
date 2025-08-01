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
    ${CMAKE_THREAD_LIBS_INIT}
    nlohmann_json::nlohmann_json
    CUDA::cudart
)

# Set output directory
set_target_properties(pattern_metric_example
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/examples
)

# PME Testbed
add_executable(pme_testbed
    pme_testbed.cpp
)

target_link_libraries(pme_testbed
    sep_engine
    sep_quantum
    sep_memory
    sep_connectors
    fmt::fmt
    ${CMAKE_THREAD_LIBS_INIT}
    nlohmann_json::nlohmann_json
)

set_target_properties(pme_testbed
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/examples
)

# PME Testbed Enhanced
add_executable(pme_testbed_enhanced
    pme_testbed_enhanced.cpp
)

target_link_libraries(pme_testbed_enhanced
    sep_engine
    sep_quantum
    sep_memory
    sep_connectors
    fmt::fmt
    ${CMAKE_THREAD_LIBS_INIT}
    nlohmann_json::nlohmann_json
)

set_target_properties(pme_testbed_enhanced
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/examples
)

# PME Testbed Phase 2
add_executable(pme_testbed_phase2
    pme_testbed_phase2.cpp
)

target_link_libraries(pme_testbed_phase2
    sep_engine
    sep_quantum
    sep_memory
    sep_connectors
    fmt::fmt
    ${CMAKE_THREAD_LIBS_INIT}
    nlohmann_json::nlohmann_json
)

set_target_properties(pme_testbed_phase2
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/examples
)


