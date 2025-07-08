if(NOT BUILD_TESTING)
    return()
endif()

find_package(GTest REQUIRED)

add_executable(utils_tests
    dataset_loader_test.cpp
)

add_dependencies(utils_tests sep_utils)

target_link_libraries(utils_tests PRIVATE
    sep_utils
    GTest::GTest
    GTest::Main
)

target_include_directories(utils_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

add_test(NAME utils_tests COMMAND utils_tests)
