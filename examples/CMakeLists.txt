cmake_minimum_required(VERSION 3.16)

# Build the JSON processor example
add_executable(json_processor json_processor.cpp)

# Include directories so sep_engine_wrapper.h can locate engine headers
# and third-party libraries used by the examples
# PROJECT_SOURCE_DIR refers to the repository root where the include directory lives

# Provide include paths for engine headers and third party deps
# used by json_processor and the demos
set(JSON_PROCESSOR_INCLUDES
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/extern/nlohmann
    ${PROJECT_SOURCE_DIR}/extern/glm
    ${PROJECT_SOURCE_DIR}/extern/cycles/src
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_include_directories(json_processor PRIVATE ${JSON_PROCESSOR_INCLUDES})

target_compile_definitions(json_processor PRIVATE SEP_WORKBENCH_DEMO)

# Link against the SEP Engine static libraries
# The examples rely only on the core components

set(JSON_PROCESSOR_LIBS
    sep_api
    sep_audio
    sep_quantum
    sep_memory
    sep_compat
    sep_core
)

target_link_libraries(json_processor PRIVATE ${JSON_PROCESSOR_LIBS})

# Build the workbench demo application
add_subdirectory(workbench)

