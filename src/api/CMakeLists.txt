add_library(sep_api STATIC 
    crow_error.cpp
    lock_free_rate_limiter.cpp
    js_integration.cpp
    rate_limit_middleware.cpp
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

target_include_directories(sep_api
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/include/api
        ${CMAKE_SOURCE_DIR}/include/audio
        ${CMAKE_SOURCE_DIR}/include/blender
        ${CMAKE_SOURCE_DIR}/include/compat
        ${CMAKE_SOURCE_DIR}/include/core
        ${CMAKE_SOURCE_DIR}/include/crow
        ${CMAKE_SOURCE_DIR}/include/memory
        ${CMAKE_SOURCE_DIR}/include/quantum
        ${CMAKE_SOURCE_DIR}/cycles/src
        ${CMAKE_SOURCE_DIR}/extern
        ${CMAKE_SOURCE_DIR}/extern/cycles/src
        ${CMAKE_SOURCE_DIR}/extern/blender/lib/linux_x64/osl/include
        ${CMAKE_SOURCE_DIR}/extern/eigen
)

# Find curl package
find_package(CURL REQUIRED)

# Link external dependencies
target_link_libraries(sep_api
    PRIVATE
        ${CURL_LIBRARIES}
        sep_core
)

if(HIREDIS_FOUND)
    target_link_libraries(sep_api PRIVATE ${HIREDIS_LIBRARIES})
endif()

# Add curl include directories
target_include_directories(sep_api
    PRIVATE
        ${CURL_INCLUDE_DIRS}
)
