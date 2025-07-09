

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
set(CYCLES_TEST_SOURCES
    cycles_test.cpp
)

set(MEMORY_TEST_SOURCES
    memory_tier_test.cpp
)

# Create test executables
add_executable(cycles_test ${CYCLES_TEST_SOURCES})
add_executable(memory_tier_test ${MEMORY_TEST_SOURCES})

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

target_link_libraries(memory_tier_test PRIVATE
    GTest::gtest
    GTest::gtest_main
    GTest::gmock
    fmt::fmt
    sep_core
    sep_memory
    sep_compat
)

# Configure include directories
target_include_directories(cycles_test PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
    ${CUDAToolkit_INCLUDE_DIRS}
)

# Set up include directories for memory test
target_include_directories(memory_tier_test PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
)

# Add tests to CTest
add_test(NAME cycles_test COMMAND cycles_test)
set_tests_properties(cycles_test PROPERTIES
    LABELS "unit;cycles"
)

add_test(NAME memory_tier_test COMMAND memory_tier_test)
set_tests_properties(memory_tier_test PROPERTIES
    LABELS "unit;memory"
)
