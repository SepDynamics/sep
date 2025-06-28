if(NOT BUILD_TESTING)
    return()
endif()

find_package(GTest REQUIRED)

add_executable(quantum_optimizer_tests
    quantum_manifold_optimizer_test.cpp
)

target_include_directories(quantum_optimizer_tests
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(quantum_optimizer_tests
    PRIVATE
        sep_quantum
        GTest::GTest
        GTest::Main
)

add_test(NAME quantum_optimizer_tests COMMAND quantum_optimizer_tests)
