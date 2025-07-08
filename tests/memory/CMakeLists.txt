if(NOT BUILD_TESTING)
    return()
endif()

find_package(GTest REQUIRED)

# Redis is optional for the unit tests.  Some environments may not
# provide a complete CMake configuration for hiredis which would
# otherwise cause configuration to fail.  Search for the package but
# continue silently if it is not available.
find_package(Hiredis 1.0 CONFIG QUIET)

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
    SEP_CUDACC_DISABLE_EXCEPTION_SPEC_CHECKS=1
)

# Include directories are inherited from linked targets
target_include_directories(memory_manager_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/memory
    ${CMAKE_SOURCE_DIR}/include/core
    ${CMAKE_SOURCE_DIR}/include/compat
    ${HIREDIS_INCLUDE_DIRS}
)

target_compile_definitions(memory_manager_tests PRIVATE
    SEP_HAS_CUDA=$<IF:$<BOOL:${SEP_HAS_CUDA}>,1,0>
    SEP_USE_CUDA=$<IF:$<BOOL:${SEP_HAS_CUDA}>,1,0>
    SEP_HAS_REDIS=${SEP_HAS_REDIS}
)

add_test(NAME memory_manager_tests COMMAND memory_manager_tests)
