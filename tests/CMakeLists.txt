cmake_minimum_required(VERSION 3.15)
project(pattern_metrics_tests)

find_package(GTest REQUIRED)
add_executable(pattern_metrics_test
    pattern_metrics_test.cpp
    quantum_processor_test.cpp)
target_include_directories(pattern_metrics_test PRIVATE ../src)
target_link_libraries(pattern_metrics_test GTest::gtest GTest::gtest_main sep_quantum)

add_executable(quantum_signal_bridge_test
    quantum_signal_bridge_test.cpp)
target_include_directories(quantum_signal_bridge_test PRIVATE ../src)
target_link_libraries(quantum_signal_bridge_test GTest::gtest GTest::gtest_main sep_quantum sep_trader_cuda sep_trader_logic)

enable_testing()
include(GoogleTest)
gtest_discover_tests(pattern_metrics_test)
