add_sep_library(sep_common
    SOURCES
        financial_data_types.cpp
    DEPENDENCIES
)

target_include_directories(sep_common PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
)
