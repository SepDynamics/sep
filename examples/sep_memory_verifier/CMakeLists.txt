cmake_minimum_required(VERSION 3.10)
project(sep_memory_verifier LANGUAGES CXX)

find_package(PkgConfig REQUIRED)
pkg_check_modules(HIREDIS REQUIRED hiredis)

add_executable(sep_memory_verifier sep_memory_verifier.cpp)

target_include_directories(sep_memory_verifier PRIVATE ${HIREDIS_INCLUDE_DIRS})
target_link_libraries(sep_memory_verifier PRIVATE sep_memory ${HIREDIS_LIBRARIES} benchmark::benchmark)