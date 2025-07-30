add_library(sep_api STATIC auth_middleware.cpp bridge.cpp bridge_c.cpp client.cpp crow_adapter.cpp crow_error.cpp curl_http_client.cpp js_integration.cpp lock_free_rate_limiter.cpp ollama_client.cpp rate_limit_middleware.cpp sep_engine.cpp server.cpp)

target_include_directories(sep_api
    PUBLIC
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/extern
        ${CMAKE_SOURCE_DIR}/extern/cycles/src
        ${CMAKE_SOURCE_DIR}/extern/eigen
)

# Link external dependencies when available
target_link_libraries(sep_api PRIVATE ${CURL_LIBRARIES})
if(HIREDIS_FOUND)
    target_link_libraries(sep_api PRIVATE ${HIREDIS_LIBRARIES})
endif()
target_link_libraries(sep_api PUBLIC crow)

if(MSVC)
    target_compile_options(sep_api PRIVATE /FI"${CMAKE_SOURCE_DIR}/src/engine/internal/glm_config.h")
else()
    target_compile_options(sep_api PRIVATE -include "${CMAKE_SOURCE_DIR}/src/engine/internal/glm_config.h")
endif()
