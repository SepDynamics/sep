if(NOT BUILD_TESTING)
    return()
endif()

if(NOT SEP_HAS_CUDA)
    message(STATUS "CUDA disabled; skipping CUDA tests")
    return()
endif()

find_package(GTest REQUIRED)

add_executable(long_double_math_test
    long_double_math_test.cpp
)
set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/long_double_math_test.cpp PROPERTIES LANGUAGE CXX)

if(SEP_HAS_CUDA)
    set_source_files_properties(long_double_math_test.cpp PROPERTIES LANGUAGE CXX)
endif()

add_executable(cuda_api_tests
    processing_api_test.cpp
)

add_executable(increment_kernel_test
    increment_kernel_test.cpp
)

# Use consistent include paths for the CUDA test so headers from the
# testbed and the main source tree are always resolved correctly.  The
# previous configuration only added the project root and the CUDA testbed
# directory which caused missing header errors on some systems.
target_include_directories(increment_kernel_test PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/_sep/testbed
    ${CMAKE_SOURCE_DIR}/_sep/testbed/cuda
)

target_include_directories(long_double_math_test PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/_sep/testbed
)

target_include_directories(cuda_api_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/_sep/testbed
)


target_link_libraries(long_double_math_test PRIVATE
    sep_compat
    sep_core
    GTest::GTest
    GTest::Main
    CUDA::cudart
    CUDA::cuda_driver
)

target_link_libraries(cuda_api_tests PRIVATE
    sep_compat
    sep_core
    GTest::GTest
    GTest::Main
    CUDA::cudart
    CUDA::cuda_driver
)

target_link_libraries(increment_kernel_test PRIVATE
    testbed_cuda_kernels
    sep_compat
    sep_core
    GTest::GTest
    GTest::Main
    CUDA::cudart
    CUDA::cuda_driver
)

add_test(NAME long_double_math_test COMMAND long_double_math_test)
add_test(NAME cuda_api_tests COMMAND cuda_api_tests)
add_test(NAME increment_kernel_test COMMAND increment_kernel_test)
