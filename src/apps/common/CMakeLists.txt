# Common headers for all apps
add_library(sep_apps_common INTERFACE)

target_include_directories(sep_apps_common INTERFACE
    ${CMAKE_SOURCE_DIR}/src/apps/common
)
