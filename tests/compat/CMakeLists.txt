if(NOT BUILD_TESTING)
    return()
endif()

find_package(GTest REQUIRED)

add_executable(long_double_math_host_tests long_double_math_test.cpp)
target_include_directories(long_double_math_host_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)
target_link_libraries(long_double_math_host_tests PRIVATE GTest::GTest GTest::Main)
add_test(NAME long_double_math_host_tests COMMAND long_double_math_host_tests)

add_executable(long_double_math_device_tests long_double_math_test.cpp)
target_compile_definitions(long_double_math_device_tests PRIVATE __CUDACC__)
target_include_directories(long_double_math_device_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)
target_link_libraries(long_double_math_device_tests PRIVATE GTest::GTest GTest::Main)
add_test(NAME long_double_math_device_tests COMMAND long_double_math_device_tests)
