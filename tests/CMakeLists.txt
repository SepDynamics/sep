# Common test configuration
# Configure test output directory
set(TEST_OUTPUT_DIR ${CMAKE_BINARY_DIR}/tests)
file(MAKE_DIRECTORY ${TEST_OUTPUT_DIR})

# Set test data directory
set(SEP_TEST_DATA_DIR ${CMAKE_CURRENT_SOURCE_DIR}/test_data)
file(MAKE_DIRECTORY ${SEP_TEST_DATA_DIR})

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

# Add test subdirectories
if(SEP_HAS_BLENDER)
add_subdirectory(blender)
endif()

add_subdirectory(quantum)
add_subdirectory(memory)
add_subdirectory(audio)
add_subdirectory(api)

# Configure test discovery
include(CTest)
enable_testing()
