# CMakeLists.txt for Cycles integration tests

add_executable(cycles_test cycles_test.cpp)
target_include_directories(sep_quantum
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/extern
)


# Link against the same libraries as the main executable to ensure consistency.
# The SEP_LINK_LIBS variable is defined in the root CMakeLists.txt.
target_link_libraries(cycles_test
    PRIVATE
    # Your internal project libraries
    ${SEP_INTERNAL_LIBS}
    # Internal project libraries
    ${SEP_INTERNAL_LIBS}
    # All external dependencies (same as sep_engine)
    ${SEP_LINK_LIBS}
    ${CURL_LIBRARIES}
    ${HIREDIS_LIBRARIES}
    ${CUDA_LIBRARIES}
    Threads::Threads
    fmt::fmt
    spdlog::spdlog
    dl
    m
)

target_link_libraries(cycles_test PRIVATE ${CYCLES_TEST_LIBS})

add_custom_target(run_cycles_test
    COMMAND cycles_test ${CMAKE_BINARY_DIR}/cycles_render.ppm
    DEPENDS cycles_test
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running Cycles renderer test"
)

