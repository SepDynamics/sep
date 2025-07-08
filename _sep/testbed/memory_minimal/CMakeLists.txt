cmake_minimum_required(VERSION 3.16)
project(memory_minimal_tests LANGUAGES CXX)

find_package(GTest REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(memory_minimal_tests
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../tests/memory/memory_tier_manager_test.cpp
)

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../src)

set(MEMORY_SRC
    ${SRC_DIR}/memory/memory_tier.cpp
    ${SRC_DIR}/memory/memory_tier_manager.cpp
    ${SRC_DIR}/core/allocation_metrics.cpp
    ${SRC_DIR}/core/logging.cpp
    ${SRC_DIR}/core/metrics_collector.cpp
    ${SRC_DIR}/core/prometheus_exporter.cpp
    ${SRC_DIR}/core/tracing.cpp
)

target_sources(memory_minimal_tests PRIVATE ${MEMORY_SRC})
target_compile_definitions(memory_minimal_tests PRIVATE SEP_MINIMAL=1)

target_include_directories(memory_minimal_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../include
)

target_link_libraries(memory_minimal_tests PRIVATE
    sep_core
    GTest::GTest
    GTest::Main
)

target_compile_definitions(memory_minimal_tests PRIVATE SEP_MEMORY_MINIMAL)

add_test(NAME memory_minimal_tests COMMAND memory_minimal_tests)
