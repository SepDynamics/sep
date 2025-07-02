if(NOT BUILD_TESTING)
    return()
endif()

find_package(GTest REQUIRED)

add_executable(long_double_math_test
    long_double_math_test.cpp
)

target_include_directories(long_double_math_test PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(long_double_math_test PRIVATE
    sep_compat
    sep_core
    GTest::GTest
    GTest::Main
    CUDA::cudart
    CUDA::cuda_driver
)

add_test(NAME long_double_math_test COMMAND long_double_math_test)
