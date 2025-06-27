# CMakeLists.txt for Cycles integration tests

add_executable(cycles_test cycles_test.cpp)

# Link against the same libraries as the main executable to ensure consistency.
# The SEP_LINK_LIBS variable is defined in the root CMakeLists.txt.
target_link_libraries(cycles_test PRIVATE ${SEP_ENGINE_LIBS})

add_custom_target(run_cycles_test
    COMMAND cycles_test ${CMAKE_BINARY_DIR}/cycles_render.ppm
    DEPENDS cycles_test
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running Cycles renderer test"
)
