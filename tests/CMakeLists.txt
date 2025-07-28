cmake_minimum_required(VERSION 3.15)
project(pattern_metrics_tests)

find_package(GTest REQUIRED)
add_executable(pattern_metrics_test
    pattern_metrics_test.cpp
    quantum_processor_test.cpp)
target_include_directories(pattern_metrics_test PRIVATE ../src)
target_link_libraries(pattern_metrics_test GTest::gtest GTest::gtest_main)

enable_testing()
include(GoogleTest)
gtest_discover_tests(pattern_metrics_test)
