add_library(sep_compat STATIC core/stream.cpp core/core.cu raii.cpp)

target_include_directories(sep_compat
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)
