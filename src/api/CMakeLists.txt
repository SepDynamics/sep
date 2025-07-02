add_library(sep_api STATIC
    crow_error.cpp
    lock_free_rate_limiter.cpp
    js_integration.cpp
    rate_limit_middleware.cpp
    logging_middleware.cpp
    bridge.cpp
    crow_adapter.cpp
    crow_request_adapter.cpp
    curl_http_client.cpp
    auth_middleware.cpp
    client.cpp
    ollama_client.cpp
    sep_engine.cpp
    bridge_c.cpp
    server.cpp
)


target_link_libraries(sep_api
    PUBLIC
        sep_core
    PRIVATE
        ${CURL_LIBRARIES}
        ${HTTP_PARSER_LIBRARY}
        sep_compat
        sep_blender
        sep_audio
)

target_include_directories(sep_api
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

# Link external dependencies when available
if(HIREDIS_FOUND)
    target_link_libraries(sep_api PRIVATE ${HIREDIS_LIBRARIES})
endif()
