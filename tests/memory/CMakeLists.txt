if(NOT BUILD_TESTING)
    return()
endif()

find_package(GTest REQUIRED)

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
)

target_include_directories(memory_manager_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/api
    ${CMAKE_SOURCE_DIR}/include/audio
    ${CMAKE_SOURCE_DIR}/include/blender
    ${CMAKE_SOURCE_DIR}/include/compat
    ${CMAKE_SOURCE_DIR}/include/core
    ${CMAKE_SOURCE_DIR}/include/crow
    ${CMAKE_SOURCE_DIR}/include/memory
    ${CMAKE_SOURCE_DIR}/include/quantum
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(memory_manager_tests PRIVATE
    sep_memory
    sep_manifold
    sep_compat
    sep_core
    GTest::GTest
    GTest::Main
)

add_test(NAME memory_manager_tests COMMAND memory_manager_tests)
