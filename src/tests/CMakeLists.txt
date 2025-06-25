# CMakeLists.txt for Cycles integration tests

# Build the Cycles test application regardless of Cycles availability
add_executable(cycles_test cycles_test.cpp)

target_include_directories(cycles_test
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(cycles_test
    PRIVATE
        sep_blender
        sep_core
        sep_quantum
        sep_memory
)

# Add a custom target to run the test
add_custom_target(run_cycles_test
    COMMAND cycles_test ${CMAKE_BINARY_DIR}/cycles_render.ppm
    DEPENDS cycles_test
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running Cycles renderer test"
)

message(STATUS "Cycles test application enabled (will use stub implementation if Cycles not available)")