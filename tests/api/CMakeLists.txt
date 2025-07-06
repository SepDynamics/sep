if(NOT BUILD_TESTING)
    return()
endif()

find_package(GTest REQUIRED)

add_executable(api_tests
    server_test.cpp
    health_endpoint_test.cpp
    long_double_math_test.cpp
    context_api_test.cpp
    context_ops_test.cpp
    embedding_model_test.cpp
)

target_include_directories(api_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(api_tests PRIVATE
    sep_api
    sep_blender
    ${HTTP_PARSER_LIBRARY}
    GTest::GTest
    GTest::Main
)

add_test(NAME api_tests COMMAND api_tests)
