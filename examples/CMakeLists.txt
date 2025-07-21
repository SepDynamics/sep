add_executable(pattern_metric_example pattern_metric_example.cpp)

target_link_libraries(pattern_metric_example PRIVATE
    sep_quantum
    sep_engine
    fmt::fmt
    benchmark::benchmark
    TBB::tbb
    nlohmann_json::nlohmann_json
)

add_executable(financial_demo financial_demo.cpp)

target_link_libraries(financial_demo PRIVATE
    sep_quantum
    sep_engine
    fmt::fmt
    TBB::tbb
)

# Simple command-line data processing sandbox
add_executable(simple_data_sandbox simple_data_sandbox.cpp)

target_link_libraries(simple_data_sandbox PRIVATE
    sep_quantum
    sep_engine
    fmt::fmt
    TBB::tbb
)

# Data processing sandbox with ImGui interface (disabled due to build issues)
# add_executable(data_processing_sandbox data_processing_sandbox.cpp)

# Standalone SEP executable with all functionality
add_executable(sep_standalone sep_standalone.cpp)

# Find static libraries
find_library(FMT_STATIC_LIB NAMES libfmt.a fmt)
find_library(SPDLOG_STATIC_LIB NAMES libspdlog.a spdlog)

# Link order matters for static libraries
target_link_libraries(sep_standalone
    PRIVATE
        sep_quantum
        sep_engine
        sep_memory
        ${SPDLOG_STATIC_LIB}
        ${FMT_STATIC_LIB}
        benchmark::benchmark
        TBB::tbb
        nlohmann_json::nlohmann_json
        pthread
        dl
        rt
)

# Force static linking
set_target_properties(sep_standalone PROPERTIES
    LINK_FLAGS "-static-libgcc -static-libstdc++"
)
# Force static linking for problematic libraries
set_target_properties(sep_standalone PROPERTIES
    LINK_FLAGS "-static-libgcc -static-libstdc++"
)

# Add include directories
target_include_directories(sep_standalone PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

# Set as the main output executable
set_target_properties(sep_standalone PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    OUTPUT_NAME "sep"
)

# Temporarily disabled due to DWARF linker issues
# add_subdirectory(sep_memory_verifier)