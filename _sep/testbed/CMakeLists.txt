if(NOT BUILD_TESTING)
    return()
endif()

find_package(GTest REQUIRED)

add_library(testbed_tests STATIC
    core_error_handler_test.cpp
    audio_pipeline_test.cpp
    audio_factory_test.cpp
    api_makejsonresponse_test.cpp
    neuro_spike_test.cpp
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
    sep_blender
    GTest::GTest
    GTest::Main
)

add_test(NAME testbed_tests COMMAND testbed_tests)

if(SEP_HAS_CUDA)
    add_subdirectory(cuda)
endif()
add_subdirectory(memory_minimal)

add_executable(pattern_data_interop_test
    pattern_data_interop_test.cpp
)

target_include_directories(pattern_data_interop_test PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(pattern_data_interop_test PRIVATE
    GTest::GTest
    GTest::Main
)

add_test(NAME pattern_data_interop_test COMMAND pattern_data_interop_test)
