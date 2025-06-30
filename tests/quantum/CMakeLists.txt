if(NOT BUILD_TESTING)
    return()
endif()

find_package(GTest REQUIRED)

add_executable(quantum_optimizer_tests
    quantum_manifold_optimizer_test.cpp
    pattern_evolution_test.cpp
    pattern_evolution_regression_test.cpp
    process_patterns_update_test.cpp
)

target_include_directories(quantum_optimizer_tests
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/include/api
        ${CMAKE_SOURCE_DIR}/include/audio
        ${CMAKE_SOURCE_DIR}/include/blender
        ${CMAKE_SOURCE_DIR}/include/compat
        ${CMAKE_SOURCE_DIR}/include/core
        ${CMAKE_SOURCE_DIR}/include/crow
        ${CMAKE_SOURCE_DIR}/include/memory
        ${CMAKE_SOURCE_DIR}/include/quantum
)

target_link_libraries(quantum_optimizer_tests
    PRIVATE
        sep_quantum
        GTest::GTest
        GTest::Main
)

add_test(NAME quantum_optimizer_tests COMMAND quantum_optimizer_tests)
