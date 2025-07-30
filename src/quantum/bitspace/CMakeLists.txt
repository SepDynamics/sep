add_sep_library(sep_quantum_bitspace
    SOURCES
        qbsa.cpp
        qfh.cpp
    pattern_processor.cpp
    DEPENDENCIES
        sep_common
        fmt::fmt
        spdlog::spdlog
)

target_include_directories(sep_quantum_bitspace PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
