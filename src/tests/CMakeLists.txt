

# Install headers
install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/tests/
        DESTINATION include/sep/tests
        FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.hpp")

install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/tests/
        DESTINATION include/sep/tests/internal
        FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.hpp")


# Set C++ standard
# Find required packages
find_package(CUDAToolkit REQUIRED)
find_package(fmt REQUIRED)

# Add test sources
set(TEST_SOURCES
    cycles_test.cpp
)

# Create test executable
add_executable(cycles_test ${TEST_SOURCES})

# Link dependencies
target_link_libraries(cycles_test PRIVATE
    GTest::gtest
    GTest::gtest_main
    GTest::gmock
    fmt::fmt
    sep_core
    sep_compat
    CUDA::cudart
    CUDA::cuda_driver
)

# Configure include directories
target_include_directories(cycles_test PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src
    ${CUDAToolkit_INCLUDE_DIRS}
)

# Add test to CTest
add_test(NAME cycles_test COMMAND cycles_test)
set_tests_properties(cycles_test PROPERTIES
    LABELS "unit;cycles"
)
