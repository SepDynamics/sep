# CMakeLists.txt for Cycles integration tests

add_executable(cycles_test cycles_test.cpp)

# Consolidate all required libraries for the test executable.  This mirrors the
# main engine's link list while keeping the file readable.
set(CYCLES_TEST_LIBS
    sep_api
    sep_memory
    sep_quantum
    sep_core
    sep_audio
    sep_blender
    sep_compat
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
