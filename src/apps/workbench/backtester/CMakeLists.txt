cmake_minimum_required(VERSION 3.16)

add_library(sep_backtester STATIC
    backtester.cpp
)

target_include_directories(sep_backtester PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(sep_backtester PUBLIC
    sep_workbench_lib
)