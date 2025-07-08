# Common test configuration
# Configure test output directory
include(CTest)
enable_testing()

set(TEST_OUTPUT_DIR ${CMAKE_BINARY_DIR}/tests)
file(MAKE_DIRECTORY ${TEST_OUTPUT_DIR})

# Set test data directory
set(SEP_TEST_DATA_DIR ${CMAKE_CURRENT_SOURCE_DIR}/test_data)
file(MAKE_DIRECTORY ${SEP_TEST_DATA_DIR})

# Enable test framework before adding subdirectories so that
# add_test() calls are registered correctly.
include(CTest)
enable_testing()

# Common test compile definitions
add_compile_definitions(
    SEP_TEST_DATA_DIR="${SEP_TEST_DATA_DIR}"
    SEP_TEST_BUILD=1
)

# Common test include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Configure test discovery
include(CTest)
enable_testing()

# Add test subdirectories
if(SEP_HAS_BLENDER)
add_subdirectory(blender)
endif()

add_subdirectory(quantum)
add_subdirectory(memory)
if(SEP_WITH_AUDIO)
    add_subdirectory(audio)
endif()
add_subdirectory(api)
add_subdirectory(cuda)
add_subdirectory(workbench)
add_subdirectory(${CMAKE_SOURCE_DIR}/_sep/testbed ${CMAKE_CURRENT_BINARY_DIR}/_sep_testbed)

add_custom_target(run_all_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    DEPENDS
        quantum_optimizer_tests
        memory_manager_tests
        api_tests
        long_double_math_test
        cuda_api_tests
        workbench_demo_tests
        $<$<BOOL:${SEP_HAS_BLENDER}>:blender_tests>
        $<$<BOOL:${SEP_WITH_AUDIO}>:audio_tests>
        pattern_data_interop_test)
