# Install headers
install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/api/
        DESTINATION src/api
        FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.hpp")

add_library(sep_api STATIC
    rate_limit_middleware.cpp
    auth_middleware.cpp
    bridge.cpp
    crow_error.cpp
    crow_request_adapter.cpp
    js_integration.cpp
    lock_free_rate_limiter.cpp
    logging_middleware.cpp
    crow_adapter.cpp
    client.cpp
    curl_http_client.cpp
    bridge_c.cpp
    ollama_client.cpp
    sep_engine.cpp
    server.cpp
)


target_link_libraries(sep_api
    PUBLIC
        sep_core
        sep_compat
        sep_quantum
        sep_memory
    PRIVATE
        ${CURL_LIBRARIES}
        sep_embeddings
        http_parser::http_parser
)

target_include_directories(sep_api
  PUBLIC
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/engine
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include>
  PRIVATE
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
  SYSTEM PRIVATE
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/extern/crow/include>
)

# Link external dependencies when available
if(HIREDIS_FOUND)
    target_link_libraries(sep_api PRIVATE ${HIREDIS_LIBRARIES})
endif()
