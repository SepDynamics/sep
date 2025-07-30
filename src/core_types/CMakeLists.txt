# SEP Core Types Library - The One True Normal Reference Frame
add_library(sep_core_types STATIC
    pattern_conversions.cpp
    result.cpp
)

target_include_directories(sep_core_types PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(sep_core_types PUBLIC cxx_std_17)
