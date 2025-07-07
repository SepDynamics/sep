if(NOT BUILD_TESTING)
    return()
endif()

if(SEP_WITH_AUDIO)
    find_package(GTest REQUIRED)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(PIPEWIRE REQUIRED libpipewire-0.3)

    add_executable(audio_tests
        capture_test.cpp
        features_test.cpp
        pipeline_features_test.cpp
        pipeline_test.cpp
        fft_accuracy_test.cpp
        pipewire_lifecycle_test.cpp
        pipewire_capture_metrics_test.cpp
        pipewire_capture_integration_test.cpp
    )

    target_include_directories(audio_tests PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${PIPEWIRE_INCLUDE_DIRS}
    )

    target_link_libraries(audio_tests PRIVATE
        sep_audio
        sep_core
        sep_compat
        GTest::GTest
        GTest::Main
        ${PIPEWIRE_LIBRARIES}
    )

    target_compile_options(audio_tests PRIVATE ${PIPEWIRE_CFLAGS})

    target_compile_definitions(audio_tests PRIVATE
        SEP_HAS_AUDIO=1
    )

    target_compile_definitions(audio_tests PUBLIC
        SEP_AUDIO_EXPORTS
    )

    set_target_properties(audio_tests PROPERTIES
        ENABLE_EXPORTS ON
        WINDOWS_EXPORT_ALL_SYMBOLS ON
        VISIBILITY_INLINES_HIDDEN OFF
        C_VISIBILITY_PRESET default
        CXX_VISIBILITY_PRESET default
    )

    add_test(NAME audio_tests COMMAND audio_tests)
endif()

