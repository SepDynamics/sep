cmake_minimum_required(VERSION 3.16)

add_library(sep_backtester STATIC
    backtester.cpp
    data/data_loader.cpp
    data/json_data_parser.cpp
    core/backtester_engine.cpp
    core/performance_metrics.cpp
    strategies/sep_signal_strategy.cpp
    ui/backtester_tab_controller.cpp
)

add_executable(data_loader_test data/data_loader_test.cpp)

target_link_libraries(data_loader_test PUBLIC sep_backtester)

target_include_directories(sep_backtester PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(sep_backtester PUBLIC
    sep_workbench_lib
)