if(NOT BUILD_TESTING)
    return()
endif()

find_package(GTest REQUIRED)

add_library(testbed_tests STATIC
    core_error_handler_test.cpp
    audio_pipeline_test.cpp
    api_makejsonresponse_test.cpp
)
add_subdirectory(embeddings)

target_include_directories(testbed_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(testbed_tests PRIVATE
    sep_core
    sep_audio
    sep_api
    GTest::GTest
    GTest::Main
)

add_test(NAME testbed_tests COMMAND testbed_tests)
