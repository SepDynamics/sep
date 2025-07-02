# Set C++ standard
# Find required packages
if(SEP_HAS_CUDA)
    find_package(CUDAToolkit REQUIRED)
endif()
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
    $<IF:$<BOOL:${SEP_HAS_CUDA}>,CUDA::cudart,>
    $<IF:$<BOOL:${SEP_HAS_CUDA}>,CUDA::cuda_driver,>
)

# Configure include directories
target_include_directories(cycles_test PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
    $<IF:$<BOOL:${SEP_HAS_CUDA}>,${CUDAToolkit_INCLUDE_DIRS},>
)

# Add test to CTest
add_test(NAME cycles_test COMMAND cycles_test)
set_tests_properties(cycles_test PROPERTIES
    LABELS "unit;cycles"
)
