# CMakeLists.txt for Cycles integration tests

add_executable(cycles_test cycles_test.cpp)

# Link against the same libraries as the main executable to ensure consistency.
# The SEP_LINK_LIBS variable is defined in the root CMakeLists.txt.
target_link_libraries(cycles_test
    PRIVATE
    # Your internal project libraries
    sep_api sep_memory sep_quantum sep_core sep_audio sep_blender sep_compat
    # All external dependencies (same as sep_engine)
    ${SEP_LINK_LIBS}
    # Required package libraries
    ${CURL_LIBRARIES} ${HIREDIS_LIBRARIES} ${CUDA_LIBRARIES} Threads::Threads fmt::fmt spdlog::spdlog
    # Standard system libs
    -ldl -lm
)

add_custom_target(run_cycles_test
    COMMAND cycles_test ${CMAKE_BINARY_DIR}/cycles_render.ppm
    DEPENDS cycles_test
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running Cycles renderer test"
)