include(SetCXXStandard)

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
        sep_compat
        sep_blender
        sep_audio
)

target_include_directories(sep_api
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/include/api
        ${CMAKE_SOURCE_DIR}/include/audio
        $<IF:$<BOOL:${SEP_HAS_BLENDER}>,${CMAKE_SOURCE_DIR}/include/blender,>
        ${CMAKE_SOURCE_DIR}/include/compat
        ${CMAKE_SOURCE_DIR}/include/core
        ${CMAKE_SOURCE_DIR}/include/crow
        ${CMAKE_SOURCE_DIR}/include/memory
        ${CMAKE_SOURCE_DIR}/include/quantum
        $<IF:$<BOOL:${SEP_HAS_BLENDER}>,${CMAKE_SOURCE_DIR}/cycles/src,>
        ${CMAKE_SOURCE_DIR}/extern
        $<IF:$<BOOL:${SEP_HAS_BLENDER}>,${CMAKE_SOURCE_DIR}/extern/cycles/src,>
        $<IF:$<BOOL:${SEP_HAS_BLENDER}>,${CMAKE_SOURCE_DIR}/extern/blender/lib/linux_x64/osl/include,>
        ${CMAKE_SOURCE_DIR}/extern/eigen
)

# Find curl package
find_package(CURL REQUIRED)

target_compile_definitions(sep_api PRIVATE SEP_HAS_BLENDER=1)

if(HIREDIS_FOUND)
    target_link_libraries(sep_api PRIVATE ${HIREDIS_LIBRARIES})
endif()

# Add curl include directories
target_include_directories(sep_api
    PRIVATE
        ${CURL_INCLUDE_DIRS}
)

# Link curl library
target_link_libraries(sep_api PRIVATE ${CURL_LIBRARIES})
