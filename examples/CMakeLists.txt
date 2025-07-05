cmake_minimum_required(VERSION 3.16)

# Build JSON processor example
add_executable(json_processor json_processor.cpp)

# Include directories for engine and external dependencies
# Allow sep_engine_wrapper.h to locate SEP headers
target_include_directories(json_processor PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/extern/nlohmann
    ${PROJECT_SOURCE_DIR}/extern/glm
    ${PROJECT_SOURCE_DIR}/extern/cycles/src
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Enable C++17
set_target_properties(json_processor PROPERTIES CXX_STANDARD 17)

# Compile definitions for example build
target_compile_definitions(json_processor PRIVATE SEP_HAS_CYCLES)

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

# Add other example subdirectories
add_subdirectory(workbench)
