if(NOT BUILD_TESTING)
    return()
endif()

find_package(GTest REQUIRED)

add_executable(workbench_demo_tests
    genesis_pattern_test.cpp
    memory_garden_test.cpp
    annealing_demo_test.cpp
)

target_include_directories(workbench_demo_tests
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/examples/workbench
        ${CMAKE_SOURCE_DIR}/examples/workbench/demos
)

target_link_libraries(workbench_demo_tests
    PRIVATE
        sep_core
        sep_quantum
        sep_memory
        sep_context
        GTest::GTest
        GTest::Main
)

add_test(NAME workbench_demo_tests COMMAND workbench_demo_tests)