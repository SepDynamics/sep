if(NOT BUILD_TESTING)
    return()
endif()

find_package(GTest REQUIRED)

add_executable(testbed_embedding_tests
    simple_embedding_model_test.cpp
)

target_include_directories(testbed_embedding_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(testbed_embedding_tests PRIVATE
    sep_embeddings
    GTest::GTest
    GTest::Main
)

add_test(NAME testbed_embedding_tests COMMAND testbed_embedding_tests)

