cmake_minimum_required(VERSION 3.10)
project(SEPEngine)

# Ensure RTTI is disabled consistently across all targets when building with
# the Crow isolation headers.
add_compile_definitions(CROW_DISABLE_RTTI=1)

# Locate external dependencies required by the engine
find_package(CURL QUIET)
find_package(Hiredis QUIET)
find_package(CUDAToolkit QUIET)

include(FetchContent)

# Add our fixed Crow headers directory BEFORE other include directories
# This ensures our fixed headers are used instead of the problematic ones
include_directories(BEFORE
  ${CMAKE_SOURCE_DIR}/include
)

include_directories(
  ${CMAKE_SOURCE_DIR}/third_party
  ${CMAKE_SOURCE_DIR}/third_party/glm
  ${CMAKE_SOURCE_DIR}/third_party/tbb
)

# Add our fixed Crow headers subdirectory
add_subdirectory(include/crow)

# Fetch the Crow web framework if it is not present
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/third_party/crow/include")
    message(STATUS "Fetching Crow framework...")
    FetchContent_Declare(
        crow
        GIT_REPOSITORY https://github.com/CrowCpp/Crow.git
        GIT_TAG v1.2.1.2
    )
    FetchContent_MakeAvailable(crow)
    include_directories(${crow_SOURCE_DIR}/include)
else()
    include_directories(${CMAKE_SOURCE_DIR}/third_party/crow/include)
endif()
add_subdirectory(src/api)
add_subdirectory(src/audio)
add_subdirectory(src/blender)
add_subdirectory(src/compat)
add_subdirectory(src/core)
add_subdirectory(src/memory)
add_subdirectory(src/quantum)
add_executable(sep_engine src/main.cpp)
target_include_directories(sep_engine
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(sep_engine
    sep_core
    sep_api
    sep_memory
    sep_quantum
    sep_audio
    sep_blender
    sep_compat
    ${CURL_LIBRARIES}
    ${HIREDIS_LIBRARIES}
    ${CUDAToolkit_LIBRARIES})
