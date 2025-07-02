if(NOT BUILD_TESTING)
    return()
endif()

find_package(GTest REQUIRED)

add_executable(long_double_math_tests
    long_double_math_test.cpp
)

target_include_directories(long_double_math_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(long_double_math_tests PRIVATE
    GTest::GTest
    GTest::Main
)

add_test(NAME long_double_math_tests COMMAND long_double_math_tests)
