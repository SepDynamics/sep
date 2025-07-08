if(NOT BUILD_TESTING)
    return()
endif()

set(SEP_HAS_REDIS 0)

find_package(GTest REQUIRED)

# The unit tests run without Redis. Disable the dependency and
# ensure headers guarded by SEP_NO_REDIS are excluded during
# compilation.
set(SEP_HAS_REDIS 0)
add_compile_definitions(SEP_NO_REDIS)

# Hiredis support is optional for unit tests. The upstream CMake
# configuration shipped with some distributions triggers errors
# when no version is specified.  Since the Redis integration isn't
# required for these tests, simply skip the package lookup if it
# isn't available.
if(FALSE)
    find_package(Hiredis CONFIG)
endif()


set(TEST_SOURCES
    memory_headers_test.cpp
    memory_manager_test.cpp
    memory_manager_smoke_test.cpp
    memory_tier_manager_block_test.cpp
    memory_tier_manager_test.cpp
    memory_threshold_test.cpp
    promotion_regression_test.cpp
    pattern_init_test.cpp
    pattern_promotion_test.cpp
)

if(SEP_HAS_REDIS)
    target_sources(memory_manager_tests PRIVATE redis_manager_test.cpp)
endif()

target_link_libraries(memory_manager_tests PRIVATE
    sep_memory
    sep_manifold
    sep_compat
    fmt::fmt
    sep_core
    GTest::GTest
    GTest::Main
    Threads::Threads
)
if(TARGET Hiredis::Hiredis)
    target_link_libraries(memory_manager_tests PRIVATE Hiredis::Hiredis)
endif()
if(SEP_HAS_CUDA)
    find_package(CUDA REQUIRED)
    target_link_libraries(memory_manager_tests PRIVATE CUDA::cudart CUDA::cuda_driver)
endif()

target_compile_definitions(memory_manager_tests PRIVATE
    SEP_HAS_CUDA=$<IF:$<BOOL:${SEP_HAS_CUDA}>,1,0>
    SEP_USE_CUDA=$<IF:$<BOOL:${SEP_HAS_CUDA}>,1,0>
    SEP_HAS_REDIS=${SEP_HAS_REDIS}
    $<$<NOT:$<BOOL:${SEP_HAS_REDIS}>>:SEP_NO_REDIS>
    SEP_CUDACC_DISABLE_EXCEPTION_SPEC_CHECKS=1
)

# Include directories are inherited from linked targets
target_include_directories(memory_manager_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/memory
    ${CMAKE_SOURCE_DIR}/include/core
    ${CMAKE_SOURCE_DIR}/include/compat
)

target_compile_definitions(memory_manager_tests PRIVATE
    SEP_HAS_CUDA=$<IF:$<BOOL:${SEP_HAS_CUDA}>,1,0>
    SEP_USE_CUDA=$<IF:$<BOOL:${SEP_HAS_CUDA}>,1,0>
    SEP_HAS_REDIS=${SEP_HAS_REDIS}
    $<$<NOT:$<BOOL:${SEP_HAS_REDIS}>>:SEP_NO_REDIS>
)

add_test(NAME memory_manager_tests COMMAND memory_manager_tests)
