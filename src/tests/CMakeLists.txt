

# Install headers
install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/tests/
        DESTINATION src/tests
        FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.hpp")


# Set C++ standard
# Find required packages
find_package(CUDAToolkit REQUIRED)
find_package(fmt REQUIRED)

# Create test executables
# add_executable(memory_tier_test memory_tier_test.cpp)
add_executable(pattern_metric_test pattern_metric_engine_test.cpp)

# Link dependencies
# target_link_libraries(cycles_test PRIVATE
#     GTest::gtest
#     GTest::gtest_main
#     GTest::gmock
#     fmt::fmt
#     sep_core
#     sep_compat
#     CUDA::cudart
#     CUDA::cuda_driver
# )

# target_link_libraries(memory_tier_test PRIVATE
#     GTest::gtest
#     GTest::gtest_main
#     GTest::gmock
#     fmt::fmt
#     sep_core
#     sep_memory
#     sep_compat
# )

# # Set up include directories for memory test
# target_include_directories(memory_tier_test PRIVATE
#     ${CMAKE_SOURCE_DIR}
#     ${CMAKE_SOURCE_DIR}/src
# )


# add_test(NAME memory_tier_test COMMAND memory_tier_test)
# set_tests_properties(memory_tier_test PROPERTIES
#     LABELS "unit;memory"
# )

# Configure pattern metric test
target_link_libraries(pattern_metric_test PRIVATE
    fmt::fmt
    sep_core
    sep_quantum
    sep_compat
    TBB::tbb
)

target_include_directories(pattern_metric_test PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

add_test(NAME pattern_metric_test COMMAND pattern_metric_test)
set_tests_properties(pattern_metric_test PROPERTIES
    LABELS "unit;quantum;pattern"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
