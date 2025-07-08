add_library(sep_utils STATIC
    dataset_loader.cpp
)

target_include_directories(sep_utils
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/utils/
        DESTINATION include/sep/utils
        FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp")

install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/utils/
        DESTINATION include/sep/utils/internal
        FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp")
