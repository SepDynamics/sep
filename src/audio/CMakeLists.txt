add_library(sep_audio STATIC config.cpp pipeline.cpp pipewire_capture.cpp pipewire_stubs.cpp)

target_include_directories(sep_audio
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)
