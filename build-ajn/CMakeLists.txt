cmake_minimum_required(VERSION 3.15)
project(sep_memory_tests CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(HIREDIS REQUIRED hiredis)

add_library(sep_core STATIC
    src/core/engine.cpp
    src/core/manager.cpp
    src/core/metrics_collector.cpp
    src/core/error_handler.cpp
    src/core/logging/manager.cpp
    src/core/metrics/allocation.cpp
)

add_library(sep_memory STATIC
    src/memory/memory_tier.cpp
    src/memory/memory_tier_manager.cpp
    src/memory/redis_manager.cpp
)

target_include_directories(sep_core PUBLIC include)
target_include_directories(sep_memory PUBLIC include)
target_link_libraries(sep_memory PUBLIC sep_core ${HIREDIS_LIBRARIES})
target_include_directories(sep_memory PUBLIC ${HIREDIS_INCLUDE_DIRS})

add_executable(memory_manager_tests
    tests/memory/memory_tier_manager_test.cpp
)

target_link_libraries(memory_manager_tests
    PRIVATE
    sep_memory
    GTest::GTest
    GTest::Main
)