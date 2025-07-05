cmake_minimum_required(VERSION 3.16)

# JSON processor example
add_executable(json_processor json_processor.cpp)

# Include directories so sep_engine_wrapper.h finds headers
# The examples directory is added for the wrapper itself
# Project include directory exposes the real engine headers
# External dependencies are expected to be resolved globally
# (e.g. nlohmann_json, glm)
target_include_directories(json_processor
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link against core engine libraries
# Only the minimal set required for the processor is linked
# Additional libraries can be added here if needed

target_link_libraries(json_processor
    PRIVATE
        sep_core
        sep_quantum
        sep_compat
)

# Build the workbench demo
# Simple neuron network simulation example
add_executable(neuro_sim neuro_sim.cpp)

target_include_directories(neuro_sim
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(neuro_sim
    PRIVATE
        sep_core
        sep_quantum
        sep_compat
)
 
add_subdirectory(workbench)
