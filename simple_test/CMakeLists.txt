cmake_minimum_required(VERSION 3.15)
project(simple_test_project)

find_package(GTest REQUIRED)

add_executable(simple_test_runner simple_test.cpp)
target_link_libraries(simple_test_runner GTest::gtest GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(simple_test_runner)