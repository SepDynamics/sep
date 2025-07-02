if(NOT BUILD_TESTING)
    return()
endif()

find_package(GTest REQUIRED)

add_executable(audio_tests
    capture_test.cpp
    features_test.cpp
    pipeline_features_test.cpp
    pipeline_test.cpp
    fft_accuracy_test.cpp
    pipewire_lifecycle_test.cpp
    pipewire_capture_metrics_test.cpp
)

target_include_directories(audio_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(audio_tests PRIVATE
    sep_audio
    GTest::GTest
    GTest::Main
)

add_test(NAME audio_tests COMMAND audio_tests)

