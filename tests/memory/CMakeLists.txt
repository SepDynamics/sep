if(NOT BUILD_TESTING)
    return()
endif()

find_package(GTest REQUIRED)

find_package(hiredis REQUIRED)

add_executable(memory_manager_tests
    memory_headers_test.cpp
    memory_manager_test.cpp
    memory_manager_smoke_test.cpp
    memory_tier_manager_block_test.cpp
    memory_tier_manager_test.cpp
    memory_threshold_test.cpp
    promotion_regression_test.cpp
    pattern_init_test.cpp
    pattern_promotion_test.cpp
    redis_manager_test.cpp
)

target_link_libraries(memory_manager_tests PRIVATE
    sep_memory
    sep_manifold
    sep_compat
    sep_core
    GTest::GTest
    GTest::Main
    hiredis
    Threads::Threads
)
if(SEP_HAS_CUDA)
    target_link_libraries(memory_manager_tests PRIVATE CUDA::cudart CUDA::cuda_driver)
endif()

if(SEP_HAS_CUDA)
    target_compile_definitions(memory_manager_tests PRIVATE
        SEP_HAS_CUDA=1
        SEP_USE_CUDA=1
        SEP_HAS_REDIS=1
        SEP_CUDACC_DISABLE_EXCEPTION_SPEC_CHECKS=1
    )
else()
    target_compile_definitions(memory_manager_tests PRIVATE
        SEP_HAS_CUDA=0
        SEP_HAS_REDIS=1
    )
endif()

# Include directories are inherited from linked targets
target_include_directories(memory_manager_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/memory
    ${CMAKE_SOURCE_DIR}/include/core
    ${CMAKE_SOURCE_DIR}/include/compat
)

if(SEP_HAS_CUDA)
    target_compile_definitions(memory_manager_tests PRIVATE
        SEP_HAS_CUDA=1
        SEP_USE_CUDA=1
        SEP_HAS_REDIS=1
    )
else()
    target_compile_definitions(memory_manager_tests PRIVATE
        SEP_HAS_CUDA=0
        SEP_HAS_REDIS=1
    )
endif()

add_test(NAME memory_manager_tests COMMAND memory_manager_tests)
