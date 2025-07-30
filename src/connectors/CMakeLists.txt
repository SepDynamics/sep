# Find required dependencies
find_package(PkgConfig REQUIRED)
pkg_check_modules(CURL REQUIRED libcurl)

# Create the connectors library
add_library(sep_connectors STATIC
    oanda_connector.cpp
    market_data_converter.cpp
)

target_include_directories(sep_connectors PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/common
PRIVATE
    ${CURL_INCLUDE_DIRS}
)

target_link_libraries(sep_connectors PUBLIC
    sep_engine
    ${CURL_LIBRARIES}
    nlohmann_json::nlohmann_json
)

target_compile_options(sep_connectors PRIVATE ${CURL_CFLAGS_OTHER})

if(MSVC)
    target_compile_options(sep_connectors PRIVATE /FI"${CMAKE_SOURCE_DIR}/src/engine/internal/glm_config.h")
else()
    target_compile_options(sep_connectors PRIVATE -include "${CMAKE_SOURCE_DIR}/src/engine/internal/glm_config.h")
endif()
