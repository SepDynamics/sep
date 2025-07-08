cmake_minimum_required(VERSION 3.16)
project(memory_minimal_tests LANGUAGES CXX)

find_package(GTest REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(memory_minimal_tests
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../tests/memory/memory_tier_manager_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../cross_language_memory_tier_test.cpp
)

find_package(GTest REQUIRED)
find_package(fmt REQUIRED)
find_package(spdlog)
find_package(Threads REQUIRED)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../src)

set(MEMORY_SRC
    ${SRC_DIR}/memory/memory_tier.cpp
    ${SRC_DIR}/memory/memory_tier_manager.cpp
    ${SRC_DIR}/core/logging.cpp
    ${SRC_DIR}/memory/memory_tier_manager_serialization.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/memory_tier_manager_stubs.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/allocation_metrics_stub.cpp
)

target_sources(memory_minimal_tests PRIVATE ${MEMORY_SRC})
target_compile_definitions(memory_minimal_tests PRIVATE SEP_MINIMAL=1)
target_compile_definitions(memory_minimal_tests PRIVATE SEP_TESTBED_STUBS)
target_compile_definitions(memory_minimal_tests PRIVATE SEP_NO_REDIS)
target_compile_definitions(memory_minimal_tests PRIVATE SEP_SOURCE_DIR="${CMAKE_SOURCE_DIR}")

target_include_directories(memory_minimal_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../include
)

target_link_libraries(memory_minimal_tests PRIVATE
    GTest::GTest
    GTest::Main
    fmt::fmt
    spdlog::spdlog
    Threads::Threads
)

target_compile_definitions(memory_minimal_tests PRIVATE SEP_MEMORY_MINIMAL)

add_test(NAME memory_minimal_tests COMMAND memory_minimal_tests)
